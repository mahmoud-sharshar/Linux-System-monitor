#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
    filestream.close();
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
    stream.close();
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string memTotal;
  string memFree;
  string line;
  std::ifstream memInfo(kProcDirectory + kMeminfoFilename);
  if(memInfo.is_open()){
    memInfo >> memTotal >> memTotal;
    getline(memInfo,line);
    memInfo >> memFree >> memFree;
    memInfo.close();
  }
  float utilization = float(std::stoll(memTotal)-std::stoll(memFree))/float(std::stoll(memTotal));
  return utilization; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string value;
  std::ifstream file(kProcDirectory + kUptimeFilename);
  if(file.is_open()){
  	file >> value;
  }
  file.close();
  return std::stol(value);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  	std::vector<std::string> cpuStat = LinuxParser::CpuUtilization();
  	long long idle = std::stoll(cpuStat[3]) + std::stoll(cpuStat[4]);
    long long nonIdle = std::stoll(cpuStat[0]) + std::stoll(cpuStat[1])
      				  + std::stoll(cpuStat[2]) + std::stoll(cpuStat[5])
      				  + std::stoll(cpuStat[6]) + std::stoll(cpuStat[7]);
   long long total = idle + nonIdle;
  return total; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long utime = 0;
  long stime = 0;
  long cutime = 0;
  long cstime = 0;
  std::ifstream file(kProcDirectory +to_string(pid) +kStatFilename);
  if(file.is_open()){
  	int i=1;
    string value;
    while(i < 14){
    	file >> value;
      i++;
    }
    file >> value;
    utime = std::stol(value);
//     std::cout << utime << "\n";
    file >> value;
    stime = std::stol(value);
    file >> value;
    cutime = std::stol(value);
    file >> value;
    cstime = std::stol(value);
  }
  return utime + stime + cutime + cstime; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::vector<std::string> cpuStat = LinuxParser::CpuUtilization();
  long long nonIdle = std::stoll(cpuStat[0]) + std::stoll(cpuStat[1])
      				  + std::stoll(cpuStat[2]) + std::stoll(cpuStat[5])
      				  + std::stoll(cpuStat[6]) + std::stoll(cpuStat[7]);
  return nonIdle; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::vector<std::string> cpuStat = LinuxParser::CpuUtilization();
  long idle = std::stoll(cpuStat[3]) + std::stoll(cpuStat[4]);
  return idle; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string value;
  vector<string> cpuStat;
  std::ifstream file(kProcDirectory + kStatFilename);
  if(file.is_open()){
    getline(file, line);
    std::istringstream linestream(line);
    linestream >> value;
    while(linestream >> value){
    	cpuStat.push_back(value);
    }	
  }
  file.close();
  return cpuStat; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream file(kProcDirectory + kStatFilename);
  if(file.is_open()){
    while(file >> key){
    	if(key =="processes"){
        	file >> value;
          break;
        }
      getline(file,line);
    }	
  }
  file.close();
  return std::stoi(value); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
 string line;
  string key;
  string value;
  std::ifstream file(kProcDirectory + kStatFilename);
  if(file.is_open()){
    while(file >> key){
    	if(key =="procs_running"){
        	file >> value;
          break;
        }
      getline(file,line);
    }	
  }
  file.close();
  return std::stoi(value); 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream cmdFile( kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(cmdFile.is_open()){
  	 getline(cmdFile,line);
  }
  cmdFile.close();
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream ramFile(kProcDirectory +to_string(pid) + kStatusFilename);
  if(ramFile.is_open()){
  	while(getline(ramFile,line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmSize:"){
      	break;
      }
    }
    ramFile.close();
  }
//   std::cout << key;
  return value; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream ramFile(kProcDirectory +to_string(pid) + kStatusFilename);
  if(ramFile.is_open()){
  	while(getline(ramFile,line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "Uid:"){
      	break;
      }
    }
    ramFile.close();
  }
//   std::cout << key;
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  int user_id = std::stoi(LinuxParser::Uid(pid));
  string line;
  string key;
  string value;
  std::ifstream file(kPasswordPath);
  if(file.is_open()){
  	while(getline(file,line)){
    	std::replace(line.begin(),line.end(),':',' ');
        std::istringstream linestream(line);
      	linestream >> key >>  value >> value;
      if(std::stoi(value) == user_id){
      		break;
      }
    }
    file.close();
  }
  return key; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long startTime = 0;
  std::ifstream file(kProcDirectory +to_string(pid) +kStatFilename);
  if(file.is_open()){
  	int i=1;
    string value;
    while(i < 22){
    	file >> value;
      i++;
    }
    file >> value;
    startTime = std::stol(value) / sysconf(_SC_CLK_TCK);
    file.close();
  }
	return startTime;
}