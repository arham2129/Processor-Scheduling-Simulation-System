#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include <queue>
#include "scheduler.h"
#include "process.h"

class RoundRobinScheduler : public Scheduler {
private:
    std::queue<Process*> readyQueue;  
    int timeQuantum;                  

public:
    RoundRobinScheduler(int quantum);

    void add_process_to_queue(Process& process) override;

    Process* get_next_process() override;

    void handle_preemption() override;

    void set_time_quantum(int quantum);
};

#endif 
