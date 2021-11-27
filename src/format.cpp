#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::string hourFormat{};
    std::string minFormat{};
    std::string secFormat{};
    int         hour = seconds / 3600;
    if (hour < 10) {
        hourFormat = "0" + std::to_string(hour);
    }
    else {
        hourFormat = std::to_string(hour);
    }
    int remainder = seconds % 3600;
    int min       = remainder / 60;
    if (min < 10) {
        minFormat = "0" + std::to_string(min);
    }
    else {
        minFormat = std::to_string(min);
    }
    remainder %= 60;
    int sec = remainder;
    if (sec < 10) {
        secFormat = "0" + std::to_string(sec);
    }
    else {
        secFormat = std::to_string(sec);
    }
    return hourFormat + ":" + minFormat + ":" + secFormat;
 }