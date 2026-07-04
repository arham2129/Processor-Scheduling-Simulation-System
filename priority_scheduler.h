#ifndef PRIORITY_SCHEDULER_H
#define PRIORITY_SCHEDULER_H

#include <vector>
#include "scheduler.h"
#include "process.h"

/**
 * @brief Preemptive Priority Scheduler.
 *
 * Always runs the process with the highest priority (lowest priority number).
 * A running process is preempted immediately when a higher-priority process
 * arrives in the queue.
 *
 * Implementation: maintains a std::vector sorted ascending by priority number
 * (lower number = higher priority) so get_next_process() is O(1).
 * Insertion is O(n log n) due to re-sorting.
 *
 * Note: This file was MISSING from the original project — its absence caused
 * a linker error because priority_scheduler.cpp referenced PriorityScheduler
 * which was never declared.
 */
class PriorityScheduler : public Scheduler {
private:
    std::vector<Process*> readyQueue; // sorted ascending by priority number

    void resort(); // re-sort after add

public:
    void     add_process_to_queue(Process& process) override;
    Process* get_next_process()                     override;
    bool     is_empty()                       const override;

    bool     is_preemptive()                  const override { return true; }
    bool     should_preempt(const Process& running) const override;

    void     handle_preemption(Process* current, Process* incoming) override;
};

#endif // PRIORITY_SCHEDULER_H
