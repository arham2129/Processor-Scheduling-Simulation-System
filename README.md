# Processor Scheduling Simulation System

This project implements a processor scheduling simulation system that models the behavior of different scheduling algorithms in an operating system environment. The system manages processes, CPU scheduling, memory, and I/O operations while collecting performance metrics such as waiting time, turnaround time, CPU utilization, and more.

## Features

### Scheduling Algorithms:

- First-Come, First-Served (FCFS)
- Round Robin (RR) (with configurable time quantum)
- Shortest Job First (SJF) (Preemptive)
- Priority Scheduling (Preemptive)
- Multilevel Feedback Queue (MLFQ) (configurable time quantum and levels)
- Lottery Scheduling (optional extension)

### System Components:

- CPU Simulation: Handles process execution and context switching.
- Memory Manager: Simulates memory allocation and deallocation.
- I/O Subsystem: Simulates I/O requests and blocking/unblocking of processes.

### Command-Line Interface (CLI):

- Add processes with custom attributes (ID, Arrival Time, Burst Time, Priority).
- Select scheduling algorithms.
- Configure parameters like time quantum.
- View detailed performance metrics after simulation.
- Save/load process configurations and simulation results.

### Performance Metrics:

- Average Waiting Time
- Average Turnaround Time
- Average Response Time
- CPU Utilization
- Throughput
- Fairness Metrics (variance in waiting times)
- Context Switch Count

### Visualization:

- Text-based Gantt Chart: Shows the order of process execution over time.
- Event Logging: Logs simulation events (process arrivals, executions, terminations) into a file.

### File I/O:

- Save and load process configurations.
- Save simulation results including Gantt chart and performance metrics.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Command-Line Interface (CLI)](#command-line-interface-cli)
- [Supported Scheduling Algorithms](#supported-scheduling-algorithms)
- [Performance Metrics](#performance-metrics)
- [File I/O](#file-io)
- [Simulation Engine](#simulation-engine)
- [Future Enhancements](#future-enhancements)
- [Contributing](#contributing)

## Installation

1. Clone the Repository:

   ```bash
   git clone https://github.com/yourusername/scheduling-simulation-system.git
   cd scheduling-simulation-system
   ```

2. Compile the Project:

   ```bash
   g++ -std=c++11 -o scheduler main.cpp cpu.cpp memory_manager.cpp io_subsystem.cpp process.cpp pcb.cpp fcfs_scheduler.cpp round_robin_scheduler.cpp mlfq_scheduler.cpp preemptive_sjf_scheduler.cpp priority_scheduler.cpp simulation_engine.cpp file_io.cpp
   ```

3. Run the Simulation:

   ```bash
   ./scheduler
   ```

## Usage

After running the program, the Command-Line Interface (CLI) will guide you through configuring and running the simulation. The main options are:

1. Add a Process: Input process details like Process ID, Arrival Time, Burst Time, and Priority.
2. Choose Scheduling Algorithm: Select one of the supported scheduling algorithms.
3. Set Time Quantum: If using Round Robin or MLFQ, configure the time quantum.
4. Run Simulation: Execute the simulation, view performance metrics, and see the Gantt chart visualization.
5. Save/Load Configurations: Save and load configurations to and from files.
6. Save Simulation Results: Save performance metrics and Gantt chart results to a file.
7. Exit: Exit the program.

## Command-Line Interface (CLI)

The system offers a simple and interactive CLI for users:

1. Add Process: Enter Process ID, Arrival Time, Burst Time, and Priority.
2. Choose Algorithm: Select one of the following:
   - First-Come, First-Served (FCFS)
   - Round Robin (RR) (requires setting time quantum)
   - Multilevel Feedback Queue (MLFQ) (requires time quantum for different levels)
   - Preemptive Shortest Job First (SJF)
   - Preemptive Priority Scheduling
3. Set Time Quantum: Set the time quantum for Round Robin or MLFQ.
4. Run Simulation: Runs the scheduling simulation, displaying performance metrics and the Gantt chart.
5. Save/Load Configurations: Save process configurations and settings to a file or load previously saved configurations.
6. Save Results: Save simulation results (metrics, Gantt chart) to a file.

## Supported Scheduling Algorithms

- **First-Come, First-Served (FCFS):**
  - Processes are executed in the order they arrive.

- **Round Robin (RR):**
  - Each process is given a fixed time quantum. If the process doesn't complete within this time, it's preempted and returned to the queue.

- **Shortest Job First (SJF) (Preemptive):**
  - Process with the shortest remaining time is selected. This algorithm is also known as Shortest Remaining Time First (SRTF).

- **Priority Scheduling (Preemptive):**
  - Process with the highest priority (lowest priority number) is selected for execution. Preemption occurs if a higher-priority process arrives.

- **Multilevel Feedback Queue (MLFQ):**
  - Processes move between queues based on their execution behavior and time quantum. Supports multiple time quantum levels for varying priorities.

## Performance Metrics

After running the simulation, the following metrics will be displayed:

- Average Waiting Time: Average time a process spends in the ready queue.
- Average Turnaround Time: Time between process arrival and completion.
- Average Response Time: Time from process arrival to the first CPU execution.
- CPU Utilization: Percentage of time the CPU is executing processes.
- Throughput: Number of processes completed per unit of time.
- Fairness Metrics: Variance in waiting times to measure fairness.
- Context Switch Count: Number of context switches that occurred.

## File I/O

The system supports saving and loading simulation configurations and results using file I/O operations.

Save Configuration:

```cpp
FileIO::save_configuration(processes, "Round Robin", timeQuantum, "config.txt");
```

Load Configuration:

```cpp
FileIO::load_configuration(processes, schedulerAlgorithm, timeQuantum, "config.txt");
```

Save Simulation Results:

```cpp
FileIO::save_results(ganttChart, "results.txt");
```

Users can save process configurations and simulation settings to a file and reload them later for further analysis or re-simulation.

## Simulation Engine

The Simulation Engine manages the entire process scheduling simulation, handling process arrivals, CPU scheduling, and I/O operations. It supports multiple scheduling algorithms and collects performance metrics for evaluation. The engine processes time-based events, executes CPU bursts, handles context switches, and logs events to a file.

Key Components:

- Event-Driven Simulation: Events like process arrival, CPU execution, and I/O completions are processed in chronological order.
- Gantt Chart: A text-based Gantt chart showing the order of process execution over time.
- Performance Metrics: After simulation, the engine calculates and prints metrics like waiting time, turnaround time, and CPU utilization.

## Future Enhancements

Some potential future improvements include:

- Graphical User Interface (GUI): Develop a GUI to improve user interaction and visualization.
- Real-Time Scheduling: Add real-time scheduling algorithms such as Rate Monotonic or Earliest Deadline First (EDF).
- Multi-Core CPU Simulation: Support multi-core environments to simulate parallel execution of processes.

## Contributing

I welcome contributions from the community. To contribute:

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -m 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a Pull Request.

## Contact

For any issues or questions, feel free to open a GitHub issue or contact me directly via the repository.
