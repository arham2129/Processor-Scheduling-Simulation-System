#include "preemptive_sjf_scheduler.h"
#include <algorithm>
#include <iostream>

void PreemptiveSJFScheduler::add_process_to_queue(Process& process) {
    readyQueue.push_back(&process);

    std::sort(readyQueue.begin(), readyQueue.end(), [](Process* p1, Process* p2) {
        return p1->getRemainingTime() < p2->getRemainingTime();
    });
}

Process* PreemptiveSJFScheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;

    Process* nextProcess = readyQueue.front();
    readyQueue.erase(readyQueue.begin());
    return nextProcess;
}

void PreemptiveSJFScheduler::handle_preemption(Process* currentProcess, Process* newProcess) {
    if (newProcess->getRemainingTime() < currentProcess->getRemainingTime()) {
        std::cout << "Process " << currentProcess->getProcessID() << " is preempted by Process " 
                  << newProcess->getProcessID() << " (SJF Preemptive)" << std::endl;
        add_process_to_queue(*currentProcess);  
    } else {
        add_process_to_queue(*newProcess);  
    }
}
