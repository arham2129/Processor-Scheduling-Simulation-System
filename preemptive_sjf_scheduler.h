#ifndef PREEMPTIVE_SJF_SCHEDULER_H
#define PREEMPTIVE_SJF_SCHEDULER_H

#include <vector>
#include "scheduler.h"
#include "process.h"

/**
 * @brief Preemptive Shortest Job First (SRTF) Scheduler.
 *
 * Also known as Shortest Remaining Time First (SRTF).  At every scheduling
 * event the process with the smallest remaining CPU time is selected.  A
 * currently-running process is preempted if a newly-arrived process has a
 * shorter remaining time.
 *
 * Implementation: maintains a std::vector sorted ascending by remaining
 * time so get_next_process() is O(1) and add is O(n log n).
 */
class PreemptiveSJFScheduler : public Scheduler {
private:
    std::vector<Process*> readyQueue; // sorted ascending by remainingTime

    void resort(); // re-sort after add or state change

public:
    void     add_process_to_queue(Process& process) override;
    Process* get_next_process()                     override;
    bool     is_empty()                       const override;

    bool     is_preemptive()                  const override { return true; }
    bool     should_preempt(const Process& running) const override;

    void     handle_preemption(Process* current, Process* incoming) override;
};

#endif // PREEMPTIVE_SJF_SCHEDULER_H
