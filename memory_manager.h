#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <map>
#include <iostream>

class MemoryManager {
private:
    std::map<int, int> memoryMap; 

public:
    void allocate_memory(int processID, int size);

    void deallocate_memory(int processID);

    void print_memory_status();
};

#endif 
