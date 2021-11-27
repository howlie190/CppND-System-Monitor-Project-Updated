#include "processor.h"

Processor::Processor(std::string cpuName)
{
    cpu = cpuName;
    GetCPUState();
}
std::string Processor::GetCPU()
{
    return cpu;
}

void Processor::GetCPUState()
{
    idle    = LinuxParser::IdleJiffies(cpu);
    nonIdle = LinuxParser::ActiveJiffies(cpu);
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
    long prevTotal = prevIdle + prevNonIdle;
    long total     = idle + nonIdle;
    long totald    = total - prevTotal;
    long idled     = idle - prevIdle;

    prevIdle    = idle;
    prevNonIdle = nonIdle;

    if (totald == 0) {
        return 0.0;
    }
    else {
        return (totald - idled) / (float)totald;
    }
}