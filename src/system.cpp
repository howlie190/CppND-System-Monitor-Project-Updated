#include "system.h"

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

#include <cstddef>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

using std::set;
using std::size_t;
using std::string;
using std::vector;

void System::GetAllProcesses()
{
    if (!processes_.empty()) {
        processes_.clear();
    }
    for (auto pid : LinuxParser::Pids()) {
        processes_.push_back(Process(pid));
    }
    std::sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) { return a > b; });
}

void System::GetAllProcessors()
{
    processors.clear();
    int numberOfProcessors = sysconf(_SC_NPROCESSORS_ONLN);
    for (int i = 0; i < numberOfProcessors; i++) {
        std::string str = "cpu" + std::to_string(i);
        processors.emplace_back(new Processor(str));
    }
}

System::System()
{
    operatingSystem   = LinuxParser::OperatingSystem();
    kernel            = LinuxParser::Kernel();
    memoryUtilization = LinuxParser::MemoryUtilization();
    totalProcesses    = LinuxParser::TotalProcesses();
    runningProcesses  = LinuxParser::RunningProcesses();
    upTime            = LinuxParser::UpTime();
    GetAllProcessors();
    GetAllProcesses();
}

void System::GetCPUState()
{
    for (std::vector<Processor*>::iterator it = processors.begin();
         it != processors.end(); it++) {
        (*it)->GetCPUState();
    }
}

void System::Refresh()
{
    GetCPUState();
    GetAllProcesses();
    memoryUtilization = LinuxParser::MemoryUtilization();
    totalProcesses    = LinuxParser::TotalProcesses();
    runningProcesses  = LinuxParser::RunningProcesses();
    upTime            = LinuxParser::UpTime();
}

// TODO: Return the system's CPU
Processor* System::Cpu(int cpuNum)
{
    std::string                       str = "cpu" + std::to_string(cpuNum);
    std::vector<Processor*>::iterator it  = std::find_if(
        processors.begin(), processors.end(),
        [&str](Processor* const cpu) { return cpu->GetCPU() == str; });
    if (it == processors.end()) {
        return nullptr;
    }
    else {
        return *it;
    }
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes()
{
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel()
{
    return kernel;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization()
{
    return memoryUtilization;
}

// TODO: Return the operating system name
std::string System::OperatingSystem()
{
    return operatingSystem;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses()
{
    return runningProcesses;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses()
{
    return totalProcesses;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime()
{
    return upTime;
}