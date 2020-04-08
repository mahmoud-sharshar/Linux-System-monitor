#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid){
	this->pid = pid;
  	this-> startTime = LinuxParser::UpTime(pid);
}
// TODO: Return this process's ID
int Process::Pid() { 
  return this->pid; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  long cpu_time = LinuxParser::UpTime() - this->startTime;
  float cpu_usage = float(LinuxParser::ActiveJiffies(this->pid)/ sysconf(_SC_CLK_TCK)) / float(cpu_time);
  return cpu_usage; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
  return LinuxParser::Command(this->pid); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
  long long mem = std::stoll(LinuxParser::Ram(this->pid)) / 1000;
  return to_string(mem); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(this->pid); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return this->startTime; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return true; }
