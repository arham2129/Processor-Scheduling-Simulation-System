#ifndef IO_SUBSYSTEM_H
#define IO_SUBSYSTEM_H

#include <queue>
#include "process.h"

/**
 * @brief Simulates I/O device queuing for blocked processes.
 *
 * When a process issues an I/O request it is moved to the ioQueue.
 * When I/O completes the process transitions back to READY state and
 * should be returned to the scheduler's ready queue by the SimulationEngine.
 */
class IOSubsystem {
private:
    std::queue<Process*> ioQueue;

public:
    /**
     * @brief Enqueue a process that is waiting for I/O.
     * @param process Must not be nullptr.
     */
    void add_io_request(Process* process);

    /**
     * @brief Complete the I/O for the head-of-queue process and transition
     *        it back to READY.
     */
    void complete_io();

    /**
     * @brief Retrieve the process that has just completed I/O (without
     *        completing it — use complete_io() for that).
     * @return Pointer to the head process, or nullptr if the queue is empty.
     */
    Process* peek_completed_io() const;

    /** @brief Print the current I/O queue occupancy to stdout. */
    void print_io_queue_status() const;

    /** @brief Returns true when no processes are waiting for I/O. */
    bool is_empty() const;
};

#endif // IO_SUBSYSTEM_H
