#ifndef FILE_IO_H
#define FILE_IO_H

#include <vector>
#include <string>
#include "process.h"

/**
 * @brief Static utility class for persisting and restoring simulation data.
 *
 * Supports three operations:
 *  - save_configuration: writes the process list and scheduler settings to a
 *    plain-text file that can be reloaded later.
 *  - load_configuration: reads a previously-saved configuration file and
 *    reconstructs the process list plus scheduling parameters.
 *  - save_results: writes the formatted simulation results (Gantt chart +
 *    performance metrics) to a plain-text file.
 *
 * All methods report errors to std::cerr and return a bool indicating success
 * so callers can react without catching exceptions.
 */
class FileIO {
public:
    /**
     * @brief Save the current process list and scheduler configuration.
     *
     * @param processes          The list of processes to persist.
     * @param schedulingAlgorithm Name of the chosen algorithm (e.g. "FCFS").
     * @param timeQuantum        The current time quantum (0 if not applicable).
     * @param filename           Path of the output file.
     * @return true on success, false if the file could not be opened.
     */
    static bool save_configuration(const std::vector<Process>& processes,
                                   const std::string& schedulingAlgorithm,
                                   int timeQuantum,
                                   const std::string& filename);

    /**
     * @brief Load a previously-saved process configuration.
     *
     * On success @p processes is cleared and repopulated.
     * @p schedulingAlgorithm and @p timeQuantum are updated from the file.
     *
     * @param processes           Output vector to populate.
     * @param schedulingAlgorithm Output: algorithm name read from the file.
     * @param timeQuantum         Output: time quantum read from the file.
     * @param filename            Path of the input file.
     * @return true on success, false if the file could not be opened or parsed.
     */
    static bool load_configuration(std::vector<Process>& processes,
                                   std::string& schedulingAlgorithm,
                                   int& timeQuantum,
                                   const std::string& filename);

    /**
     * @brief Save a formatted results string (Gantt chart + metrics) to a file.
     *
     * @param results  The formatted string returned by SimulationEngine::get_results().
     * @param filename Path of the output file.
     * @return true on success, false if the file could not be opened.
     */
    static bool save_results(const std::string& results,
                             const std::string& filename);
};

#endif // FILE_IO_H
