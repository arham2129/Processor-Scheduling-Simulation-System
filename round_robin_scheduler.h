#ifndef ROUND_ROBIN_SCHEDULER_H
#define ROUND_ROBIN_SCHEDULER_H

#include <queue>
#include "scheduler.h"
#include "process.h"

/**
 * @brief Round Robin (RR) scheduler with configurable time quantum.
 *
 * Each process receives a fixed CPU time slice (quantum).  If it does not
 * complete within that slice it is preempted and moved to the back of the
 * queue, giving every process a fair share of the CPU.
 *
 * Note: The SimulationEngine calls on_quantum_expiry() (inherited default:
 * re-add to queue) when a process exhausts its quantum.  No explicit
 * preemption on arrival (is_preemptive() == false); preemption happens
 * only at quantum boundaries.
 */
class RoundRobinScheduler : public Scheduler {
private:
    std::queue<Process*> readyQueue;
    int timeQuantum;

public:
    /**
     * @param quantum  Time units each process may execute before being
     *                 preempted.  Must be >= 1.
     */
    explicit RoundRobinScheduler(int quantum);

    void     add_process_to_queue(Process& process) override;
    Process* get_next_process()                     override;
    bool     is_empty()                       const override;
    int      get_time_quantum()               const override;

    /** @brief Update the time quantum (e.g. from user menu option 3). */
    void set_time_quantum(int quantum);
};

#endif // ROUND_ROBIN_SCHEDULER_H
