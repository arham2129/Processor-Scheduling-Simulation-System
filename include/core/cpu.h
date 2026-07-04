#ifndef CPU_H
#define CPU_H

#include "process.h"

/**
 * @brief Simulates a single CPU core.
 *
 * Tracks the currently executing process and a monotonic clock that
 * advances whenever a process runs.  Context switching is recorded
 * for informational purposes.
 */
class CPU {
private:
    Process* currentProcess; // process currently on the CPU (nullptr if idle)
    int      clock;          // total simulated time elapsed on this CPU

public:
    CPU();

    /**
     * @brief Execute a time slice of @p timeSlice units for @p process.
     *
     * Calls process->simulate_cpu_burst(timeSlice) and advances the
     * internal clock.  Does NOT handle quantum expiry — that is the
     * responsibility of the SimulationEngine.
     *
     * @param process   The process to run.  Must not be nullptr.
     * @param timeSlice Number of time units to execute.  Must be > 0.
     */
    void run_process(Process* process, int timeSlice);

    /**
     * @brief Perform a context switch to @p nextProcess.
     *
     * Updates the internal currentProcess pointer and prints a log message.
     *
     * @param nextProcess  The process to switch to.  Must not be nullptr.
     */
    void context_switch(Process* nextProcess);

    /** @brief Returns the total simulated time the CPU has been running. */
    int  get_clock() const;

    /** @brief Advance the CPU clock by @p timeUnits without running a process. */
    void update_clock(int timeUnits);

    /** @brief Returns the process currently on the CPU, or nullptr if idle. */
    const Process* get_current_process() const;
};

#endif // CPU_H
