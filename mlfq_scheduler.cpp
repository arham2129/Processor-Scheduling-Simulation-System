#include "mlfq_scheduler.h"

MLFQScheduler::MLFQScheduler(std::vector<int> quantums) : timeQuantums(quantums) {
    feedbackQueues.resize(timeQuantums.size());
}

void MLFQScheduler::add_process_to_queue(Process& process) {
    feedbackQueues[0].push(&process); 
}

Process* MLFQScheduler::get_next_process() {
    for (size_t i = 0; i < feedbackQueues.size(); ++i) {
        if (!feedbackQueues[i].empty()) {
            Process* nextProcess = feedbackQueues[i].front();
            feedbackQueues[i].pop();
            return nextProcess;
        }
    }
    return nullptr; 
}

void MLFQScheduler::demote_process(Process* process, int currentQueueLevel) {
    if (currentQueueLevel + 1 < feedbackQueues.size()) {
        feedbackQueues[currentQueueLevel + 1].push(process);
        cout << "Process " << process->getProcessID() << " has been demoted to lower priority queue." << endl;
    } else {

        feedbackQueues[currentQueueLevel].push(process);
    }
}
