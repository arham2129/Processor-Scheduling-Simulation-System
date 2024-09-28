#include "round_robin_scheduler.h"

RoundRobinScheduler::RoundRobinScheduler(int quantum) : timeQuantum(quantum) {}

void RoundRobinScheduler::add_process_to_queue(Process& process) {
    readyQueue.push(&process);
}

Process* RoundRobinScheduler::get_next_process() {
    if (!readyQueue.empty()) {
        Process* nextProcess = readyQueue.front();
        readyQueue.pop();
        return nextProcess;
    }
    return nullptr;  
}

void RoundRobinScheduler::handle_preemption() {
}

void RoundRobinScheduler::set_time_quantum(int quantum) {
    timeQuantum = quantum;
}
