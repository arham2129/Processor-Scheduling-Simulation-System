#ifndef IO_SUBSYSTEM_H
#define IO_SUBSYSTEM_H

#include <queue>
#include "process.h"

class IOSubsystem {
private:
    std::queue<Process*> ioQueue; 

public:
    void add_io_request(Process* process);

    void complete_io();

    void print_io_queue_status();
};

#endif 
