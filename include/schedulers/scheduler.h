#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

/**
 * @brief Abstract base class for all CPU scheduling algorithms.
 *
 * Provides a uniform interface for the SimulationEngine to interact with
 * any scheduling strategy without knowing concrete implementation details.
 *
 * Design notes:
 *  - handle_preemption() has a default no-op, suitable for non-preemptive
 *    schedulers.  Preemptive schedulers override it.
 *  - on_quantum_expiry() defaults to simply re-adding the process to the
 *    queue.  MLFQ overrides it to demote the process.
 *  - is_preemptive() signals whether the SimulationEngine should check for
 *    mid-burst preemption on every new process arrival.
 *  - should_preempt() lets the scheduler itself decide whether the currently
 *    running process ought to be evicted in favour of a queued process.
 *  - get_time_quantum() returns 0 to mean "run to completion" (FCFS / SJF).
 */
class Scheduler {
public:
    // ---- Queue management -------------------------------------------------

    /** @brief Add a process to this scheduler's ready queue. */
    virtual void add_process_to_queue(Process& process) = 0;

    /**
     * @brief Remove and return the next process to execute.
     * @return Pointer to the chosen process, or nullptr if the queue is empty.
     */
    virtual Process* get_next_process() = 0;

    /** @brief Returns true when no processes are waiting in the queue. */
    virtual bool is_empty() const = 0;

    // ---- Preemption support -----------------------------------------------

    /**
     * @brief Called when a new process arrives while another is executing.
     *        Default: no-op (non-preemptive behaviour).
     *
     * @param current  The process currently holding the CPU.
     * @param incoming The newly arrived process.
     */
    virtual void handle_preemption(Process* /*current*/, Process* /*incoming*/) {}

    /**
     * @brief Called when the running process exhausts its time quantum.
     *        Default: re-add the process to the back of the queue.
     *        MLFQ overrides this to perform queue-level demotion.
     *
     * @param process The process whose quantum has expired.
     */
    virtual void on_quantum_expiry(Process& process) {
        add_process_to_queue(process);
    }

    /**
     * @brief Returns true if this scheduler supports mid-burst preemption
     *        triggered by new process arrivals (SRTF, Preemptive Priority).
     */
    virtual bool is_preemptive() const { return false; }

    /**
     * @brief Returns true if the currently running process should yield the
     *        CPU because a higher-priority process is now in the queue.
     *
     *        Only meaningful when is_preemptive() == true.
     *
     * @param running Reference to the process currently on the CPU.
     */
    virtual bool should_preempt(const Process& /*running*/) const { return false; }

    // ---- Time quantum -----------------------------------------------------

    /**
     * @brief Returns the time quantum for the current scheduling context.
     *        Returns 0 to indicate "run to completion" (non-time-sliced).
     *
     *        For MLFQ this reflects the quantum of the most recently
     *        scheduled queue level (call after get_next_process()).
     */
    virtual int get_time_quantum() const { return 0; }

    virtual ~Scheduler() = default;
};

#endif // SCHEDULER_H
