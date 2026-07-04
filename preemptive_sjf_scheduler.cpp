#include "preemptive_sjf_scheduler.h"
#include <algorithm>
#include <iostream>

// ---- Private helper -------------------------------------------------------

void PreemptiveSJFScheduler::resort() {
    std::sort(readyQueue.begin(), readyQueue.end(),
              [](const Process* a, const Process* b) {
                  // Primary: ascending remaining time
                  // Tie-break: ascending arrival time (FCFS for equal remaining)
                  if (a->getRemainingTime() == b->getRemainingTime()) {
                      return a->getArrivalTime() < b->getArrivalTime();
                  }
                  return a->getRemainingTime() < b->getRemainingTime();
              });
}

// ---- Scheduler interface --------------------------------------------------

void PreemptiveSJFScheduler::add_process_to_queue(Process& process) {
    readyQueue.push_back(&process);
    resort();
}

Process* PreemptiveSJFScheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;
    Process* next = readyQueue.front();
    readyQueue.erase(readyQueue.begin());
    return next;
}

bool PreemptiveSJFScheduler::is_empty() const {
    return readyQueue.empty();
}

bool PreemptiveSJFScheduler::should_preempt(const Process& running) const {
    if (readyQueue.empty()) return false;
    // Preempt only if the best queued process has strictly less remaining time
    return readyQueue.front()->getRemainingTime() < running.getRemainingTime();
}

void PreemptiveSJFScheduler::handle_preemption(Process* current, Process* incoming) {
    if (current == nullptr || incoming == nullptr) return;

    if (incoming->getRemainingTime() < current->getRemainingTime()) {
        std::cout << "  [SRTF] Process " << current->getProcessID()
                  << " (remaining=" << current->getRemainingTime() << ")"
                  << " preempted by Process " << incoming->getProcessID()
                  << " (remaining=" << incoming->getRemainingTime() << ").\n";
        add_process_to_queue(*current);  // re-queue the current process
    } else {
        // New process does not preempt; just queue it
        add_process_to_queue(*incoming);
    }
}
