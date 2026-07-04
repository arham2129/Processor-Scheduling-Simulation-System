cmake_minimum_required(VERSION 3.14)

project(ProcessorSchedulingSimulation
        VERSION 2.0.0
        DESCRIPTION "Processor Scheduling Simulation System"
        LANGUAGES CXX)

# ---- Require C++17 --------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Compiler warnings -----------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wno-unused-parameter)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# ---- Source files ----------------------------------------------------------
set(SOURCES
    cpu.cpp
    fcfs_scheduler.cpp
    file_io.cpp
    io_subsystem.cpp
    main.cpp
    memory_manager.cpp
    mlfq_scheduler.cpp
    multilevel_queue_scheduler.cpp
    pcb.cpp
    preemptive_sjf_scheduler.cpp
    priority_scheduler.cpp
    process.cpp
    readyqueue.cpp
    round_robin_scheduler.cpp
    simulation_engine.cpp
    sjf_scheduler.cpp
)

# ---- Executable ------------------------------------------------------------
add_executable(scheduler ${SOURCES})

# ---- Installation ----------------------------------------------------------
install(TARGETS scheduler DESTINATION bin)
