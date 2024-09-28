#include "readyqueue.h"

void ReadyQueue::add_process(Process* process) {
    queue.push(process);
}

Process* ReadyQueue::get_next_process() {
    if (!queue.empty()) {
        Process* next_process = queue.front();
        queue.pop();
        return next_process;
    }
    return nullptr;  
}

bool ReadyQueue::is_empty() {
    return queue.empty();
}
