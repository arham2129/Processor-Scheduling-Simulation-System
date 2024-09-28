#include "sjf_scheduler.h"

void SJFScheduler::add_process_to_queue(Process& process) {
    readyQueue.push(&process);
}

Process* SJFScheduler::get_next_process() {
    if (!readyQueue.empty()) {
        Process* nextProcess = readyQueue.top();
        readyQueue.pop();
        return nextProcess;
    }
    return nullptr; 
}
