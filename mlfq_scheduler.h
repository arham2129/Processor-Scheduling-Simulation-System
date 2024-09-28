#ifndef MLFQ_SCHEDULER_H
#define MLFQ_SCHEDULER_H

#include <queue>
#include <vector>
#include "scheduler.h"
#include "process.h"

class MLFQScheduler : public Scheduler {
private:
    std::vector<std::queue<Process*>> feedbackQueues;
    std::vector<int> timeQuantums; 

public:
    MLFQScheduler(std::vector<int> quantums);

    void add_process_to_queue(Process& process) override;

    Process* get_next_process() override;

    void demote_process(Process* process, int currentQueueLevel);

    void handle_preemption() override {}
};

#endif 
