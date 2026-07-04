#include "round_robin_scheduler.h"

RoundRobinScheduler::RoundRobinScheduler(int quantum)
    : timeQuantum(quantum)
{}

void RoundRobinScheduler::add_process_to_queue(Process& process) {
    readyQueue.push(&process);
}

Process* RoundRobinScheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;
    Process* next = readyQueue.front();
    readyQueue.pop();
    return next;
}

bool RoundRobinScheduler::is_empty() const {
    return readyQueue.empty();
}

int RoundRobinScheduler::get_time_quantum() const {
    return timeQuantum;
}

void RoundRobinScheduler::set_time_quantum(int quantum) {
    timeQuantum = quantum;
}
