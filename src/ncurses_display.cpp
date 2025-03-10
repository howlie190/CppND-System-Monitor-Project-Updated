#include "ncurses_display.h"

#include "format.h"
#include "system.h"

#include <chrono>
#include <curses.h>
#include <string>
#include <thread>
#include <vector>

using std::string;
using std::to_string;

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
std::string NCursesDisplay::ProgressBar(float percent)
{
    std::string result{"0%"};
    int         size{20};
    float       bars{percent * size};

    for (int i{0}; i < size; ++i) {
        result += i <= bars ? '|' : ' ';
    }

    string display{to_string(percent * 100).substr(0, 4)};
    if (percent < 0.1 || percent == 1.0)
        display = " " + to_string(percent * 100).substr(0, 3);
    return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window)
{
    int row{0};
    mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
    mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
    mvwprintw(window, ++row, 2, "CPU: ");
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row, 10, "");
    if (system.Cpu(0) == nullptr) {
        wprintw(window, ProgressBar(0.0).c_str());
    }
    else {
        wprintw(window, ProgressBar(system.Cpu(0)->Utilization()).c_str());
    }
    wattroff(window, COLOR_PAIR(1));
    for (int i = 0; i < sysconf(_SC_NPROCESSORS_ONLN); i++) {
        std::string str = "CPU" + to_string(i) + ": ";
        char*       chr = new char[str.size()];
        std::copy(str.begin(), str.end(), chr);
        mvwprintw(window, i + 1, 43, chr);
        wattron(window, COLOR_PAIR(1));
        mvwprintw(window, i + 1, 51, "");
        if (system.Cpu(i + 1) == nullptr) {
            wprintw(window, ProgressBar(0.0).c_str());
        }
        else {
            wprintw(window,
                    ProgressBar(system.Cpu(i + 1)->Utilization()).c_str());
        }
        wattroff(window, COLOR_PAIR(1));
    }
    mvwprintw(window, ++row, 2, "Memory: ");
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row, 10, "");
    wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
    wattroff(window, COLOR_PAIR(1));
    mvwprintw(
        window, ++row, 2,
        ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
    mvwprintw(
        window, ++row, 2,
        ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
    mvwprintw(window, ++row, 2,
              ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
    wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,
                                      WINDOW*               window,
                                      int                   n)
{
    int       row{0};
    int const pid_column{2};
    int const user_column{9};
    int const cpu_column{16};
    int const ram_column{26};
    int const time_column{35};
    int const command_column{46};
    wattron(window, COLOR_PAIR(2));
    mvwprintw(window, ++row, pid_column, "PID");
    mvwprintw(window, row, user_column, "USER");
    mvwprintw(window, row, cpu_column, "CPU[%%]");
    mvwprintw(window, row, ram_column, "RAM[MB]");
    mvwprintw(window, row, time_column, "TIME+");
    mvwprintw(window, row, command_column, "COMMAND");
    wattroff(window, COLOR_PAIR(2));
    int const num_processes = int(processes.size()) > n ? n : processes.size();
    for (int i = 0; i < num_processes; ++i) {
        mvwprintw(window, ++row, pid_column,
                  to_string(processes[i].Pid()).c_str());
        mvwprintw(window, row, user_column, processes[i].User().c_str());
        float cpu = processes[i].CpuUtilization() * 100;
        mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 4).c_str());
        mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
        mvwprintw(window, row, time_column,
                  Format::ElapsedTime(processes[i].UpTime()).c_str());
        mvwprintw(window, row, command_column,
                  processes[i].Command().substr(0, window->_maxx - 46).c_str());
    }
}

void NCursesDisplay::Display(System& system, int n)
{
    initscr();      // start ncurses
    noecho();       // do not print input values
    cbreak();       // terminate ncurses on ctrl + c
    start_color();  // enable color

    int     x_max{getmaxx(stdscr)};
    WINDOW* system_window = newwin(10, x_max - 1, 0, 0);
    WINDOW* process_window =
        newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

    while (1) {
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        box(system_window, 0, 0);
        box(process_window, 0, 0);
        DisplaySystem(system, system_window);
        DisplayProcesses(system.Processes(), process_window, n);
        wrefresh(system_window);
        wrefresh(process_window);
        system.Refresh();
        refresh();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    endwin();
}
