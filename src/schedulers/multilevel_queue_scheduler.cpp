#include "multilevel_queue_scheduler.h"

MultilevelQueueScheduler::MultilevelQueueScheduler(int userQuantum)
    : userQueue(userQuantum),
      userTimeQuantum(userQuantum),
      lastFromSystem(false)
{}

void MultilevelQueueScheduler::add_process_to_queue(Process& process) {
    // Priority < 2  => system process (FCFS, high priority)
    // Priority >= 2 => user process   (Round Robin)
    if (process.getPriority() < 2) {
        systemQueue.add_process_to_queue(process);
    } else {
        userQueue.add_process_to_queue(process);
    }
}

Process* MultilevelQueueScheduler::get_next_process() {
    // System queue always has absolute priority over the user queue
    if (!systemQueue.is_empty()) {
        lastFromSystem = true;
        return systemQueue.get_next_process();
    }
    lastFromSystem = false;
    return userQueue.get_next_process();
}

bool MultilevelQueueScheduler::is_empty() const {
    return systemQueue.is_empty() && userQueue.is_empty();
}

int MultilevelQueueScheduler::get_time_quantum() const {
    // System processes run FCFS (quantum == 0 means "run to completion")
    if (lastFromSystem) return 0;
    return userTimeQuantum;
}
