/**
 * @file main.cpp
 * @brief Processor Scheduling Simulation System — CLI entry point.
 *
 * Provides an interactive menu-driven interface for:
 *  - Adding processes with user-specified attributes
 *  - Choosing from 7 scheduling algorithms
 *  - Configuring time quantums
 *  - Running the simulation and viewing results
 *  - Saving / loading configurations
 *  - Saving simulation results to file
 *
 * All std::cin failures are handled gracefully (no infinite-loop risk).
 * Duplicate process IDs and invalid values are rejected with clear messages.
 * The scheduler pointer is properly managed to prevent memory leaks.
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <memory>

#include "process.h"
#include "fcfs_scheduler.h"
#include "sjf_scheduler.h"
#include "round_robin_scheduler.h"
#include "mlfq_scheduler.h"
#include "preemptive_sjf_scheduler.h"
#include "priority_scheduler.h"
#include "multilevel_queue_scheduler.h"
#include "simulation_engine.h"
#include "file_io.h"

// ===========================================================================
// Input helpers
// ===========================================================================

/**
 * @brief Clear a failed std::cin state and discard the offending input.
 */
static void clear_cin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Read a single integer from stdin with a prompt.
 *
 * Loops until a valid integer is entered.
 * Accepts an optional valid range [minVal, maxVal]; pass INT_MIN/INT_MAX to
 * disable range checking.
 *
 * @param prompt  Text shown to the user before reading.
 * @param minVal  Minimum acceptable value (inclusive).
 * @param maxVal  Maximum acceptable value (inclusive).
 * @return        A validated integer in [minVal, maxVal].
 */
static int read_int(const std::string& prompt,
                    int minVal = std::numeric_limits<int>::min(),
                    int maxVal = std::numeric_limits<int>::max())
{
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value >= minVal && value <= maxVal) {
                clear_cin();
                return value;
            }
            std::cout << "  Value must be between " << minVal
                      << " and " << maxVal << ".  Please try again.\n";
        } else {
            clear_cin();
            std::cout << "  Invalid input — please enter an integer.\n";
        }
    }
}

// ===========================================================================
// Process creation
// ===========================================================================

/**
 * @brief Prompt the user for process attributes and construct a Process.
 *
 * @param existing  Existing process list used to detect duplicate IDs.
 * @return          A fully constructed Process object.
 */
static Process create_process(const std::vector<Process>& existing) {
    std::cout << "\n  --- Add Process ---\n";

    // Enforce unique process IDs
    int pid;
    while (true) {
        pid = read_int("  Process ID (> 0): ", 1, 9999);
        bool duplicate = false;
        for (const auto& p : existing) {
            if (p.getProcessID() == pid) {
                std::cout << "  Process ID " << pid
                          << " already exists.  Choose a different ID.\n";
                duplicate = true;
                break;
            }
        }
        if (!duplicate) break;
    }

    const int arrival  = read_int("  Arrival Time (>= 0): ", 0, 99999);
    const int burst    = read_int("  Burst Time (>= 1): ",   1, 99999);
    const int priority = read_int("  Priority (0 = highest; lower number = higher priority): ",
                                  0, 9999);

    return Process(pid, arrival, burst, priority, {"CPU"});
}

// ===========================================================================
// Scheduler construction
// ===========================================================================

/**
 * @brief Display the algorithm menu and construct the selected scheduler.
 *
 * @param algorithmName  Output: human-readable name of the chosen algorithm.
 * @param timeQuantum    Output: time quantum (0 if not applicable).
 * @return               Heap-allocated Scheduler*.  Caller owns it.
 */
static Scheduler* choose_scheduler(std::string& algorithmName, int& timeQuantum) {
    std::cout << "\n  +-------------------------------------------------+\n";
    std::cout << "  |           Choose Scheduling Algorithm           |\n";
    std::cout << "  +-------------------------------------------------+\n";
    std::cout << "  |  1. First-Come, First-Served (FCFS)             |\n";
    std::cout << "  |  2. Round Robin (RR)                            |\n";
    std::cout << "  |  3. Multilevel Feedback Queue (MLFQ)            |\n";
    std::cout << "  |  4. Preemptive Shortest Job First (SRTF)        |\n";
    std::cout << "  |  5. Preemptive Priority Scheduling              |\n";
    std::cout << "  |  6. Non-Preemptive SJF                          |\n";
    std::cout << "  |  7. Multilevel Queue (System / User)            |\n";
    std::cout << "  +-------------------------------------------------+\n";

    const int choice = read_int("  Enter choice (1-7): ", 1, 7);

    switch (choice) {
        case 1:
            algorithmName = "FCFS";
            timeQuantum   = 0;
            return new FCFSScheduler();

        case 2:
            timeQuantum = read_int("  Enter time quantum for Round Robin (>= 1): ", 1, 99999);
            algorithmName = "Round Robin";
            return new RoundRobinScheduler(timeQuantum);

        case 3: {
            timeQuantum = read_int("  Enter base time quantum for MLFQ (>= 1): ", 1, 99999);
            std::vector<int> quantums = {timeQuantum, timeQuantum * 2, timeQuantum * 4};
            std::cout << "  MLFQ levels: Q0=" << quantums[0]
                      << "  Q1=" << quantums[1]
                      << "  Q2=" << quantums[2] << "\n";
            algorithmName = "MLFQ";
            return new MLFQScheduler(quantums);
        }

        case 4:
            algorithmName = "Preemptive SJF (SRTF)";
            timeQuantum   = 0;
            return new PreemptiveSJFScheduler();

        case 5:
            algorithmName = "Preemptive Priority";
            timeQuantum   = 0;
            return new PriorityScheduler();

        case 6:
            algorithmName = "SJF (Non-Preemptive)";
            timeQuantum   = 0;
            return new SJFScheduler();

        case 7: {
            timeQuantum = read_int("  Enter time quantum for user-level Round Robin (>= 1): ", 1, 99999);
            algorithmName = "Multilevel Queue";
            return new MultilevelQueueScheduler(timeQuantum);
        }

        default:
            // Unreachable due to read_int bounds, but keeps compiler happy
            algorithmName = "FCFS";
            timeQuantum   = 0;
            return new FCFSScheduler();
    }
}

// ===========================================================================
// Menu display
// ===========================================================================

static void print_separator() {
    std::cout << "  -----------------------------------------------------\n";
}

static int display_menu(const std::string& algorithm,
                        std::size_t        processCount,
                        bool               simulationRun)
{
    std::cout << "\n";
    std::cout << "  +=====================================================+\n";
    std::cout << "  |       PROCESSOR SCHEDULING SIMULATION SYSTEM        |\n";
    std::cout << "  +=====================================================+\n";
    std::cout << "  |  Algorithm : "
              << std::left << std::setw(38) << algorithm << " |\n";
    std::cout << "  |  Processes : "
              << std::left << std::setw(38) << processCount << " |\n";
    std::cout << "  |  Last run  : "
              << std::left << std::setw(38) << (simulationRun ? "Yes" : "No") << " |\n";
    std::cout << "  +-----------------------------------------------------+\n";
    std::cout << "  |  1. Add a process                                   |\n";
    std::cout << "  |  2. Choose scheduling algorithm                     |\n";
    std::cout << "  |  3. Update time quantum (RR / MLFQ only)            |\n";
    std::cout << "  |  4. Run simulation                                  |\n";
    std::cout << "  |  5. Save configuration to file                      |\n";
    std::cout << "  |  6. Load configuration from file                    |\n";
    std::cout << "  |  7. Save last simulation results to file            |\n";
    std::cout << "  |  8. Display current process list                    |\n";
    std::cout << "  |  9. Clear all processes                             |\n";
    std::cout << "  |  0. Exit                                            |\n";
    std::cout << "  +=====================================================+\n";

    return read_int("  Enter choice (0-9): ", 0, 9);
}

// ===========================================================================
// Display process table
// ===========================================================================

static void display_processes(const std::vector<Process>& processes) {
    if (processes.empty()) {
        std::cout << "  No processes have been added yet.\n";
        return;
    }
    print_separator();
    std::cout << "  " << std::left
              << std::setw(8)  << "PID"
              << std::setw(14) << "Arrival Time"
              << std::setw(12) << "Burst Time"
              << std::setw(10) << "Priority"
              << "\n";
    print_separator();
    for (const auto& p : processes) {
        std::cout << "  " << std::left
                  << std::setw(8)  << p.getProcessID()
                  << std::setw(14) << p.getArrivalTime()
                  << std::setw(12) << p.getBurstTime()
                  << std::setw(10) << p.getPriority()
                  << "\n";
    }
    print_separator();
}

// ===========================================================================
// Scheduler reconstruction from loaded algorithm name
// ===========================================================================

/**
 * @brief Reconstruct a Scheduler from the algorithm name stored in a config file.
 *
 * Called after FileIO::load_configuration() to restore the correct scheduler.
 */
static Scheduler* reconstruct_scheduler(const std::string& algorithmName,
                                         int timeQuantum)
{
    if (algorithmName == "FCFS") {
        return new FCFSScheduler();
    }
    if (algorithmName == "Round Robin") {
        return new RoundRobinScheduler(timeQuantum > 0 ? timeQuantum : 4);
    }
    if (algorithmName == "MLFQ") {
        int q = timeQuantum > 0 ? timeQuantum : 4;
        return new MLFQScheduler({q, q * 2, q * 4});
    }
    if (algorithmName == "Preemptive SJF (SRTF)") {
        return new PreemptiveSJFScheduler();
    }
    if (algorithmName == "Preemptive Priority") {
        return new PriorityScheduler();
    }
    if (algorithmName == "SJF (Non-Preemptive)") {
        return new SJFScheduler();
    }
    if (algorithmName == "Multilevel Queue") {
        return new MultilevelQueueScheduler(timeQuantum > 0 ? timeQuantum : 4);
    }

    std::cerr << "Warning: Unknown algorithm '" << algorithmName
              << "' in configuration file — defaulting to FCFS.\n";
    return new FCFSScheduler();
}

// ===========================================================================
// Read filename from stdin
// ===========================================================================

static std::string read_filename(const std::string& prompt) {
    std::cout << prompt;
    std::string filename;
    std::cin >> filename;
    clear_cin();
    return filename;
}

// ===========================================================================
// main
// ===========================================================================

int main() {
    // ---- Application state ----
    std::vector<Process> processes;
    Scheduler*           scheduler       = nullptr;
    std::string          algorithmName   = "None";
    int                  timeQuantum     = 4;
    bool                 schedulerChosen = false;
    bool                 simulationRun   = false;
    std::string          lastResults;    // populated after each run

    std::cout << "\n  Welcome to the Processor Scheduling Simulation System!\n\n";

    while (true) {
        const int choice = display_menu(algorithmName, processes.size(), simulationRun);

        // ---- 1: Add a process ----
        if (choice == 1) {
            processes.push_back(create_process(processes));
            std::cout << "  Process P" << processes.back().getProcessID()
                      << " added successfully.\n";

        // ---- 2: Choose scheduler ----
        } else if (choice == 2) {
            // Free previous scheduler to prevent memory leak
            delete scheduler;
            scheduler = nullptr;

            scheduler = choose_scheduler(algorithmName, timeQuantum);
            if (scheduler) {
                schedulerChosen = true;
                simulationRun   = false; // results from old scheduler are stale
                std::cout << "  Scheduler set to: " << algorithmName << "\n";
            }

        // ---- 3: Update time quantum ----
        } else if (choice == 3) {
            if (!schedulerChosen) {
                std::cout << "  Please choose a scheduler first (option 2).\n";
            } else if (auto* rr = dynamic_cast<RoundRobinScheduler*>(scheduler)) {
                timeQuantum = read_int("  New time quantum (>= 1): ", 1, 99999);
                rr->set_time_quantum(timeQuantum);
                std::cout << "  Round Robin time quantum updated to " << timeQuantum << ".\n";
            } else if (auto* mlfq = dynamic_cast<MLFQScheduler*>(scheduler)) {
                timeQuantum = read_int("  New base time quantum (>= 1): ", 1, 99999);
                mlfq->update_time_quantums({timeQuantum, timeQuantum * 2, timeQuantum * 4});
                std::cout << "  MLFQ time quantums updated: Q0=" << timeQuantum
                          << "  Q1=" << timeQuantum * 2
                          << "  Q2=" << timeQuantum * 4 << "\n";
            } else {
                std::cout << "  Time quantum is only applicable to Round Robin and MLFQ.\n"
                          << "  Current algorithm (" << algorithmName
                          << ") does not use a time quantum.\n";
            }

        // ---- 4: Run simulation ----
        } else if (choice == 4) {
            if (!schedulerChosen) {
                std::cout << "  Please choose a scheduler first (option 2).\n";
            } else if (processes.empty()) {
                std::cout << "  Please add at least one process first (option 1).\n";
            } else {
                // Build a fresh scheduler to reset internal queue state
                // (re-using the same scheduler after a run would retain old queue state)
                delete scheduler;
                scheduler = nullptr;
                scheduler = reconstruct_scheduler(algorithmName, timeQuantum);

                // Create fresh copies of processes so original data is preserved
                // across multiple runs (remainingTime resets to burstTime)
                std::vector<Process> runCopies = processes;

                SimulationEngine engine(scheduler);
                for (auto& p : runCopies) {
                    engine.add_event(p.getArrivalTime(), "ARRIVAL", &p);
                }

                std::cout << "\n  Running simulation with " << algorithmName
                          << (timeQuantum > 0 ? " (quantum=" + std::to_string(timeQuantum) + ")" : "")
                          << " for " << processes.size() << " process(es)...\n\n";

                engine.run_simulation();

                lastResults   = engine.get_results();
                simulationRun = true;
            }

        // ---- 5: Save configuration ----
        } else if (choice == 5) {
            if (processes.empty()) {
                std::cout << "  No processes to save.\n";
            } else {
                std::string filename = read_filename("  Enter filename to save configuration: ");
                FileIO::save_configuration(processes, algorithmName, timeQuantum, filename);
            }

        // ---- 6: Load configuration ----
        } else if (choice == 6) {
            std::string filename = read_filename("  Enter filename to load configuration: ");
            std::string loadedAlgorithm;
            int loadedQuantum = 0;

            if (FileIO::load_configuration(processes, loadedAlgorithm, loadedQuantum, filename)) {
                algorithmName = loadedAlgorithm;
                timeQuantum   = loadedQuantum;

                // Reconstruct the scheduler from the loaded algorithm name
                delete scheduler;
                scheduler       = reconstruct_scheduler(algorithmName, timeQuantum);
                schedulerChosen = true;
                simulationRun   = false;
                lastResults.clear();

                std::cout << "  Algorithm restored: " << algorithmName << "\n";
                if (timeQuantum > 0) {
                    std::cout << "  Time quantum restored: " << timeQuantum << "\n";
                }
                display_processes(processes);
            }

        // ---- 7: Save last results ----
        } else if (choice == 7) {
            if (!simulationRun || lastResults.empty()) {
                std::cout << "  No simulation results to save.  "
                          << "Run the simulation first (option 4).\n";
            } else {
                std::string filename = read_filename("  Enter filename to save results: ");
                FileIO::save_results(lastResults, filename);
            }

        // ---- 8: Display process list ----
        } else if (choice == 8) {
            display_processes(processes);

        // ---- 9: Clear all processes ----
        } else if (choice == 9) {
            if (processes.empty()) {
                std::cout << "  Process list is already empty.\n";
            } else {
                std::cout << "  Are you sure you want to remove all "
                          << processes.size() << " process(es)? (y/N): ";
                char confirm;
                std::cin >> confirm;
                clear_cin();
                if (confirm == 'y' || confirm == 'Y') {
                    processes.clear();
                    simulationRun = false;
                    lastResults.clear();
                    std::cout << "  Process list cleared.\n";
                } else {
                    std::cout << "  Cancelled.\n";
                }
            }

        // ---- 0: Exit ----
        } else if (choice == 0) {
            std::cout << "\n  Thank you for using the Scheduling Simulation System.\n\n";
            break;
        }
    }

    // Clean up owned scheduler
    delete scheduler;

    return 0;
}
