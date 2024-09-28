#include "memory_manager.h"

void MemoryManager::allocate_memory(int processID, int size) {
    memoryMap[processID] = size;
    std::cout << "Allocated " << size << " units of memory to Process ID: " << processID << std::endl;
}

void MemoryManager::deallocate_memory(int processID) {
    memoryMap.erase(processID);
    std::cout << "Deallocated memory for Process ID: " << processID << std::endl;
}

void MemoryManager::print_memory_status() {
    std::cout << "Current Memory Allocation:" << std::endl;
    for (const auto& entry : memoryMap) {
        std::cout << "Process ID: " << entry.first << " - Memory Size: " << entry.second << " units" << std::endl;
    }
}
