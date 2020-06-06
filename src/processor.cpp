#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() { 
  
  Idle = LinuxParser::IdleJiffies();
  NonIdle = LinuxParser::ActiveJiffies();
  
  long pTotal = pIdle + pNonIdle;
  long Total = Idle + NonIdle;
  
  
  

  long Tot_Delta = Total - pTotal;
  long Idle_Delta = Idle - pIdle;
  pIdle = Idle;
  pNonIdle = NonIdle;
  return (float)(Tot_Delta - Idle_Delta) / Tot_Delta; }
