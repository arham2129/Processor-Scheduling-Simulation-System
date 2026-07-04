#ifndef READYQUEUE_H
#define READYQUEUE_H

#include <queue>
#include <cstddef>
#include "process.h"

/**
 * @brief A simple FIFO ready queue used by non-preemptive schedulers.
 *
 * Wraps std::queue<Process*> to provide a named, semantically clear interface.
 * Used directly by FCFSScheduler; other schedulers use their own data structures.
 */
class ReadyQueue {
private:
    std::queue<Process*> queue;

public:
    /** @brief Enqueue a process pointer at the back of the queue. */
    void add_process(Process* process);

    /**
     * @brief Dequeue and return the front process.
     * @return Pointer to the next process, or nullptr if the queue is empty.
     */
    Process* get_next_process();

    /** @brief Returns true when no processes are waiting. */
    bool is_empty() const;

    /** @brief Returns the number of processes currently in the queue. */
    std::size_t size() const;
};

#endif // READYQUEUE_H
