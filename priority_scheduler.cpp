#include "priority_scheduler.h"
#include <algorithm>
#include <iostream>

// ---- Private helper -------------------------------------------------------

void PriorityScheduler::resort() {
    std::sort(readyQueue.begin(), readyQueue.end(),
              [](const Process* a, const Process* b) {
                  // Primary: ascending priority number (lower = higher priority)
                  // Tie-break: ascending arrival time (FCFS for equal priority)
                  if (a->getPriority() == b->getPriority()) {
                      return a->getArrivalTime() < b->getArrivalTime();
                  }
                  return a->getPriority() < b->getPriority();
              });
}

// ---- Scheduler interface --------------------------------------------------

void PriorityScheduler::add_process_to_queue(Process& process) {
    readyQueue.push_back(&process);
    resort();
}

Process* PriorityScheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;
    Process* next = readyQueue.front();
    readyQueue.erase(readyQueue.begin());
    return next;
}

bool PriorityScheduler::is_empty() const {
    return readyQueue.empty();
}

bool PriorityScheduler::should_preempt(const Process& running) const {
    if (readyQueue.empty()) return false;
    // Preempt only if a higher-priority (lower number) process is waiting
    return readyQueue.front()->getPriority() < running.getPriority();
}

void PriorityScheduler::handle_preemption(Process* current, Process* incoming) {
    if (current == nullptr || incoming == nullptr) return;

    if (incoming->getPriority() < current->getPriority()) {
        std::cout << "  [Priority] Process " << current->getProcessID()
                  << " (priority=" << current->getPriority() << ")"
                  << " preempted by Process " << incoming->getProcessID()
                  << " (priority=" << incoming->getPriority() << ").\n";
        add_process_to_queue(*current);  // re-queue the evicted process
    } else {
        add_process_to_queue(*incoming);
    }
}
