#include "multilevel_queue_scheduler.h"

MultilevelQueueScheduler::MultilevelQueueScheduler(int userQuantum) : userQueue(userQuantum), userTimeQuantum(userQuantum) {}

void MultilevelQueueScheduler::add_process_to_queue(Process& process) {
    if (process.getPriority() < 2) {
        systemQueue.add_process_to_queue(process);
    } else {
        userQueue.add_process_to_queue(process);
    }
}

Process* MultilevelQueueScheduler::get_next_process() {
    if (Process* systemProcess = systemQueue.get_next_process()) {
        return systemProcess;
    }
    return userQueue.get_next_process();
}
