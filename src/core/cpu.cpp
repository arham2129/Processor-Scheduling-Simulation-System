#include "cpu.h"
#include <iostream>
#include <stdexcept>

CPU::CPU()
    : currentProcess(nullptr), clock(0)
{}

void CPU::run_process(Process* process, int timeSlice) {
    if (process == nullptr) {
        throw std::invalid_argument("CPU::run_process called with nullptr process.");
    }
    if (timeSlice <= 0) {
        throw std::invalid_argument("CPU::run_process called with non-positive timeSlice.");
    }
    currentProcess = process;
    process->simulate_cpu_burst(timeSlice);
    update_clock(timeSlice);
}

void CPU::context_switch(Process* nextProcess) {
    if (nextProcess == nullptr) {
        throw std::invalid_argument("CPU::context_switch called with nullptr nextProcess.");
    }
    std::cout << "  [CPU] Context switch: "
              << (currentProcess ? std::to_string(currentProcess->getProcessID()) : "idle")
              << " -> P" << nextProcess->getProcessID() << "\n";
    currentProcess = nextProcess;
}

int CPU::get_clock() const {
    return clock;
}

void CPU::update_clock(int timeUnits) {
    clock += timeUnits;
}

const Process* CPU::get_current_process() const {
    return currentProcess;
}
