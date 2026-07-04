#include "simulation_engine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <limits>
#include <stdexcept>

// ===========================================================================
// Construction
// ===========================================================================

SimulationEngine::SimulationEngine(Scheduler* sched)
    : currentTime(0),
      scheduler(sched),
      totalWaitingTime(0),
      totalTurnaroundTime(0),
      totalResponseTime(0),
      totalProcessCount(0),
      completedCount(0),
      cpuBusyTime(0),
      contextSwitchCount(0)
{
    logFile.open("simulation_log.txt");
    if (!logFile) {
        std::cerr << "Warning: Could not open 'simulation_log.txt' for writing.\n";
    }
}

// ===========================================================================
// Event registration
// ===========================================================================

void SimulationEngine::add_event(int time, const std::string& type, Process* process) {
    if (type == "ARRIVAL" && process != nullptr) {
        eventQueue.push({time, type, process});
    }
    // Other event types are intentionally ignored — CPU scheduling is driven
    // internally by the simulation loop (not by externally injected CPU events).
}

// ===========================================================================
// Private: logging
// ===========================================================================

void SimulationEngine::log_event(const std::string& description) {
    if (logFile.is_open()) {
        logFile << "[t=" << std::setw(4) << std::setfill('0') << currentTime
                << "] " << description << "\n";
    }
}

// ===========================================================================
// Core simulation loop
// ===========================================================================

void SimulationEngine::run_simulation() {
    if (scheduler == nullptr) {
        std::cerr << "Error: No scheduler assigned to SimulationEngine.\n";
        return;
    }

    // ---- Collect and chronologically sort all ARRIVAL events ----
    std::vector<std::pair<int, Process*>> arrivals;
    while (!eventQueue.empty()) {
        Event e = eventQueue.top();
        eventQueue.pop();
        arrivals.push_back({e.time, e.process});
    }
    std::sort(arrivals.begin(), arrivals.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });

    if (arrivals.empty()) {
        std::cout << "No processes registered.  Nothing to simulate.\n";
        return;
    }

    // ---- Simulation state ----
    std::size_t nextArrIdx    = 0;
    currentTime               = 0;
    Process*    running       = nullptr;  // process currently on the CPU
    int         quantumUsed   = 0;        // time units used in current slice
    int         currentQuantum= 0;        // quantum for the running process

    // ---- Lambda: drain all arrivals whose time <= currentTime ----
    auto processArrivals = [&]() {
        while (nextArrIdx < arrivals.size() &&
               arrivals[nextArrIdx].first <= currentTime)
        {
            Process* p = arrivals[nextArrIdx].second;
            p->update_state(ProcessState::READY);
            scheduler->add_process_to_queue(*p);
            memoryManager.allocate_memory(p->getProcessID(), 100);
            ++totalProcessCount;

            std::string msg = "Process P" + std::to_string(p->getProcessID())
                            + " arrived (burst=" + std::to_string(p->getBurstTime())
                            + ", priority=" + std::to_string(p->getPriority()) + ")";
            log_event(msg);
            std::cout << "[t=" << std::setw(3) << currentTime << "] " << msg << "\n";
            ++nextArrIdx;
        }
    };

    // ---- Main loop: runs until all processes have completed ----
    while (nextArrIdx < arrivals.size() ||
           !scheduler->is_empty()         ||
           running != nullptr)
    {
        // 1. Process any arrivals that have occurred at or before currentTime
        processArrivals();

        // 2. Start a new process if the CPU is idle
        if (running == nullptr) {
            running = scheduler->get_next_process();
            if (running != nullptr) {
                // Record response time on first CPU access
                if (running->getResponseTime() == -1) {
                    running->set_response_time(currentTime);
                }
                running->update_state(ProcessState::RUNNING);
                currentQuantum = scheduler->get_time_quantum();
                quantumUsed    = 0;

                log_event("P" + std::to_string(running->getProcessID())
                        + " scheduled (quantum="
                        + (currentQuantum > 0 ? std::to_string(currentQuantum) : "∞")
                        + ")");
            }
        }

        // 3. If CPU is still idle, jump forward to the next process arrival
        if (running == nullptr) {
            if (nextArrIdx < arrivals.size()) {
                currentTime = arrivals[nextArrIdx].first;
                continue;
            }
            break; // queue empty and no future arrivals — done
        }

        // ---- 4. Determine how long to run in this slice ----
        const int burstLeft   = running->getRemainingTime();
        const int quantumLeft = (currentQuantum > 0)
                                    ? (currentQuantum - quantumUsed)
                                    : burstLeft; // 0 quantum => run to completion

        // Time until the next unprocessed arrival
        int timeToNextArrival = std::numeric_limits<int>::max();
        if (nextArrIdx < arrivals.size()) {
            timeToNextArrival = arrivals[nextArrIdx].first - currentTime;
            if (timeToNextArrival <= 0) timeToNextArrival = 0;
        }

        int runFor;
        if (scheduler->is_preemptive() &&
            timeToNextArrival > 0 &&
            timeToNextArrival < std::min(quantumLeft, burstLeft))
        {
            // Preemptive: stop just before the next arrival to evaluate preemption
            runFor = timeToNextArrival;
        } else {
            runFor = std::min(quantumLeft, burstLeft);
        }
        if (runFor <= 0) runFor = 1; // safety guard

        // ---- 5. Execute the slice ----
        ganttEntries.push_back({running->getProcessID(), currentTime, currentTime + runFor});
        log_event("P" + std::to_string(running->getProcessID())
                + " executing [" + std::to_string(currentTime)
                + " -> " + std::to_string(currentTime + runFor) + "]");

        // Use the CPU object (cpu.run_process calls simulate_cpu_burst + updates clock)
        cpu.run_process(running, runFor);
        cpuBusyTime  += runFor;
        currentTime  += runFor;
        quantumUsed  += runFor;

        // ---- 6. Post-execution: decide what happens next ----

        if (running->getRemainingTime() == 0) {
            // ---- Process completed ----
            running->update_state(ProcessState::TERMINATED);
            running->set_completion_time(currentTime);
            memoryManager.deallocate_memory(running->getProcessID());

            // Compute per-process metrics (correctness fix: relative times)
            const int tat = running->getCompletionTime() - running->getArrivalTime();
            const int wt  = tat - running->getBurstTime();
            const int rt  = running->getResponseTime()  - running->getArrivalTime();

            totalTurnaroundTime += tat;
            totalWaitingTime    += wt;
            totalResponseTime   += rt;
            waitingTimes.push_back(wt);
            ++completedCount;
            ++contextSwitchCount;

            std::string msg = "Process P" + std::to_string(running->getProcessID())
                            + " completed  (TAT=" + std::to_string(tat)
                            + ", WT=" + std::to_string(wt)
                            + ", RT=" + std::to_string(rt) + ")";
            log_event(msg);
            std::cout << "[t=" << std::setw(3) << currentTime << "] " << msg << "\n";

            running       = nullptr;
            quantumUsed   = 0;
            currentQuantum = 0;
        }
        else if (currentQuantum > 0 && quantumUsed >= currentQuantum) {
            // ---- Time quantum expired ----
            log_event("P" + std::to_string(running->getProcessID())
                    + " quantum expired (remaining=" + std::to_string(running->getRemainingTime()) + ")");
            running->update_state(ProcessState::READY);

            // Process arrivals FIRST so that processes that arrived during this
            // running slice enter the queue BEFORE the expired process is
            // re-added to the back.  This ensures correct Round Robin ordering:
            // e.g. if P1 runs [0→4] and P2 arrives at t=1, P2 should be
            // scheduled before P1 gets its second slice.
            processArrivals();

            scheduler->on_quantum_expiry(*running); // MLFQ demotes; RR/others re-add to back
            ++contextSwitchCount;
            running       = nullptr;
            quantumUsed   = 0;
            currentQuantum = 0;
        }
        else if (scheduler->is_preemptive()) {
            // ---- Check arrival-triggered preemption ----
            // First, add any processes that arrived at currentTime (timeToNextArrival==0)
            processArrivals();

            if (!scheduler->is_empty() && scheduler->should_preempt(*running)) {
                log_event("P" + std::to_string(running->getProcessID())
                        + " preempted (remaining=" + std::to_string(running->getRemainingTime()) + ")");
                running->update_state(ProcessState::READY);
                scheduler->add_process_to_queue(*running);
                ++contextSwitchCount;
                running       = nullptr;
                quantumUsed   = 0;
                currentQuantum = 0;
            }
            // else: continue running same process in next iteration
        }
        // For non-preemptive, non-quantum schedulers: loop continues running
    }

    // ---- Build and display results ----
    resultsString  = format_gantt_chart();
    resultsString += format_metrics();

    std::cout << resultsString;

    if (logFile.is_open()) {
        logFile << "\n[SIMULATION COMPLETE] Duration=" << currentTime << " units\n";
        logFile.close();
    }
}

// ===========================================================================
// Private: metric computation (single source of truth)
// ===========================================================================

PerformanceMetrics SimulationEngine::compute_metrics() const {
    PerformanceMetrics m;
    m.totalProcesses     = completedCount;
    m.simulationDuration = currentTime;
    m.contextSwitches    = contextSwitchCount;

    if (completedCount > 0) {
        m.avgWaitingTime    = static_cast<double>(totalWaitingTime)    / completedCount;
        m.avgTurnaroundTime = static_cast<double>(totalTurnaroundTime) / completedCount;
        m.avgResponseTime   = static_cast<double>(totalResponseTime)   / completedCount;
    }

    m.cpuUtilization = (currentTime > 0)
        ? (static_cast<double>(cpuBusyTime) * 100.0 / currentTime) : 0.0;
    m.throughput     = (currentTime > 0)
        ? (static_cast<double>(completedCount) / currentTime) : 0.0;

    // Standard deviation of waiting times (fairness metric)
    if (waitingTimes.size() > 1) {
        double sum = 0.0;
        for (int wt : waitingTimes) sum += static_cast<double>(wt);
        double mean = sum / static_cast<double>(waitingTimes.size());
        double sqDiff = 0.0;
        for (int wt : waitingTimes) {
            double diff = static_cast<double>(wt) - mean;
            sqDiff += diff * diff;
        }
        m.fairnessStdDev = std::sqrt(sqDiff / static_cast<double>(waitingTimes.size()));
    }
    return m;
}

// ===========================================================================
// Private: formatting helpers
// ===========================================================================

std::string SimulationEngine::format_gantt_chart() const {
    if (ganttEntries.empty()) return "";

    // Merge adjacent entries for the same process into single bars
    std::vector<GanttEntry> merged;
    for (const auto& entry : ganttEntries) {
        if (!merged.empty() &&
            merged.back().processID == entry.processID &&
            merged.back().endTime   == entry.startTime)
        {
            merged.back().endTime = entry.endTime;
        } else {
            merged.push_back(entry);
        }
    }

    std::ostringstream oss;
    oss << "\n";
    oss << "╔══════════════════════════════════════════════════════╗\n";
    oss << "║                     GANTT CHART                     ║\n";
    oss << "╚══════════════════════════════════════════════════════╝\n";

    // Calculate column widths (at least 5 chars wide)
    std::vector<int> widths;
    for (const auto& e : merged) {
        int duration = e.endTime - e.startTime;
        int labelLen = static_cast<int>(std::to_string(e.processID).size()) + 2; // "Px"
        widths.push_back(std::max({duration + 2, labelLen + 2, 5}));
    }

    // ---- Top border ----
    oss << "+";
    for (int w : widths) {
        oss << std::string(static_cast<std::size_t>(w), '-') << "+";
    }
    oss << "\n";

    // ---- Process labels row ----
    oss << "|";
    for (std::size_t i = 0; i < merged.size(); ++i) {
        std::string label = "P" + std::to_string(merged[i].processID);
        int w = widths[i];
        int pad = w - static_cast<int>(label.size());
        int padL = pad / 2;
        int padR = pad - padL;
        oss << std::string(static_cast<std::size_t>(padL), ' ')
            << label
            << std::string(static_cast<std::size_t>(padR), ' ')
            << "|";
    }
    oss << "\n";

    // ---- Bottom border ----
    oss << "+";
    for (int w : widths) {
        oss << std::string(static_cast<std::size_t>(w), '-') << "+";
    }
    oss << "\n";

    // ---- Timeline row ----
    // Print start time of first segment
    oss << std::setw(1) << merged.front().startTime;
    for (std::size_t i = 0; i < merged.size(); ++i) {
        std::string endStr = std::to_string(merged[i].endTime);
        int w = widths[i];
        // Right-align the end time within its column width + 1 (for the "|")
        int pad = w + 1 - static_cast<int>(endStr.size());
        oss << std::string(static_cast<std::size_t>(std::max(pad, 0)), ' ') << endStr;
    }
    oss << "\n\n";

    return oss.str();
}

std::string SimulationEngine::format_metrics() const {
    PerformanceMetrics m = compute_metrics();
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    oss << "╔══════════════════════════════════════════════════════╗\n";
    oss << "║                 PERFORMANCE METRICS                 ║\n";
    oss << "╠══════════════════════════════════════════════════════╣\n";
    oss << "║  Total Processes Completed : "
        << std::setw(5) << m.totalProcesses
        << "                         ║\n";
    oss << "║  Simulation Duration       : "
        << std::setw(5) << m.simulationDuration << " time units            ║\n";
    oss << "╠══════════════════════════════════════════════════════╣\n";
    oss << "║  Avg Waiting Time          : "
        << std::setw(8) << m.avgWaitingTime << " units               ║\n";
    oss << "║  Avg Turnaround Time       : "
        << std::setw(8) << m.avgTurnaroundTime << " units               ║\n";
    oss << "║  Avg Response Time         : "
        << std::setw(8) << m.avgResponseTime << " units               ║\n";
    oss << "╠══════════════════════════════════════════════════════╣\n";
    oss << "║  CPU Utilization           : "
        << std::setw(7) << m.cpuUtilization << "%                    ║\n";
    oss << "║  Throughput                : "
        << std::setw(8) << m.throughput << " proc/unit           ║\n";
    oss << "║  Fairness (Waiting StdDev) : "
        << std::setw(8) << m.fairnessStdDev << " units               ║\n";
    oss << "║  Context Switches          : "
        << std::setw(5) << m.contextSwitches
        << "                         ║\n";
    oss << "╚══════════════════════════════════════════════════════╝\n\n";

    return oss.str();
}

// ===========================================================================
// Public display/save methods
// ===========================================================================

void SimulationEngine::print_gantt_chart() const {
    std::cout << format_gantt_chart();
}

void SimulationEngine::print_performance_metrics() const {
    std::cout << format_metrics();
}

void SimulationEngine::save_simulation_results(const std::string& filename) const {
    if (resultsString.empty()) {
        std::cerr << "Warning: No simulation results to save.  "
                     "Run run_simulation() first.\n";
        return;
    }
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error: Cannot open '" << filename << "' for writing.\n";
        return;
    }
    out << resultsString;
    std::cout << "Results saved to '" << filename << "'.\n";
}

const std::string& SimulationEngine::get_results() const {
    return resultsString;
}
