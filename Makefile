# ==============================================================================
# Processor Scheduling Simulation System — Makefile
# ==============================================================================
#
# Usage:
#   make          — build the 'scheduler' executable (default target)
#   make clean    — remove all build artefacts
#   make rebuild  — clean then build
#   make run      — build and launch the simulator
#
# Requirements:
#   - g++ with C++17 support (GCC 7+ or Clang 5+)
#   - GNU Make 3.81+
#
# On Windows (MinGW/MSYS2):
#   mingw32-make   (or 'make' if GNU Make is on PATH)
# ==============================================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Wshadow \
            -Wno-unused-parameter -O2

TARGET   := scheduler

# All source files in the current directory
SRCS := cpu.cpp \
        fcfs_scheduler.cpp \
        file_io.cpp \
        io_subsystem.cpp \
        main.cpp \
        memory_manager.cpp \
        mlfq_scheduler.cpp \
        multilevel_queue_scheduler.cpp \
        pcb.cpp \
        preemptive_sjf_scheduler.cpp \
        priority_scheduler.cpp \
        process.cpp \
        readyqueue.cpp \
        round_robin_scheduler.cpp \
        simulation_engine.cpp \
        sjf_scheduler.cpp

# Derive object file names from source files
OBJS := $(SRCS:.cpp=.o)

# Derive dependency file names
DEPS := $(OBJS:.o=.d)

# ==============================================================================
# Default target
# ==============================================================================

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo ""
	@echo "  Build successful — run './$(TARGET)' to start the simulation."
	@echo ""

# Compile each .cpp to .o, generating dependency files alongside
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include auto-generated header dependency files (if they exist)
-include $(DEPS)

# ==============================================================================
# Utility targets
# ==============================================================================

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET)
	@echo "  Cleaned build artefacts."

.PHONY: rebuild
rebuild: clean all

.PHONY: run
run: all
	./$(TARGET)
