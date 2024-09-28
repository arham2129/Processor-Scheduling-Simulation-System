#ifndef PCB_H
#define PCB_H

#include <string>
#include <vector>
#include "process.h" 

using namespace std;

class PCB {
public:
    ProcessState processState;
    int programCounter;
    vector<int> cpuRegisters;
    string schedulingInfo;
    string memoryInfo;
    string accountingInfo;

    PCB(ProcessState state, int pc, string schedInfo, string memInfo, string accInfo);
    void displayPCB();
};

#endif // PCB_H