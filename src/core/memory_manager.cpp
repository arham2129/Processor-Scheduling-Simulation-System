#include "memory_manager.h"
#include <iostream>

void MemoryManager::allocate_memory(int processID, int size) {
    memoryMap[processID] = size;
    std::cout << "  [MEM] Allocated " << size
              << " units to Process " << processID << ".\n";
}

void MemoryManager::deallocate_memory(int processID) {
    if (memoryMap.erase(processID) > 0) {
        std::cout << "  [MEM] Released memory for Process " << processID << ".\n";
    }
}

void MemoryManager::print_memory_status() const {
    if (memoryMap.empty()) {
        std::cout << "[Memory] No active allocations.\n";
        return;
    }
    std::cout << "[Memory] Current allocations:\n";
    for (const auto& entry : memoryMap) {
        std::cout << "  Process " << entry.first
                  << " => " << entry.second << " units\n";
    }
}

std::size_t MemoryManager::allocation_count() const {
    return memoryMap.size();
}
