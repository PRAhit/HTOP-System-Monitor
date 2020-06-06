#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  
  long prevTotal = pIdle + pNonIdle;
  long Total = Idle + NonIdle;
  
  Idle = LinuxParser::IdleJiffies();
  NonIdle = LinuxParser::ActiveJiffies();
  

  long Tot_Delta = Total - prevTotal;
  long Idle_Delta = Idle - pIdle;
  pIdle = Idle;
  pNonIdle = NonIdle;
  return (float)(Tot_Delta - Idle_Delta) / Tot_Delta; }
