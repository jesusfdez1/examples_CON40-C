# CON40-C: Do not refer to an atomic variable twice in an expression

This repository contains practical implementations of the **CON40-C** security rule from the CERT C Coding Standard. The rule states that operations on shared atomic variables should not be performed in isolation, as this can lead to race conditions in multithreaded environments.

## Context

In concurrent programming, atomic operations provide atomicity guarantees for individual operations. However, when multiple atomic operations are performed sequentially to complete a single logical task, the set of operations is not atomic. This creates a time window where other threads can interfere, causing inconsistencies in shared data.

### Problems
Violation of CON40-C can result in:
- **Lost updates**: Lost updates when multiple threads read the same value before writing
- **Race conditions**: The result depends on the execution order of threads
- **Data inconsistency**: Inconsistencies in the state of shared variables
- **Non-deterministic behavior**: Non-deterministic behavior that makes debugging difficult

## Repository Structure

```
examples_CON40-C/
│
├── README.md
│
├── example-1/                     # Toggle operations on atomic_bool
│   ├── error.c                    # Non-compliant code
│   ├── solution.c                 # Solution with atomic_compare_exchange_weak
│   ├── solution_2.c               # Solution with XOR operator
│   └── Makefile
│
└── example-2/                     # Increment operations on atomic_int
    ├── error.c                    # Non-compliant code
    ├── solution.c                 # Solution with atomic_fetch_add
    ├── solution_2.c               # Solution with ++ operator
    └── Makefile
```

## System Requirements

- **Compiler**: GCC 4.9+ or Clang 3.1+ with C11 support
- **Make**: GNU Make or compatible
- **Operating System**: Linux/Unix, Windows (MinGW, MSYS2, WSL), macOS

## Build Instructions

```bash
# Compile an example
cd example-1/ or cd example-2/
make

# Compile specific file
make error
make solution
make solution_2

# Run all examples
make run

# Clean compiled files
make clean
```


