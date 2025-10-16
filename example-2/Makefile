# Makefile for CON40-C Examples - Example 2
# Demonstrates atomic counter operations with race conditions

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pthread
LDFLAGS = -pthread

# Targets
TARGETS = error solution solution_2

# Windows executable extension
ifeq ($(OS),Windows_NT)
    EXT = .exe
else
    EXT =
endif

# Default target: build all
all: $(TARGETS)

# Build error (noncompliant code)
error: error.c
	$(CC) $(CFLAGS) -o error$(EXT) error.c $(LDFLAGS)

# Build solution (using atomic_fetch_add)
solution: solution.c
	$(CC) $(CFLAGS) -o solution$(EXT) solution.c $(LDFLAGS)

# Build solution_2 (using atomic ++ operator)
solution_2: solution_2.c
	$(CC) $(CFLAGS) -o solution_2$(EXT) solution_2.c $(LDFLAGS)

# Clean all executables
clean:
	rm -f error$(EXT) solution$(EXT) solution_2$(EXT)

# Run all examples
run: all
	@echo "Running error (noncompliant code with race condition):"
	@./error$(EXT)
	@echo ""
	@echo "=========================================="
	@echo ""
	@echo "Running solution (using atomic_fetch_add):"
	@./solution$(EXT)
	@echo ""
	@echo "=========================================="
	@echo ""
	@echo "Running solution_2 (using atomic ++ operator):"
	@./solution_2$(EXT)

# Help message
help:
	@echo "Available targets:"
	@echo "  make all        - Build all executables (default)"
	@echo "  make error      - Build error executable only"
	@echo "  make solution   - Build solution executable only"
	@echo "  make solution_2 - Build solution_2 executable only"
	@echo "  make clean      - Remove all executables"
	@echo "  make run        - Build and run all examples"
	@echo "  make help       - Show this help message"

.PHONY: all clean run help
