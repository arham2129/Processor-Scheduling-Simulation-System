#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <map>
#include <cstddef>

/**
 * @brief Simulates a simple memory manager that tracks allocations by process ID.
 *
 * In this simulation each process is given a fixed-size memory block.
 * The manager tracks which blocks are allocated and prints status on request.
 */
class MemoryManager {
private:
    std::map<int, int> memoryMap; // processID -> allocated size (in units)

public:
    /**
     * @brief Allocate @p size memory units for process @p processID.
     *        If the process already has an allocation it is overwritten.
     */
    void allocate_memory(int processID, int size);

    /**
     * @brief Release the memory allocation for process @p processID.
     *        No-op if the process has no allocation.
     */
    void deallocate_memory(int processID);

    /** @brief Print all current allocations to stdout. */
    void print_memory_status() const;

    /** @brief Returns the total number of currently-allocated processes. */
    std::size_t allocation_count() const;
};

#endif // MEMORY_MANAGER_H
