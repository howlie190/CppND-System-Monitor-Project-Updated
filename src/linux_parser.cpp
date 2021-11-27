#include "linux_parser.h"

#include <dirent.h>
#include <string>
#include <unistd.h>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

std::vector<long> LinuxParser::GetCPUStateValue(std::string cpuNum)
{
    std::ifstream     stream(LinuxParser::kProcDirectory +
                         LinuxParser::kStatFilename);
    std::string       line  = {};
    std::string       str   = {};
    std::vector<long> state = {};
    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::stringstream ss(line);
            ss >> str;
            if (str == cpuNum) {
                long value;
                while (ss >> value) {
                    state.emplace_back(value);
                }
                break;
            }
        }
        stream.close();
        return state;
    }
    else {
        return {};
    }
}

int getDigitInString(std::string& str)
{
    int               rst{0};
    std::stringstream ss{};
    for (auto ch : str) {
        if (isdigit(ch)) {
            ss << ch;
        }
    }
    ss >> rst;
    return rst;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem()
{
    string        line;
    string        key;
    string        value;
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
    }
    return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
    string        os, version, kernel;
    string        line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids()
{
    vector<int>    pids;
    DIR*           directory = opendir(kProcDirectory.c_str());
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
float LinuxParser::MemoryUtilization()
{
    int           memTotal{0};
    int           memFree{0};
    std::string   temp = {};
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {
        while (getline(stream, temp)) {
            if (temp.find("MemTotal") != std::string::npos) {
                memTotal = getDigitInString(temp);
            }
            else if (temp.find("MemFree") != std::string::npos) {
                memFree = getDigitInString(temp);
            }
        }
        return (float)(memTotal - memFree) / memTotal;
    }
    else {
        return 0.0;
    }
}

// TODO: Read and return the system uptime
long int LinuxParser::UpTime()
{
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    long int      rst{0};
    if (stream.is_open()) {
        stream >> rst;
        return rst;
    }
    else {
        return 0;
    }
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
    return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
    std::string dir = LinuxParser::kProcDirectory + std::to_string(pid) +
                      LinuxParser::kStatFilename;
    std::ifstream stream(dir);
    long          value{0};
    long          rst{0};
    if (stream.is_open()) {
        std::string temp;
        for (int i = 0; i < 13; i++) {
            stream >> temp;
        }
        for (int i = 0; i < 4; i++) {
            stream >> value;
            rst += value;
        }
        stream.close();
        return rst;
    }
    else {
        return 0;
    }
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(std::string cpuNum)
{
    std::vector<long> state(GetCPUStateValue(cpuNum));
    try {
        return state.at(CPUStates::kUser_) + state.at(CPUStates::kNice_) +
               state.at(CPUStates::kSystem_) + state.at(CPUStates::kIRQ_) +
               state.at(CPUStates::kSoftIRQ_) + state.at(CPUStates::kSteal_);
    }
    catch (const std::out_of_range& e) {
        return 0;
    }
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(std::string cpuNum)
{
    std::vector<long> state(GetCPUStateValue(cpuNum));
    try {
        return state.at(CPUStates::kIdle_) + state.at(CPUStates::kIOwait_);
    }
    catch (const std::out_of_range& e) {
        return 0;
    }
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
    return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
    std::string   line      = {};
    std::string   processes = {};
    std::ifstream stream(LinuxParser::kProcDirectory +
                         LinuxParser::kStatFilename);
    int           rst{0};
    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::stringstream ss(line);
            ss >> processes;
            if (processes == "processes") {
                ss >> rst;
                break;
            }
        }
        stream.close();
        return rst;
    }
    else {
        return 0;
    }
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
    std::string   line         = {};
    std::string   procsRunning = {};
    std::ifstream stream(LinuxParser::kProcDirectory +
                         LinuxParser::kStatFilename);
    int           rst{0};
    if (stream.is_open()) {
        while (getline(stream, line)) {
            std::stringstream ss(line);
            ss >> procsRunning;
            if (procsRunning == "procs_running") {
                ss >> rst;
                break;
            }
        }
        stream.close();
        return rst;
    }
    else {
        return 0;
    }
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
    std::string dir = LinuxParser::kProcDirectory + "/" + std::to_string(pid) +
                      LinuxParser::kCmdlineFilename;
    std::ifstream stream(dir);
    std::string   line{};
    if (stream.is_open()) {
        getline(stream, line);
        return line;
    }
    else {
        return {};
    }
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
    std::string dir = LinuxParser::kProcDirectory + std::to_string(pid) +
                      LinuxParser::kStatusFilename;
    std::ifstream stream(dir);
    std::string   uid = {};
    std::string   rst = {};
    if (stream.is_open()) {
        std::string line = {};
        while (getline(stream, line)) {
            std::stringstream ss(line);
            ss >> uid;
            if (uid == "VmSize:") {
                ss >> rst;
            }
        }
        stream.close();
        return rst;
    }
    else {
        return {};
    }
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{
    std::string dir = LinuxParser::kProcDirectory + std::to_string(pid) +
                      LinuxParser::kStatusFilename;
    std::ifstream stream(dir);
    std::string   uid = {};
    std::string   rst = {};
    if (stream.is_open()) {
        std::string line = {};
        while (getline(stream, line)) {
            std::stringstream ss(line);
            ss >> uid;
            if (uid == "Uid:") {
                ss >> rst;
            }
        }
        stream.close();
        return rst;
    }
    else {
        return {};
    }
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
    string        targetUid = LinuxParser::Uid(pid);
    string        line;
    string        user;
    string        pwd;
    string        uid;
    std::ifstream stream(kPasswordPath);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::stringstream ss(line);
            ss >> user >> pwd >> uid;
            if (uid == targetUid) {
                break;
            }
        }
        return user;
    }
    else {
        return {};
    }
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
    std::string dir = LinuxParser::kProcDirectory + std::to_string(pid) +
                      LinuxParser::kStatFilename;
    std::ifstream stream(dir);
    long          value{0};
    if (stream.is_open()) {
        std::string temp;
        for (int i = 0; i < 21; i++) {
            stream >> temp;
        }
        stream >> value;
        stream.close();
        return value / sysconf(_SC_CLK_TCK);
    }
    else {
        return 0;
    }
}
