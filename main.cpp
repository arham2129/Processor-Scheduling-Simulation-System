#include <iostream>
#include <vector>
#include <string>
#include "process.h"
#include "pcb.h"
#include "fcfs_scheduler.h"
#include "round_robin_scheduler.h"
#include "mlfq_scheduler.h"
#include "preemptive_sjf_scheduler.h"
#include "priority_scheduler.h"
#include "simulation_engine.h"
#include "file_io.h"

using namespace std;

Process create_process() {
    int processID, arrivalTime, burstTime, priority;
    vector<string> resources;

    cout << "Enter Process ID: ";
    cin >> processID;
    cout << "Enter Arrival Time: ";
    cin >> arrivalTime;
    cout << "Enter Burst Time: ";
    cin >> burstTime;
    cout << "Enter Priority (lower number indicates higher priority): ";
    cin >> priority;

    resources.push_back("CPU");

    return Process(processID, arrivalTime, burstTime, priority, resources);
}

int display_menu() {
    cout << "\n=== Process Scheduling Simulation System ===" << endl;
    cout << "1. Add a process" << endl;
    cout << "2. Choose scheduling algorithm" << endl;
    cout << "3. Set time quantum (for Round Robin or MLFQ)" << endl;
    cout << "4. Run simulation" << endl;
    cout << "5. Save configuration" << endl;
    cout << "6. Load configuration" << endl;
    cout << "7. Save simulation results" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    return choice;
}

Scheduler* choose_scheduler(int& quantum) {
    cout << "\nChoose Scheduling Algorithm:" << endl;
    cout << "1. First-Come, First-Served (FCFS)" << endl;
    cout << "2. Round Robin" << endl;
    cout << "3. Multilevel Feedback Queue (MLFQ)" << endl;
    cout << "4. Shortest Job First (SJF - Preemptive)" << endl;
    cout << "5. Priority Scheduling (Preemptive)" << endl;
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            return new FCFSScheduler();
        case 2:
            cout << "Enter time quantum for Round Robin: ";
            cin >> quantum;
            return new RoundRobinScheduler(quantum);
        case 3:
            cout << "Enter time quantum for MLFQ: ";
            cin >> quantum;
            vector<int> timeQuantums = {quantum, quantum * 2, quantum * 3}; 
            return new MLFQScheduler(timeQuantums);
        case 4:
            return new PreemptiveSJFScheduler();  
        case 5:
            return new PriorityScheduler(); 
        default:
            cout << "Invalid choice, defaulting to FCFS" << endl;
            return new FCFSScheduler();
    }
}

int main() {
    vector<Process> processes;  
    Scheduler* scheduler = nullptr;
    int timeQuantum = 0;
    bool schedulerChosen = false;
    bool simulationRun = false;
    string results;  

    while (true) {
        int choice = display_menu();

        if (choice == 1) {
            Process newProcess = create_process();
            processes.push_back(newProcess);
            cout << "Process added successfully." << endl;

        } else if (choice == 2) {
            scheduler = choose_scheduler(timeQuantum);
            if (scheduler) {
                cout << "Scheduler chosen successfully." << endl;
                schedulerChosen = true;
            }

        } else if (choice == 3) {
            if (schedulerChosen && (dynamic_cast<RoundRobinScheduler*>(scheduler) || dynamic_cast<MLFQScheduler*>(scheduler))) {
                cout << "Enter new time quantum: ";
                cin >> timeQuantum;
                if (auto rrScheduler = dynamic_cast<RoundRobinScheduler*>(scheduler)) {
                    rrScheduler->set_time_quantum(timeQuantum);
                } else if (auto mlfqScheduler = dynamic_cast<MLFQScheduler*>(scheduler)) {
                    vector<int> timeQuantums = {timeQuantum, timeQuantum * 2, timeQuantum * 3};
                    mlfqScheduler->update_time_quantums(timeQuantums);
                }
            } else {
                cout << "Please choose a Round Robin or MLFQ scheduler first." << endl;
            }

        } else if (choice == 4) {
            if (schedulerChosen && !processes.empty()) {
                SimulationEngine simulation(scheduler);

                for (auto& process : processes) {
                    simulation.add_event(process.getArrivalTime(), "ARRIVAL", &process);
                }

                simulation.add_event(0, "CPU", nullptr);
                simulation.add_event(2, "CPU", nullptr);
                simulation.add_event(4, "CPU", nullptr);

                simulation.run_simulation(100);

                simulationRun = true;

            } else {
                cout << "Please choose a scheduler and add processes first." << endl;
            }

        } else if (choice == 5) {
            if (!processes.empty()) {
                string filename;
                cout << "Enter filename to save configuration: ";
                cin >> filename;
                FileIO::save_configuration(processes, "Round Robin", timeQuantum, filename); 
            } else {
                cout << "No processes to save." << endl;
            }

        } else if (choice == 6) {
            string filename;
            cout << "Enter filename to load configuration: ";
            cin >> filename;
            FileIO::load_configuration(processes, results, timeQuantum, filename);
            schedulerChosen = true;

        } else if (choice == 7) {
            if (simulationRun) {
                string filename;
                cout << "Enter filename to save results: ";
                cin >> filename;
                FileIO::save_results(results, filename);
            } else {
                cout << "No simulation has been run yet." << endl;
            }

        } else if (choice == 8) {
            // Exit
            cout << "Exiting the simulation system." << endl;
            break;

        } else {
            cout << "Invalid choice, please try again." << endl;
        }
    }

    delete scheduler;

    return 0;
}
