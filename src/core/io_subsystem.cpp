#include "io_subsystem.h"
#include <iostream>

void IOSubsystem::add_io_request(Process* process) {
    if (process == nullptr) return;
    ioQueue.push(process);
    process->update_state(ProcessState::BLOCKED);
    std::cout << "  [I/O] Process " << process->getProcessID()
              << " is waiting for I/O.\n";
}

void IOSubsystem::complete_io() {
    if (ioQueue.empty()) return;
    Process* process = ioQueue.front();
    ioQueue.pop();
    process->update_state(ProcessState::READY);
    std::cout << "  [I/O] Process " << process->getProcessID()
              << " completed I/O and is now READY.\n";
}

Process* IOSubsystem::peek_completed_io() const {
    if (ioQueue.empty()) return nullptr;
    return ioQueue.front();
}

void IOSubsystem::print_io_queue_status() const {
    std::cout << "[I/O Queue] " << ioQueue.size()
              << " process(es) waiting for I/O.\n";
}

bool IOSubsystem::is_empty() const {
    return ioQueue.empty();
}
