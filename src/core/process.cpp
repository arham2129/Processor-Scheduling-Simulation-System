#include "process.h"

// ---- Constructor ----------------------------------------------------------

Process::Process(int pid, int arrival, int burst, int prio,
                 const std::vector<std::string>& resources)
    : processID(pid),
      arrivalTime(arrival),
      burstTime(burst),
      remainingTime(burst),
      priority(prio),
      state(ProcessState::NEW),
      resourceRequirements(resources),
      startTime(-1),
      responseTime(-1),
      completionTime(-1)
{}

// ---- Accessors ------------------------------------------------------------

int Process::getProcessID()      const { return processID;     }
int Process::getArrivalTime()    const { return arrivalTime;   }
int Process::getBurstTime()      const { return burstTime;     }
int Process::getRemainingTime()  const { return remainingTime; }
int Process::getPriority()       const { return priority;      }
int Process::getResponseTime()   const { return responseTime;  }
int Process::getCompletionTime() const { return completionTime; }
ProcessState Process::getState() const { return state;         }

// ---- Mutators -------------------------------------------------------------

void Process::simulate_cpu_burst(int timeSlice) {
    if (remainingTime > 0) {
        remainingTime -= timeSlice;
        if (remainingTime < 0) {
            remainingTime = 0; // clamp: never go below zero
        }
    }
}

void Process::update_state(ProcessState newState) {
    state = newState;
}

void Process::set_start_time(int time) {
    startTime = time;
}

void Process::set_response_time(int time) {
    // Only record the FIRST time the process receives the CPU
    if (responseTime == -1) {
        responseTime = time;
    }
}

void Process::set_completion_time(int time) {
    completionTime = time;
}

// ---- Queries --------------------------------------------------------------

bool Process::is_completed() const {
    return remainingTime == 0;
}
