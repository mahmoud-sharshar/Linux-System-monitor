#include "processor.h"
#include <vector>
#include <string>
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
	std::vector<std::string> cpuStat = LinuxParser::CpuUtilization();
  	long long idle = std::stoll(cpuStat[3]) + std::stoll(cpuStat[4]);
    long long nonIdle = std::stoll(cpuStat[0]) + std::stoll(cpuStat[1])
      				  + std::stoll(cpuStat[2]) + std::stoll(cpuStat[5])
      				  + std::stoll(cpuStat[6]) + std::stoll(cpuStat[7]);
   long long total = idle + nonIdle;
  float cpu_percent = float(nonIdle) / total;
  return cpu_percent; 
}