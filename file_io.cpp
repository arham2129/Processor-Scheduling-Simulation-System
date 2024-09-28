#include "file_io.h"
#include <fstream>
#include <iostream>
#include <sstream>

void FileIO::save_configuration(const std::vector<Process>& processes, const std::string& schedulingAlgorithm, int timeQuantum, const std::string& filename) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << "Scheduling Algorithm: " << schedulingAlgorithm << std::endl;
        outFile << "Time Quantum: " << timeQuantum << std::endl;
        outFile << "ProcessID,ArrivalTime,BurstTime,Priority" << std::endl;
        for (const Process& process : processes) {
            outFile << process.getProcessID() << "," << process.getArrivalTime() << "," << process.getBurstTime() << "," << process.getPriority() << std::endl;
        }
        outFile.close();
        std::cout << "Configuration saved to " << filename << std::endl;
    } else {
        std::cerr << "Error opening file for saving configuration." << std::endl;
    }
}

void FileIO::load_configuration(std::vector<Process>& processes, std::string& schedulingAlgorithm, int& timeQuantum, const std::string& filename) {
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        processes.clear();
        std::string line;

        std::getline(inFile, line);
        schedulingAlgorithm = line.substr(line.find(": ") + 2);
        std::getline(inFile, line);
        timeQuantum = std::stoi(line.substr(line.find(": ") + 2));

        std::getline(inFile, line);

        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string token;
            int processID, arrivalTime, burstTime, priority;
            std::getline(ss, token, ',');
            processID = std::stoi(token);
            std::getline(ss, token, ',');
            arrivalTime = std::stoi(token);
            std::getline(ss, token, ',');
            burstTime = std::stoi(token);
            std::getline(ss, token, ',');
            priority = std::stoi(token);

            processes.emplace_back(processID, arrivalTime, burstTime, priority, std::vector<std::string>{});
        }
        inFile.close();
        std::cout << "Configuration loaded from " << filename << std::endl;
    } else {
        std::cerr << "Error opening file for loading configuration." << std::endl;
    }
}

void FileIO::save_results(const std::string& results, const std::string& filename) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << results;
        outFile.close();
        std::cout << "Results saved to " << filename << std::endl;
    } else {
        std::cerr << "Error opening file for saving results." << std::endl;
    }
}
