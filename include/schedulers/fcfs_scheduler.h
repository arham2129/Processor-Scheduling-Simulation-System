#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "scheduler.h"
#include "readyqueue.h"

/**
 * @brief First-Come, First-Served (FCFS) non-preemptive scheduler.
 *
 * Processes are executed in strict arrival order.  Once a process starts
 * running it is not interrupted until it completes its full CPU burst.
 */
class FCFSScheduler : public Scheduler {
private:
    ReadyQueue readyQueue;

public:
    void     add_process_to_queue(Process& process) override;
    Process* get_next_process()                     override;
    bool     is_empty()                       const override;

    // Non-preemptive: preemption and quantum methods use base-class defaults
    // (handle_preemption is no-op, get_time_quantum returns 0, is_preemptive
    //  returns false).
};

#endif // FCFS_SCHEDULER_H
