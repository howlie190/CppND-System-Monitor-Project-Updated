#ifndef PROCESS_H
#define PROCESS_H

#include "linux_parser.h"

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
  public:
    Process(int);
    int         Pid();                              // TODO: See src/process.cpp
    std::string User();                             // TODO: See src/process.cpp
    std::string Command();                          // TODO: See src/process.cpp
    float       CpuUtilization();                   // TODO: See src/process.cpp
    std::string Ram();                              // TODO: See src/process.cpp
    long int    UpTime();                           // TODO: See src/process.cpp
    bool        operator<(Process const& a) const;  // TODO: See src/process.cpp
    bool        operator>(Process const& a) const;

    // TODO: Declare any necessary private members
  private:
    int         pid{0};
    std::string user = {};
    float       cpuUtilization{0.0};
    std::string ram = {};
    long int    upTime{0};
    std::string cmd = {};
};

#endif