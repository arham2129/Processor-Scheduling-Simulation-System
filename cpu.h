#ifndef CPU_H
#define CPU_H

#include "process.h"

class CPU {
private:
    Process* currentProcess;  
    int clock; 

public:
    CPU();

    void run_process(Process* process, int timeSlice);

    void context_switch(Process* nextProcess);

    int get_clock() const;

    void update_clock(int timeUnits);
};

#endif 
