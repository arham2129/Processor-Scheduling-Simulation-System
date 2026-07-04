#ifndef SIMULATION_ENGINE_H
#define SIMULATION_ENGINE_H

#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include "process.h"
#include "cpu.h"
#include "memory_manager.h"
#include "io_subsystem.h"
#include "scheduler.h"

// ---------------------------------------------------------------------------
// Supporting types
// ---------------------------------------------------------------------------

/**
 * @brief A single interval in the Gantt chart: process ran from startTime
 *        to endTime.
 */
struct GanttEntry {
    int processID;
    int startTime;
    int endTime;
};

/**
 * @brief Aggregated performance metrics computed after a simulation run.
 */
struct PerformanceMetrics {
    double avgWaitingTime     = 0.0; ///< Mean waiting time across all processes
    double avgTurnaroundTime  = 0.0; ///< Mean turnaround time across all processes
    double avgResponseTime    = 0.0; ///< Mean response time (arrival → first CPU)
    double cpuUtilization     = 0.0; ///< % of simulated time the CPU was busy
    double throughput         = 0.0; ///< Completed processes per time unit
    double fairnessStdDev     = 0.0; ///< Standard deviation of waiting times
    int    contextSwitches    = 0;   ///< Total context switches performed
    int    totalProcesses     = 0;   ///< Number of processes that completed
    int    simulationDuration = 0;   ///< Total simulated time units elapsed
};

/**
 * @brief An event in the engine's arrival-event priority queue.
 *
 * Only ARRIVAL events are stored; CPU scheduling is driven internally by the
 * simulation loop (not by queued CPU events).
 */
struct Event {
    int         time;
    std::string type;    ///< "ARRIVAL"
    Process*    process;

    /// Min-heap ordering: smallest time first
    bool operator<(const Event& other) const {
        return time > other.time;
    }
};

// ---------------------------------------------------------------------------
// SimulationEngine
// ---------------------------------------------------------------------------

/**
 * @brief Drives the full CPU scheduling simulation.
 *
 * Architecture:
 *  - Callers register process arrivals via add_event().
 *  - run_simulation() executes the scheduler loop, advancing simulated time
 *    in jumps (not tick-by-tick) for efficiency.
 *  - The engine respects time quantums, preemption, and process demotion
 *    through the polymorphic Scheduler interface.
 *  - All results (Gantt chart + metrics) are stored internally and can be
 *    printed to stdout or saved to a file.
 *
 * Correctness fixes over the original implementation:
 *  - Burst time is taken from the process, not hardcoded to 4.
 *  - CPU events are no longer injected externally — the engine drives itself.
 *  - Response time is measured as (first CPU access − arrival time).
 *  - Waiting time is measured as (turnaround − original burst time).
 *  - Preemption and quantum expiry are handled correctly per algorithm.
 *  - Metrics are deduplicated into compute_metrics() and reused.
 */
class SimulationEngine {
private:
    // ---- Simulation state ----
    int                        currentTime;
    std::priority_queue<Event> eventQueue;  // arrival events only
    CPU                        cpu;
    MemoryManager              memoryManager;
    IOSubsystem                ioSubsystem;
    Scheduler*                 scheduler;   // owned externally — not deleted here

    // ---- Results ----
    std::vector<GanttEntry>    ganttEntries;
    std::ofstream              logFile;
    std::string                resultsString; // populated by run_simulation()

    // ---- Metric accumulators ----
    int              totalWaitingTime;
    int              totalTurnaroundTime;
    int              totalResponseTime;
    int              totalProcessCount;   // processes that arrived
    int              completedCount;      // processes that finished
    int              cpuBusyTime;
    int              contextSwitchCount;
    std::vector<int> waitingTimes;        // per-process, for fairness calculation

    // ---- Private helpers ----
    void              log_event(const std::string& description);
    PerformanceMetrics compute_metrics() const;
    std::string        format_gantt_chart() const;
    std::string        format_metrics() const;

public:
    /**
     * @param sched  The scheduler to drive.  Ownership remains with the caller.
     */
    explicit SimulationEngine(Scheduler* sched);

    /**
     * @brief Register a process ARRIVAL at the specified simulation time.
     *
     * Call this for every process before invoking run_simulation().
     * Only "ARRIVAL" events are meaningful; other types are ignored.
     *
     * @param time    Simulation time at which the process arrives.
     * @param type    Event type string — use "ARRIVAL".
     * @param process Pointer to the Process object.
     */
    void add_event(int time, const std::string& type, Process* process);

    /**
     * @brief Execute the complete simulation and compute all results.
     *
     * After this call:
     *  - get_results()             returns the formatted output string.
     *  - save_simulation_results() can persist it to a file.
     */
    void run_simulation();

    /** @brief Print the Gantt chart to stdout. */
    void print_gantt_chart() const;

    /** @brief Print performance metrics to stdout. */
    void print_performance_metrics() const;

    /** @brief Save all results (Gantt + metrics) to @p filename. */
    void save_simulation_results(const std::string& filename) const;

    /**
     * @brief Returns the formatted results string populated by run_simulation().
     *        Returns an empty string if run_simulation() has not been called.
     */
    const std::string& get_results() const;
};

#endif // SIMULATION_ENGINE_H
