#include "cpu.h"
#include <iostream>

CPU::CPU() : currentProcess(nullptr), clock(0) {}

void CPU::run_process(Process* process, int timeSlice) {
    currentProcess = process;
    std::cout << "Running Process ID: " << process->getProcessID() << " for " << timeSlice << " time units." << std::endl;
    process->simulate_cpu_burst(timeSlice);  
    update_clock(timeSlice); 
}

void CPU::context_switch(Process* nextProcess) {
    std::cout << "Context switch: Switching from Process ID: " 
              << (currentProcess ? currentProcess->getProcessID() : -1)
              << " to Process ID: " << nextProcess->getProcessID() << std::endl;
    currentProcess = nextProcess;
}

int CPU::get_clock() const {
    return clock;
}

void CPU::update_clock(int timeUnits) {
    clock += timeUnits;
}
