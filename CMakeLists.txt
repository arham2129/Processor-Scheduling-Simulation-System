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
    src/core/cpu.cpp
    src/core/io_subsystem.cpp
    src/core/memory_manager.cpp
    src/core/pcb.cpp
    src/core/process.cpp
    src/core/readyqueue.cpp
    src/schedulers/fcfs_scheduler.cpp
    src/schedulers/mlfq_scheduler.cpp
    src/schedulers/multilevel_queue_scheduler.cpp
    src/schedulers/preemptive_sjf_scheduler.cpp
    src/schedulers/priority_scheduler.cpp
    src/schedulers/round_robin_scheduler.cpp
    src/schedulers/sjf_scheduler.cpp
    src/simulation/file_io.cpp
    src/simulation/simulation_engine.cpp
    src/main.cpp
)

# ---- Executable ------------------------------------------------------------
add_executable(scheduler ${SOURCES})

# ---- Include directories ---------------------------------------------------
target_include_directories(scheduler PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/schedulers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/simulation
)

# ---- Installation ----------------------------------------------------------
install(TARGETS scheduler DESTINATION bin)
