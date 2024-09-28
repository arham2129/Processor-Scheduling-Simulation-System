#ifndef PREEMPTIVE_SJF_SCHEDULER_H
#define PREEMPTIVE_SJF_SCHEDULER_H

#include <queue>
#include "scheduler.h"
#include "process.h"

struct CompareRemainingTime {
    bool operator()(Process* p1, Process* p2) {
        return p1->getRemainingTime() > p2->getRemainingTime(); 
    }
};

class PreemptiveSJFScheduler : public Scheduler {
private:
    std::priority_queue<Process*, std::vector<Process*>, CompareRemainingTime> readyQueue;
    Process* currentProcess = nullptr; 

public:
    void add_process_to_queue(Process& process) override;

    Process* get_next_process() override;

    void handle_preemption() override;
};

#endif 
