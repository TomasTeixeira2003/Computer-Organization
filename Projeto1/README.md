# System Modeling and Profiling (OC Lab 1)

This is my implementation and analysis for the **First Lab Assignment** of the **Computer Organization** (Organização de Computadores) course at LEIC, Instituto Superior Técnico (2023/2024). The project focuses on hardware-level performance modeling and software optimization.

## Project Overview

The objective of this assignment is to understand the memory hierarchy of a target microprocessor and use that knowledge to optimize matrix multiplication operations.

### Key Objectives
1.  **Cache Characterization**: Determining the specific characteristics (size, associativity, and line size) of the L1 and L2 caches.
2.  **Performance Profiling**: Using hardware performance counters to measure cache misses and execution cycles.
3.  **Software Optimization**: Applying code transformation techniques to minimize cache misses and improve execution time.

## Technologies & Tools

* **Language**: C
* **API**: **PAPI** (Performance Application Programming Interface) – used to access hardware performance counters.
* **Target Environment**: Linux-based lab computers with specific Intel/AMD microarchitectures.
* **Performance Metrics**:
    * `PAPI_L1_DCM`: L1 Data Cache Misses.
    * `PAPI_L2_DCM`: L2 Data Cache Misses.
    * `PAPI_TOT_CYC`: Total clock cycles.

---

## Methodology

### 1. Modeling L1 and L2 Caches
The system is modeled by executing a specific memory-access program that varies the stride and the size of the data set. By plotting the access times, we identify:
* **Cache Line Size**: Found by observing where the number of misses per access changes.
* **Cache Size**: Identified by the "steps" in latency when the data set exceeds cache capacity.
* **Associativity**: Determined by analyzing conflict misses within specific power-of-two strides.

### 2. Matrix Multiplication Optimization
The lab involves optimizing a standard $N \times N$ matrix multiplication algorithm ($C = A \times B$).
* **Baseline**: Standard triple-nested loop (ijk).
* **Optimization Technique**: **Blocking (Tiling)**. By dividing matrices into smaller blocks that fit entirely within the L2 cache, we significantly reduce the number of capacity misses and memory traffic.
