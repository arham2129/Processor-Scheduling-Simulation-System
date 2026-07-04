#ifndef MULTILEVEL_QUEUE_SCHEDULER_H
#define MULTILEVEL_QUEUE_SCHEDULER_H

#include "scheduler.h"
#include "fcfs_scheduler.h"
#include "round_robin_scheduler.h"

/**
 * @brief Multilevel Queue Scheduler.
 *
 * Divides processes into two fixed queues based on priority:
 *  - System queue (priority < 2):  FCFS, always runs to completion.
 *  - User queue   (priority >= 2): Round Robin with configurable quantum.
 *
 * System processes always take precedence over user processes.
 * Processes cannot migrate between queues (unlike MLFQ).
 */
class MultilevelQueueScheduler : public Scheduler {
private:
    FCFSScheduler         systemQueue;
    RoundRobinScheduler   userQueue;
    int                   userTimeQuantum;
    bool                  lastFromSystem; // true if last scheduled from system queue

public:
    /**
     * @param userQuantum  Time quantum for the user-level Round Robin queue.
     */
    explicit MultilevelQueueScheduler(int userQuantum);

    void     add_process_to_queue(Process& process) override;
    Process* get_next_process()                     override;
    bool     is_empty()                       const override;

    /**
     * @brief Returns 0 (FCFS) for system-queue processes, or the user quantum
     *        for user-queue processes.  Must be called after get_next_process().
     */
    int      get_time_quantum()               const override;
};

#endif // MULTILEVEL_QUEUE_SCHEDULER_H
