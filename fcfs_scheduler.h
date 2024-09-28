#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "scheduler.h"
#include "readyqueue.h"

class FCFSScheduler : public Scheduler {
private:
    ReadyQueue readyQueue;  

public:
    void add_process_to_queue(Process& process) override;

    Process* get_next_process() override;

    void handle_preemption() override {}
};

#endif
