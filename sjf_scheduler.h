#ifndef SJF_SCHEDULER_H
#define SJF_SCHEDULER_H

#include <queue>
#include "scheduler.h"
#include "process.h"

struct CompareBurstTime {
    bool operator()(Process* p1, Process* p2) {
        return p1->getBurstTime() > p2->getBurstTime();  
    }
};

class SJFScheduler : public Scheduler {
private:
    std::priority_queue<Process*, std::vector<Process*>, CompareBurstTime> readyQueue;

public:
    void add_process_to_queue(Process& process) override;

    Process* get_next_process() override;

    void handle_preemption() override {}
};

#endif 
