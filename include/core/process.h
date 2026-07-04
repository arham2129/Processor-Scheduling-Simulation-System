#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

/**
 * @brief Represents the lifecycle state of a process.
 *
 * Using enum class for strong type safety — prevents accidental implicit
 * conversions to/from int that unscoped enums allow.
 */
enum class ProcessState {
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

/**
 * @brief Represents a process in the scheduling simulation.
 *
 * Stores all attributes needed to simulate scheduling decisions and compute
 * accurate performance metrics (waiting time, turnaround time, response time).
 */
class Process {
private:
    int processID;
    int arrivalTime;
    int burstTime;       // original (total) CPU burst requirement
    int remainingTime;   // time still needed on CPU
    int priority;        // lower number = higher priority
    ProcessState state;
    std::vector<std::string> resourceRequirements;

    // Timing fields — all initialised to -1 (sentinel: "not yet set")
    int startTime;        // first time the process was placed on the CPU
    int responseTime;     // absolute clock time of first CPU access
    int completionTime;   // absolute clock time when the process finishes

public:
    /**
     * @brief Construct a Process with all required scheduling attributes.
     *
     * @param pid       Unique process identifier (must be > 0).
     * @param arrival   Time unit at which the process enters the system.
     * @param burst     Total CPU time required (must be > 0).
     * @param prio      Scheduling priority (lower = higher priority).
     * @param resources List of resource tags the process needs.
     */
    Process(int pid, int arrival, int burst, int prio,
            const std::vector<std::string>& resources);

    // ---- Accessors --------------------------------------------------------
    int          getProcessID()     const;
    int          getArrivalTime()   const;
    int          getBurstTime()     const;
    int          getRemainingTime() const;
    int          getPriority()      const;
    int          getResponseTime()  const;   // -1 if never scheduled
    int          getCompletionTime() const;  // -1 if not yet complete
    ProcessState getState()         const;

    // ---- Mutators ---------------------------------------------------------

    /**
     * @brief Deducts @p timeSlice from remainingTime (clamps to zero).
     */
    void simulate_cpu_burst(int timeSlice);

    /** @brief Transition the process to a new lifecycle state. */
    void update_state(ProcessState newState);

    /** @brief Record the first time this process was placed on the CPU. */
    void set_start_time(int time);

    /**
     * @brief Record the absolute simulation time of first CPU access.
     *        Ignored if called more than once (response time is immutable
     *        after the first scheduling event).
     */
    void set_response_time(int time);

    /** @brief Record the absolute simulation time at which the process finished. */
    void set_completion_time(int time);

    // ---- Queries ----------------------------------------------------------
    /** @brief Returns true when remainingTime == 0. */
    bool is_completed() const;
};

#endif // PROCESS_H
