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


int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  Total = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  Idle = LinuxParser::IdleJiffies();
  totalDiff = (float)(Total - PrevTotal);
  idleDiff = (float)(Idle-PrevIdle);
  CPU_percent = ((totalDiff - idleDiff)/totalDiff);
  PrevTotal = Total; 
  PrevIdle = Idle;
  return CPU_percent; }

string Process::Command() { return LinuxParser::Command(Pid()); }

string Process::Ram() { return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid());}

long int Process::UpTime() { 
  uptime = LinuxParser::UpTime() - (LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK));
  return uptime;
  }

bool Process::operator<(Process const& a) const { 
 return cpu_utilize_ > a.cpu_utilize_; }
