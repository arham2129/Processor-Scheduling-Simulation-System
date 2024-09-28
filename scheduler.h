#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

class Scheduler {
public:
    virtual void add_process_to_queue(Process& process) = 0;

    virtual Process* get_next_process() = 0;

    virtual void handle_preemption() = 0;

    virtual ~Scheduler() = default;
};

#endif 
