#include "priority_scheduler.h"
#include <algorithm>
#include <iostream>

void PriorityScheduler::add_process_to_queue(Process& process) {
    readyQueue.push_back(&process);

    std::sort(readyQueue.begin(), readyQueue.end(), [](Process* p1, Process* p2) {
        return p1->getPriority() < p2->getPriority();
    });
}

Process* PriorityScheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;

    Process* nextProcess = readyQueue.front();
    readyQueue.erase(readyQueue.begin());
    return nextProcess;
}

void PriorityScheduler::handle_preemption(Process* currentProcess, Process* newProcess) {
    if (newProcess->getPriority() < currentProcess->getPriority()) {
        std::cout << "Process " << currentProcess->getProcessID() << " is preempted by Process " 
                  << newProcess->getProcessID() << " (Priority Preemptive)" << std::endl;
        add_process_to_queue(*currentProcess);  
    } else {
        add_process_to_queue(*newProcess);  
    }
}
