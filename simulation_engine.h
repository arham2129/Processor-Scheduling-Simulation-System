#ifndef SIMULATION_ENGINE_H
#define SIMULATION_ENGINE_H

#include <queue>
#include <vector>
#include <fstream>
#include "process.h"
#include "cpu.h"
#include "memory_manager.h"
#include "io_subsystem.h"
#include "scheduler.h"

struct Event {
    int time;  
    std::string type;  
    Process* process; 

    bool operator<(const Event& other) const {
        return time > other.time;  
    }
};

class SimulationEngine {
private:
    int currentTime;  
    std::priority_queue<Event> eventQueue;  
    CPU cpu;  
    MemoryManager memoryManager; 
    IOSubsystem ioSubsystem;  
    Scheduler* scheduler;  
    
    std::string ganttChart;  
    std::ofstream logFile;  

    int totalWaitingTime;
    int totalTurnaroundTime;
    int totalResponseTime;
    int totalProcessCount;
    int cpuBusyTime;
    int contextSwitchCount;
    std::vector<int> waitingTimes;  

public:
    SimulationEngine(Scheduler* sched);

    void add_event(int time, const std::string& type, Process* process);

    void run_simulation(int endTime);

    void handle_event(const Event& event);

    void generate_gantt_chart(Process* process, int burstTime);

    void log_event(const std::string& eventDescription);

    void calculate_metrics(Process* process);

    void print_performance_metrics();

    void save_simulation_results(const std::string& filename);
};

#endif 
