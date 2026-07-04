#include "pcb.h"
#include <iostream>

PCB::PCB(ProcessState state, int pc,
         const std::string& schedInfo,
         const std::string& memInfo,
         const std::string& accInfo)
    : processState(state),
      programCounter(pc),
      schedulingInfo(schedInfo),
      memoryInfo(memInfo),
      accountingInfo(accInfo)
{}

void PCB::displayPCB() const {
    displayPCB(std::cout);
}

void PCB::displayPCB(std::ostream& out) const {
    out << "Process State    : " << static_cast<int>(processState) << "\n"
        << "Program Counter  : " << programCounter << "\n"
        << "CPU Registers    : ";
    for (int reg : cpuRegisters) {
        out << reg << " ";
    }
    out << "\n"
        << "Scheduling Info  : " << schedulingInfo << "\n"
        << "Memory Info      : " << memoryInfo     << "\n"
        << "Accounting Info  : " << accountingInfo << "\n";
}