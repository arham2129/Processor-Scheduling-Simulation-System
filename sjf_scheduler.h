#ifndef SJF_SCHEDULER_H
#define SJF_SCHEDULER_H

#include <vector>
#include "scheduler.h"
#include "process.h"

/**
 * @brief Non-preemptive Shortest Job First (SJF) scheduler.
 *
 * At each scheduling point the process with the smallest ORIGINAL burst
 * time is selected.  Once started, a process runs to completion without
 * interruption.
 *
 * Implementation: processes are inserted into a sorted std::vector
 * (ascending burst time) so that get_next_process() is always O(1).
 * Insertion is O(n log n) due to std::sort after each addition.
 */
class SJFScheduler : public Scheduler {
private:
    std::vector<Process*> readyQueue;

public:
    void     add_process_to_queue(Process& process) override;
    Process* get_next_process()                     override;
    bool     is_empty()                       const override;

    // Non-preemptive: base-class defaults apply for all preemption/quantum methods.
};

#endif // SJF_SCHEDULER_H
