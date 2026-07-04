#include "mlfq_scheduler.h"
#include <iostream>
#include <algorithm>

MLFQScheduler::MLFQScheduler(const std::vector<int>& quantums)
    : timeQuantums(quantums), lastScheduledLevel(0)
{
    feedbackQueues.resize(timeQuantums.size());
}

void MLFQScheduler::add_process_to_queue(Process& process) {
    // New / returning processes always enter at the highest-priority queue
    feedbackQueues[0].push(&process);
}

Process* MLFQScheduler::get_next_process() {
    for (std::size_t i = 0; i < feedbackQueues.size(); ++i) {
        if (!feedbackQueues[i].empty()) {
            lastScheduledLevel = static_cast<int>(i);
            Process* next = feedbackQueues[i].front();
            feedbackQueues[i].pop();
            return next;
        }
    }
    return nullptr;
}

bool MLFQScheduler::is_empty() const {
    for (const auto& q : feedbackQueues) {
        if (!q.empty()) return false;
    }
    return true;
}

void MLFQScheduler::on_quantum_expiry(Process& process) {
    demote_process(&process, lastScheduledLevel);
}

int MLFQScheduler::get_time_quantum() const {
    // Return the quantum that was in effect for the last scheduled process.
    // Callers must invoke this AFTER get_next_process() to get a meaningful value.
    if (!timeQuantums.empty()) {
        return timeQuantums[static_cast<std::size_t>(lastScheduledLevel)];
    }
    return 0;
}

void MLFQScheduler::update_time_quantums(const std::vector<int>& quantums) {
    timeQuantums = quantums;
    // Resize queue array to match the new number of levels
    feedbackQueues.resize(timeQuantums.size());
}

void MLFQScheduler::demote_process(Process* process, int currentQueueLevel) {
    int maxLevel = static_cast<int>(feedbackQueues.size()) - 1;
    int nextLevel = std::min(currentQueueLevel + 1, maxLevel);
    feedbackQueues[static_cast<std::size_t>(nextLevel)].push(process);
    std::cout << "  [MLFQ] Process " << process->getProcessID()
              << " demoted to queue level " << nextLevel
              << " (quantum=" << timeQuantums[static_cast<std::size_t>(nextLevel)] << ").\n";
}
