#include "process.h"

Process::Process(int pid, int arrival, int burst, int priority, const std::vector<std::string>& resources)
    : processID(pid), arrivalTime(arrival), burstTime(burst), remainingTime(burst), 
      priority(priority), state(NEW), resourceRequirements(resources), startTime(-1), responseTime(-1) {}

int Process::getProcessID() const { return processID; }
int Process::getArrivalTime() const { return arrivalTime; }
int Process::getBurstTime() const { return burstTime; }
int Process::getRemainingTime() const { return remainingTime; }
int Process::getPriority() const { return priority; }
int Process::getResponseTime() const { return responseTime; }
ProcessState Process::getState() const { return state; }

void Process::simulate_cpu_burst(int timeSlice) {
    if (remainingTime > 0) {
        remainingTime -= timeSlice;
        if (remainingTime < 0) remainingTime = 0; 
    }
}

void Process::update_state(ProcessState newState) {
    state = newState;
}

void Process::set_start_time(int start) {
    startTime = start;
}

// Set response time (first time the process is scheduled)
void Process::set_response_time(int time) {
    if (responseTime == -1) {
        responseTime = time;
    }
}

// Check if the process is completed
bool Process::is_completed() const {
    return remainingTime == 0;
}
