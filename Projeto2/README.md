# Cache Simulator (OC Lab 2)

This is my implementation of a functional **Memory Hierarchy Simulator** developed for the **Computer Organization** (Organização de Computadores) course at LEIC, Instituto Superior Técnico (2023/2024). The project involves building a multi-level cache system from scratch in C.

## Project Overview

The objective of this lab is to design and implement a complete memory hierarchy, simulating how a CPU interacts with different cache levels before reaching main memory. The simulator tracks "time" (cycles) to evaluate the latency impact of cache hits and misses.

### Key Components
1.  **Directly-Mapped L1 Cache**: Implementation of a multi-line cache where each memory address maps to a specific cache line.
2.  **Directly-Mapped L2 Cache**: A second, larger cache level that acts as a bridge between L1 and Main Memory.
3.  **2-Way Set-Associative L2 Cache**: Evolution of the L2 design to support two lines per set, requiring a replacement policy (e.g., LRU - Least Recently Used) to handle conflict misses.



---

## Implementation Details

The simulator is built using a provided base structure and must adhere to strict performance and architectural constraints.

* **Language**: C (Targeting x86-64 Linux).
* **Memory Operations**:
    * `read(address, pointer)`: Fetches data from the hierarchy.
    * `write(address, value)`: Updates data using a specific write policy (typically Write-Through or Write-Back).
* **Timing Model**: Every access increments a global clock based on whether it results in an L1 hit, L2 hit, or a Main Memory access.

### Logic Flow
1.  **L1 Request**: Check L1. If hit, return. If miss, request from L2.
2.  **L2 Request**: Check L2. If hit, update L1 and return. If miss, request from Main Memory.
3.  **Main Memory**: Update both L2 and L1 on the way back to the "CPU".
