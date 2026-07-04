#ifndef PCB_H
#define PCB_H

#include <string>
#include <vector>
#include <ostream>
#include "process.h"

/**
 * @brief Process Control Block — stores the saved context of a process.
 *
 * In a real OS the PCB is saved and restored on each context switch.
 * In this simulation it models the concept for educational completeness.
 *
 * Note: std::string and std::vector are qualified explicitly (no
 *       "using namespace std" in headers — doing so would pollute every
 *       translation unit that includes this file).
 */
class PCB {
public:
    ProcessState         processState;
    int                  programCounter;
    std::vector<int>     cpuRegisters;
    std::string          schedulingInfo;
    std::string          memoryInfo;
    std::string          accountingInfo;

    /**
     * @param state      Current ProcessState.
     * @param pc         Saved program counter value.
     * @param schedInfo  Human-readable scheduling context (e.g. priority).
     * @param memInfo    Human-readable memory allocation summary.
     * @param accInfo    Accounting information (CPU time used, etc.).
     */
    PCB(ProcessState state, int pc,
        const std::string& schedInfo,
        const std::string& memInfo,
        const std::string& accInfo);

    /** @brief Print all PCB fields to stdout for debugging purposes. */
    void displayPCB() const;

    /** @brief Print all PCB fields to the given output stream. */
    void displayPCB(std::ostream& out) const;
};

#endif // PCB_H