# TestForge Stress Test Suite

This folder contains a comprehensive collection of **stress test source files** designed to validate the robustness of **TestForge** — the automated unit test generator for embedded systems.

These files deliberately use complex, real-world embedded patterns that are commonly found in production firmware but are often challenging for parsers and test generators.

## Purpose

The goal of this suite is to ensure TestForge can reliably:

- Parse complex C, C++, and assembly code
- Handle hardware abstraction layers, volatile registers, and bitfields
- Generate useful, compilable unit tests even on difficult constructs
- Support real RTOS, ISR, and driver patterns

## File Overview

### Core Stress Test Files

| File                        | Focus Area                              | Key Patterns Tested                              | Difficulty |
|----------------------------|-----------------------------------------|--------------------------------------------------|----------|
| `hal_stress.c` + `.h`      | Hardware Abstraction Layers            | Macros, volatile registers, guards               | High     |
| `stress_register_map.c`    | Register Maps & Bitfields              | Unions, bitfields, shadow registers, macros      | **Very High** |
| `stress_isr.c`             | Interrupt Service Routines             | Volatile shared data, ISR-to-task communication  | High     |
| `stress_rtos_queue.c`      | RTOS Queues & Semaphores               | Queue + Mutex + Binary Semaphore + ISR posting   | High     |
| `mega_stress_with_asm.c`   | Combined + Inline Assembly             | Bitfields, nested structs, callbacks, inline asm | Very High |
| `mega_stress.c`            | General Mega Stress                    | Multiple difficult patterns in one file          | High     |
| `stress_edge_cases.c`      | Edge Cases                             | Bitfields, unions, guard conditions              | High     |
| `stress_nested_structs.c`  | Deeply Nested Structures               | Multi-level struct access                        | High     |
| `callback_table.c`         | Callback Tables                        | Arrays of function pointers                      | High     |
| `complex_callbacks.c`      | Function Pointers & Callbacks          | Complex callback patterns                        | Medium   |
| `rtos_stress.c`            | RTOS Primitives                        | Tasks, semaphores, volatile shared state         | High     |
| `stress_cpp.cpp`           | C++ Stress Cases                       | Templates, inheritance, RAII, virtual functions  | High     |
| `mixed_assembly.s`         | Assembly Parsing                       | Register push/pop, function boundaries           | Medium   |

### Support Files

| File                        | Purpose                                      |
|----------------------------|----------------------------------------------|
| `mock_hal_stress.c`        | Mock implementations for HAL functions       |
| `validate_stress_tests.py` | Automated validation + compilation checker   |
| `CMakeLists.txt`           | Build system for running generated tests     |

## How to Use

### 1. Generate Tests

From the project root:

```bash
# Generate tests for a single file
testforge generate -i test_data/StressTestApp/stress_register_map.c --framework unity

# Or generate for the entire suite
testforge generate -i test_data/StressTestApp/ --framework unity