#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }


// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  Total = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  Idle = LinuxParser::IdleJiffies();
  totalDiff = (float)(Total - PrevTotal);
  idleDiff = (float)(Idle-PrevIdle);
  CPU_percent = ((totalDiff - idleDiff)/totalDiff);
  PrevTotal = Total; 
  PrevIdle = Idle;
  return CPU_percent; }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid());}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  uptime = LinuxParser::UpTime() - (LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK));
  return uptime;
  }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { 
 return cpu_utilize_ > a.cpu_utilize_; }