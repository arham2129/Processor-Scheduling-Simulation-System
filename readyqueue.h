#ifndef READYQUEUE_H
#define READYQUEUE_H

#include <queue>
#include "process.h"

class ReadyQueue {
private:
    std::queue<Process*> queue;  

public:
    void add_process(Process* process);

    Process* get_next_process();

    bool is_empty();
};

#endif 
