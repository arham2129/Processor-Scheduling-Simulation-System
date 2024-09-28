#ifndef MULTILEVEL_QUEUE_SCHEDULER_H
#define MULTILEVEL_QUEUE_SCHEDULER_H

#include "scheduler.h"
#include "fcfs_scheduler.h"
#include "round_robin_scheduler.h"

class MultilevelQueueScheduler : public Scheduler {
private:
    FCFSScheduler systemQueue;  
    RoundRobinScheduler userQueue;  
    int userTimeQuantum;  

public:
    MultilevelQueueScheduler(int userQuantum);

    void add_process_to_queue(Process& process) override;

    Process* get_next_process() override;

    void handle_preemption() override {}
};

#endif 
