#include "file_io.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// ---- save_configuration ---------------------------------------------------

bool FileIO::save_configuration(const std::vector<Process>& processes,
                                const std::string& schedulingAlgorithm,
                                int timeQuantum,
                                const std::string& filename)
{
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error: Cannot open '" << filename
                  << "' for writing configuration.\n";
        return false;
    }

    out << "Scheduling Algorithm: " << schedulingAlgorithm << "\n"
        << "Time Quantum: "         << timeQuantum          << "\n"
        << "ProcessID,ArrivalTime,BurstTime,Priority\n";

    for (const Process& p : processes) {
        out << p.getProcessID()   << ","
            << p.getArrivalTime() << ","
            << p.getBurstTime()   << ","
            << p.getPriority()    << "\n";
    }

    std::cout << "Configuration saved to '" << filename << "'.\n";
    return true;
}

// ---- load_configuration ---------------------------------------------------

bool FileIO::load_configuration(std::vector<Process>& processes,
                                std::string& schedulingAlgorithm,
                                int& timeQuantum,
                                const std::string& filename)
{
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error: Cannot open '" << filename
                  << "' for reading configuration.\n";
        return false;
    }

    processes.clear();
    std::string line;

    // ---- Parse header: "Scheduling Algorithm: <name>" ----
    if (!std::getline(in, line)) {
        std::cerr << "Error: Configuration file '" << filename << "' is empty.\n";
        return false;
    }
    {
        const std::string prefix = "Scheduling Algorithm: ";
        auto pos = line.find(prefix);
        if (pos == std::string::npos) {
            std::cerr << "Error: Malformed configuration file (missing algorithm line).\n";
            return false;
        }
        schedulingAlgorithm = line.substr(pos + prefix.size());
    }

    // ---- Parse header: "Time Quantum: <n>" ----
    if (!std::getline(in, line)) {
        std::cerr << "Error: Configuration file is truncated (missing quantum line).\n";
        return false;
    }
    {
        const std::string prefix = "Time Quantum: ";
        auto pos = line.find(prefix);
        if (pos == std::string::npos) {
            std::cerr << "Error: Malformed configuration file (missing quantum line).\n";
            return false;
        }
        try {
            timeQuantum = std::stoi(line.substr(pos + prefix.size()));
        } catch (const std::exception& e) {
            std::cerr << "Error: Cannot parse time quantum from configuration file: "
                      << e.what() << "\n";
            return false;
        }
    }

    // ---- Skip column header line ----
    if (!std::getline(in, line)) {
        // File has no processes — not necessarily an error
        std::cout << "Configuration loaded from '" << filename
                  << "' (no processes).\n";
        return true;
    }

    // ---- Parse process records ----
    int lineNumber = 4; // 3 header lines already consumed
    while (std::getline(in, line)) {
        ++lineNumber;
        if (line.empty()) continue; // skip blank lines

        std::stringstream ss(line);
        std::string token;
        int pid = 0, arrival = 0, burst = 0, priority = 0;

        try {
            if (!std::getline(ss, token, ','))
                throw std::runtime_error("missing ProcessID");
            pid = std::stoi(token);

            if (!std::getline(ss, token, ','))
                throw std::runtime_error("missing ArrivalTime");
            arrival = std::stoi(token);

            if (!std::getline(ss, token, ','))
                throw std::runtime_error("missing BurstTime");
            burst = std::stoi(token);

            if (!std::getline(ss, token, ','))
                throw std::runtime_error("missing Priority");
            priority = std::stoi(token);

        } catch (const std::exception& e) {
            std::cerr << "Warning: Skipping malformed line " << lineNumber
                      << " in '" << filename << "': " << e.what() << "\n";
            continue;
        }

        // Basic sanity checks
        if (pid <= 0 || burst <= 0 || arrival < 0) {
            std::cerr << "Warning: Skipping line " << lineNumber
                      << " — invalid values (pid=" << pid
                      << ", burst=" << burst
                      << ", arrival=" << arrival << ").\n";
            continue;
        }

        processes.emplace_back(pid, arrival, burst, priority,
                               std::vector<std::string>{"CPU"});
    }

    std::cout << "Configuration loaded from '" << filename << "' ("
              << processes.size() << " process(es)).\n";
    return true;
}

// ---- save_results ---------------------------------------------------------

bool FileIO::save_results(const std::string& results,
                          const std::string& filename)
{
    if (results.empty()) {
        std::cerr << "Warning: No simulation results to save.\n";
        return false;
    }

    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error: Cannot open '" << filename
                  << "' for writing results.\n";
        return false;
    }

    out << results;
    std::cout << "Results saved to '" << filename << "'.\n";
    return true;
}
