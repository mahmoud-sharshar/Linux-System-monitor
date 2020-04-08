#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  string hour = std::to_string(seconds / 3600);
  long min_sec = seconds % 3600;
  string minutes = std::to_string(min_sec / 60);
  string sec = std::to_string(min_sec % 60);
  return hour + ":" + minutes + ":" + sec; 
}