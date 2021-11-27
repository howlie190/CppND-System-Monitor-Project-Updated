#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

#include <string>

class Processor {
  public:
    Processor(std::string);
    std::string GetCPU();
    void        GetCPUState();
    float       Utilization();  // TODO: See src/processor.cpp

    // TODO: Declare any necessary private members
  private:
    long        prevIdle{0};
    long        prevNonIdle{0};
    long        idle{0};
    long        nonIdle{0};
    std::string cpu = {};
};

#endif