#include "process.h"

#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int processID)
{
    pid    = processID;
    user   = LinuxParser::User(pid);
    upTime = LinuxParser::UpTime() - LinuxParser::UpTime(pid);
    cpuUtilization =
        (float)(LinuxParser::ActiveJiffies(pid) / sysconf(_SC_CLK_TCK)) /
        upTime;
    int               ramkb;
    std::stringstream ss(LinuxParser::Ram(pid));
    ss >> ramkb;
    int rammb = ramkb / 1024;
    ram       = to_string(rammb);
    cmd = LinuxParser::Command(pid);
}

// TODO: Return this process's ID
int Process::Pid()
{
    return pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization()
{
    return cpuUtilization;
}

// TODO: Return the command that generated this process
string Process::Command()
{
    return cmd;
}

// TODO: Return this process's memory utilization
string Process::Ram()
{
    return ram;
}

// TODO: Return the user (name) that generated this process
string Process::User()
{
    return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()
{
    return upTime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const
{
    return cpuUtilization < a.cpuUtilization;
}

bool Process::operator>(Process const& a) const
{
    return cpuUtilization > a.cpuUtilization;
}