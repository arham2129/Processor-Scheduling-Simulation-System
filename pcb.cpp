#include "pcb.h"
#include <iostream>

using namespace std;

PCB::PCB(ProcessState state, int pc, string schedInfo, string memInfo, string accInfo)
    : processState(state), programCounter(pc), schedulingInfo(schedInfo), 
      memoryInfo(memInfo), accountingInfo(accInfo) {}

void PCB::displayPCB() {
    cout << "Process State: " << processState << endl;
    cout << "Program Counter: " << programCounter << endl;
    cout << "CPU Registers: ";
    for (int reg : cpuRegisters) {
        cout << reg << " ";
    }
    cout << endl;
    cout << "Scheduling Info: " << schedulingInfo << endl;
    cout << "Memory Info: " << memoryInfo << endl;
    cout << "Accounting Info: " << accountingInfo << endl;
}