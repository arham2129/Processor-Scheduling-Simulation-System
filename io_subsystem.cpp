#include "io_subsystem.h"
#include <iostream>

void IOSubsystem::add_io_request(Process* process) {
    ioQueue.push(process);
    std::cout << "Process ID: " << process->getProcessID() << " is waiting for I/O." << std::endl;
}

void IOSubsystem::complete_io() {
    if (!ioQueue.empty()) {
        Process* process = ioQueue.front();
        ioQueue.pop();
        std::cout << "Process ID: " << process->getProcessID() << " has completed I/O." << std::endl;
        process->update_state(READY);  
    }
}

void IOSubsystem::print_io_queue_status() {
    std::cout << "Current I/O Queue: " << ioQueue.size() << " processes waiting." << std::endl;
}
