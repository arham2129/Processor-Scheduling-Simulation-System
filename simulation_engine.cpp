#include "simulation_engine.h"
#include <iostream>
#include <fstream>
#include <cmath>  

SimulationEngine::SimulationEngine(Scheduler* sched)
    : currentTime(0), scheduler(sched), totalWaitingTime(0), totalTurnaroundTime(0), 
      totalResponseTime(0), totalProcessCount(0), cpuBusyTime(0), contextSwitchCount(0) {
    logFile.open("simulation_log.txt");  
    if (!logFile) {
        std::cerr << "Error: Could not open log file for writing." << std::endl;
    }
}

void SimulationEngine::add_event(int time, const std::string& type, Process* process) {
    Event newEvent = { time, type, process };
    eventQueue.push(newEvent);
}

void SimulationEngine::run_simulation(int endTime) {
    while (!eventQueue.empty() && currentTime <= endTime) {
        Event event = eventQueue.top();
        eventQueue.pop();

        currentTime = event.time;

        handle_event(event);
    }

    print_performance_metrics();

    logFile.close();

    std::cout << "\n=== Gantt Chart ===" << std::endl;
    std::cout << ganttChart << std::endl;
}

void SimulationEngine::handle_event(const Event& event) {
    if (event.type == "ARRIVAL") {
        std::string logMessage = "Process " + std::to_string(event.process->getProcessID()) + 
                                 " arrived at time " + std::to_string(event.time);
        log_event(logMessage);
        std::cout << logMessage << std::endl;

        memoryManager.allocate_memory(event.process->getProcessID(), 100);  
        scheduler->add_process_to_queue(*event.process);
        totalProcessCount++;
    } else if (event.type == "CPU") {
        Process* nextProcess = scheduler->get_next_process();
        if (nextProcess) {
            int burstTime = 4;  
            cpu.run_process(nextProcess, burstTime);
            cpuBusyTime += burstTime;
            contextSwitchCount++;  

            generate_gantt_chart(nextProcess, burstTime);

            if (nextProcess->getRemainingTime() == 0) {
                memoryManager.deallocate_memory(nextProcess->getProcessID());
                std::string logMessage = "Process " + std::to_string(nextProcess->getProcessID()) + 
                                         " finished at time " + std::to_string(currentTime);
                log_event(logMessage);
                std::cout << logMessage << std::endl;

                calculate_metrics(nextProcess);
            } else {
                scheduler->add_process_to_queue(*nextProcess);
            }
        }
    } else if (event.type == "IO_COMPLETE") {
        ioSubsystem.complete_io();
    }
}

void SimulationEngine::generate_gantt_chart(Process* process, int burstTime) {
    ganttChart += "P" + std::to_string(process->getProcessID()) + " | ";
}

void SimulationEngine::log_event(const std::string& eventDescription) {
    logFile << "[Time " << currentTime << "]: " << eventDescription << std::endl;
}

void SimulationEngine::calculate_metrics(Process* process) {
    int turnaroundTime = currentTime - process->getArrivalTime();
    int waitingTime = turnaroundTime - process->getBurstTime();
    int responseTime = process->getResponseTime();  

    totalTurnaroundTime += turnaroundTime;
    totalWaitingTime += waitingTime;
    totalResponseTime += responseTime;

    waitingTimes.push_back(waitingTime); 
}

void SimulationEngine::print_performance_metrics() {
    double averageWaitingTime = (totalProcessCount > 0) ? (double)totalWaitingTime / totalProcessCount : 0;
    double averageTurnaroundTime = (totalProcessCount > 0) ? (double)totalTurnaroundTime / totalProcessCount : 0;
    double averageResponseTime = (totalProcessCount > 0) ? (double)totalResponseTime / totalProcessCount : 0;
    double cpuUtilization = (currentTime > 0) ? (double)cpuBusyTime * 100.0 / currentTime : 0;
    double throughput = (totalProcessCount > 0) ? (double)totalProcessCount / currentTime : 0;

    double sumOfSquaredDiffs = 0;
    double mean = averageWaitingTime;
    for (int wt : waitingTimes) {
        sumOfSquaredDiffs += (wt - mean) * (wt - mean);
    }
    double fairness = (waitingTimes.size() > 1) ? sqrt(sumOfSquaredDiffs / waitingTimes.size()) : 0;

    std::cout << "\n=== Performance Metrics ===" << std::endl;
    std::cout << "Total processes: " << totalProcessCount << std::endl;
    std::cout << "Average Waiting Time: " << averageWaitingTime << " units" << std::endl;
    std::cout << "Average Turnaround Time: " << averageTurnaroundTime << " units" << std::endl;
    std::cout << "Average Response Time: " << averageResponseTime << " units" << std::endl;
    std::cout << "CPU Utilization: " << cpuUtilization << "%" << std::endl;
    std::cout << "Throughput: " << throughput << " processes per unit time" << std::endl;
    std::cout << "Fairness (Variance in Waiting Time): " << fairness << std::endl;
    std::cout << "Context Switch Count: " << contextSwitchCount << std::endl;
}

void SimulationEngine::save_simulation_results(const std::string& filename) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << "=== Gantt Chart ===\n" << ganttChart << "\n\n";

        outFile << "=== Performance Metrics ===\n";
        outFile << "Total processes: " << totalProcessCount << "\n";
        outFile << "Average Waiting Time: " << (totalProcessCount > 0 ? (double)totalWaitingTime / totalProcessCount : 0) << " units\n";
        outFile << "Average Turnaround Time: " << (totalProcessCount > 0 ? (double)totalTurnaroundTime / totalProcessCount : 0) << " units\n";
        outFile << "Average Response Time: " << (totalProcessCount > 0 ? (double)totalResponseTime / totalProcessCount : 0) << " units\n";
        outFile << "CPU Utilization: " << (currentTime > 0 ? (double)cpuBusyTime * 100.0 / currentTime : 0) << "%\n";
        outFile << "Throughput: " << (totalProcessCount > 0 ? (double)totalProcessCount / currentTime : 0) << " processes per unit time\n";

        double sumOfSquaredDiffs = 0;
        double mean = (totalProcessCount > 0) ? (double)totalWaitingTime / totalProcessCount : 0;
        for (int wt : waitingTimes) {
            sumOfSquaredDiffs += (wt - mean) * (wt - mean);
        }
        double fairness = (waitingTimes.size() > 1) ? sqrt(sumOfSquaredDiffs / waitingTimes.size()) : 0;

        outFile << "Fairness (Variance in Waiting Time): " << fairness << "\n";
        outFile << "Context Switch Count: " << contextSwitchCount << "\n";
        outFile.close();
        std::cout << "Simulation results saved to " << filename << std::endl;
    } else {
        std::cerr << "Error: Could not open file for saving simulation results." << std::endl;
    }
}
