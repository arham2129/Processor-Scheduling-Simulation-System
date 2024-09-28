#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

enum ProcessState {
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

class Process {
private:
    int processID;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    ProcessState state;
    std::vector<std::string> resourceRequirements;
    int startTime;
    int responseTime;

public:
    Process(int pid, int arrival, int burst, int priority, const std::vector<std::string>& resources);

    int getProcessID() const;
    int getArrivalTime() const;
    int getBurstTime() const;
    int getRemainingTime() const;
    int getPriority() const;
    int getResponseTime() const;
    ProcessState getState() const;

    void simulate_cpu_burst(int timeSlice);

    void update_state(ProcessState newState);

    void set_start_time(int startTime);

    void set_response_time(int time);

    bool is_completed() const;
};

#endif 
