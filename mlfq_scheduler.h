#ifndef MLFQ_SCHEDULER_H
#define MLFQ_SCHEDULER_H

#include <queue>
#include <vector>
#include "scheduler.h"
#include "process.h"

/**
 * @brief Multilevel Feedback Queue (MLFQ) Scheduler.
 *
 * Implements the classic MLFQ algorithm:
 *  - New processes always enter the highest-priority queue (level 0).
 *  - Each queue level has its own time quantum (longer for lower-priority queues).
 *  - When a process exhausts its quantum it is demoted to the next level.
 *  - The scheduler always picks from the highest-priority non-empty queue.
 *
 * This approximates "interactive vs. CPU-bound" process separation without
 * knowing a priori which category a process belongs to.
 *
 * Time quantums are supplied as a vector; element [0] is the shortest
 * (highest-priority) and the last element is the longest (lowest-priority).
 * Traditionally: quantums[i+1] = quantums[i] * 2.
 */
class MLFQScheduler : public Scheduler {
private:
    std::vector<std::queue<Process*>> feedbackQueues;
    std::vector<int>                  timeQuantums;
    int                               lastScheduledLevel; // level of last scheduled process

public:
    /**
     * @param quantums  Time quantum for each queue level (must be non-empty,
     *                  all values >= 1).
     */
    explicit MLFQScheduler(const std::vector<int>& quantums);

    void     add_process_to_queue(Process& process) override;
    Process* get_next_process()                     override;
    bool     is_empty()                       const override;

    /**
     * @brief Demote the process to the next lower-priority queue.
     *        Called automatically by on_quantum_expiry().
     */
    void on_quantum_expiry(Process& process)        override;

    /**
     * @brief Returns the time quantum for the most-recently-scheduled level.
     *        Must be called AFTER get_next_process() for correct results.
     */
    int  get_time_quantum()                   const override;

    /**
     * @brief Replace all time quantums (and resize queues if needed).
     *        Used by the CLI "set time quantum" option.
     */
    void update_time_quantums(const std::vector<int>& quantums);

    /**
     * @brief Manually demote a process to a specific queue level.
     * @param process           The process to demote.
     * @param currentQueueLevel The level it is currently in.
     */
    void demote_process(Process* process, int currentQueueLevel);
};

#endif // MLFQ_SCHEDULER_H
