#include "sjf_scheduler.h"
#include <algorithm>

void SJFScheduler::add_process_to_queue(Process& process) {
    readyQueue.push_back(&process);
    // Keep queue sorted ascending by original burst time so the front is
    // always the shortest job — O(n log n) but n is small for a simulation.
    std::sort(readyQueue.begin(), readyQueue.end(),
              [](const Process* a, const Process* b) {
                  // Tie-break by arrival time for deterministic output
                  if (a->getBurstTime() == b->getBurstTime()) {
                      return a->getArrivalTime() < b->getArrivalTime();
                  }
                  return a->getBurstTime() < b->getBurstTime();
              });
}

Process* SJFScheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;
    Process* next = readyQueue.front();
    readyQueue.erase(readyQueue.begin());
    return next;
}

bool SJFScheduler::is_empty() const {
    return readyQueue.empty();
}
