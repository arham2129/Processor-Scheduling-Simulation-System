#include "readyqueue.h"

void ReadyQueue::add_process(Process* process) {
    queue.push(process);
}

Process* ReadyQueue::get_next_process() {
    if (!queue.empty()) {
        Process* next = queue.front();
        queue.pop();
        return next;
    }
    return nullptr;
}

bool ReadyQueue::is_empty() const {
    return queue.empty();
}

std::size_t ReadyQueue::size() const {
    return queue.size();
}
