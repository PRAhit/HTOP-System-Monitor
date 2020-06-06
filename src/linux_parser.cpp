#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::stol;
using std::stoi;


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}
 
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line, key;
  float totalMemo, freeMemo;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key){
      if (key == "MemTotal:") 
        linestream >> totalMemo;
      if (key == "MemAvailable:") 
        linestream >> freeMemo;
      }
    }
  }
  return (totalMemo - freeMemo) / totalMemo;
}

long LinuxParser::UpTime() { 
  std::string line, value;
  std::ifstream stream(kProcDirectory + kUptimeFilename); 
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream upTimeStream(line); 
    upTimeStream >> value;
  	return std::stol(value);
 }
return std::stol(value);
}

long LinuxParser::Jiffies() { 
return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); }


long LinuxParser::ActiveJiffies(int pid) {
  string utime, stime, cutime, cstime, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    int count = 0;
    while (filestream >> value) { 
      if( count == 13){
        utime = std::stol(value);
      } 
      else if(count == 14){
        stime = std::stol(value);
      }
      else if(count == 15){
        cutime = std::stol(value);
      }
      else if(count == 16){
        count = std::stol(value);
        break;
      }
      count++;
    }
  }
  return std::stol(utime) + std::stol(stime) + std::stol(cutime) + std::stol(cstime);
} 

long LinuxParser::ActiveJiffies() { 
  string line;
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;  
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user>> nice >>system>>idle>>iowait>>irq>> softirq>>steal>>guest>>guest_nice;
  }
  return (std::stol(user)+std::stol(nice)+std::stol(system)+std::stol(irq)+
          std::stol(softirq)+std::stol(steal)+std::stol(guest)+ std::stol(guest_nice));
}

long LinuxParser::IdleJiffies() { 
  string line;
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;  
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user>> nice >>system>>idle>>iowait>>irq>> softirq>>steal>>guest>>guest_nice;
  }
  return (std::stol(idle)+std::stol(iowait));
}

vector<string> LinuxParser::CpuUtilization() {
  return {};
}

int LinuxParser::TotalProcesses() {
  string key;
  string line;
  string value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);
} 

int LinuxParser::RunningProcesses() { 
  string key;
  string line;
  string value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return std::stoi(value);
} 


string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}


string LinuxParser::Ram(int pid) { 
  string value;
  string key;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> value){
      if (key == "VmSize:") {
        return value;
      }}
    }
  }
  return value;
}


string LinuxParser::Uid(int pid) {
  
  string key;
  string line;
  string uid_val;
  
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key >> uid_val){
      if (key == "Uid:") {
        return uid_val;
      }
    }}
  }
  return uid_val;
}


string LinuxParser::User(int pid) {

  string key;
  string line;
  string x;
  string u_name;
  string uid = Uid(pid);
  std::ifstream fileStream(LinuxParser::kPasswordPath);
  if (fileStream.is_open()){
    while(getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      lineStream >> u_name >> x >> key; 
      if (key == uid) {
        return u_name;
      }
    }
  }
  return u_name;
}


long LinuxParser::UpTime(int pid) { 
  string key, line;
  long int upTime{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; ++i) {
      linestream >> key;
    }
    linestream >> upTime;
    upTime = upTime / sysconf(_SC_CLK_TCK);
    return upTime;
  }
  return upTime;
}
