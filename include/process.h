#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  int Pid();                               
  std::string User();                      
  std::string Command();                 
  float CpuUtilization();                 
  std::string Ram();                      
  long int UpTime();                      
  bool operator<(Process const& a) const;  

  Process(int pid);
  
  

 private:
  float CPU_percent; 
  long PrevTotal = 0, PrevIdle = 0; 
  long Total, Idle;
  float totalDiff, idleDiff;
  int pid_;
  long uptime;
  float cpu_utilize_;

};

#endif
