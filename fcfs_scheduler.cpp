#include "fcfs_scheduler.h"

void FCFSScheduler::add_process_to_queue(Process& process) {
    readyQueue.add_process(&process);
}

Process* FCFSScheduler::get_next_process() {
    return readyQueue.get_next_process();
}
