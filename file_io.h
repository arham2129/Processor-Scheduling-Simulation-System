#ifndef FILE_IO_H
#define FILE_IO_H

#include <vector>
#include <string>
#include "process.h"

class FileIO {
public:
    static void save_configuration(const std::vector<Process>& processes, const std::string& schedulingAlgorithm, int timeQuantum, const std::string& filename);

    static void load_configuration(std::vector<Process>& processes, std::string& schedulingAlgorithm, int& timeQuantum, const std::string& filename);

    static void save_results(const std::string& results, const std::string& filename);
};

#endif 
