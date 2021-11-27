#ifndef SYSTEM_H
#define SYSTEM_H

#include "process.h"
#include "processor.h"

#include <string>
#include <vector>

class System {
  public:
    System();
    void                  Refresh();
    void                  GetAllProcessors();
    void                  GetCPUState();
    void                  GetAllProcesses();
    Processor*            Cpu(int);             // TODO: See src/system.cpp
    std::vector<Process>& Processes();          // TODO: See src/system.cpp
    float                 MemoryUtilization();  // TODO: See src/system.cpp
    long                  UpTime();             // TODO: See src/system.cpp
    int                   TotalProcesses();     // TODO: See src/system.cpp
    int                   RunningProcesses();   // TODO: See src/system.cpp
    std::string           Kernel();             // TODO: See src/system.cpp
    std::string           OperatingSystem();    // TODO: See src/system.cpp

    // TODO: Define any necessary private members
  private:
    long int                upTime{0};
    float                   memoryUtilization{0.0};
    int                     totalProcesses{0};
    int                     runningProcesses{0};
    std::string             operatingSystem = {};
    std::string             kernel          = {};
    std::vector<Processor*> processors      = {};
    std::vector<Process>    processes_      = {};
};

#endif