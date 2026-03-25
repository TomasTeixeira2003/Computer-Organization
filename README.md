# Computer Organization (OC) Portfolio: Memory Hierarchy & Profiling

This repository contains the laboratory projects developed for the **Computer Organization** (Organização de Computadores) course at LEIC, Instituto Superior Técnico (2023/2024). These projects explore the intersection of hardware architecture and software optimization.

---

## Lab 1: System Modeling and Profiling
**Focus:** Performance analysis and code optimization using hardware counters.

The objective was to characterize the memory hierarchy of a physical microprocessor and optimize matrix multiplication ($C = A \times B$) to leverage cache efficiency.

* **Cache Modeling**: Used a profiling tool to determine L1/L2 cache sizes, line sizes, and associativity by measuring latency across various memory access strides.
* **Hardware Counters**: Integrated the **PAPI API** to collect real-time data on L1/L2 data cache misses and total execution cycles.
* **Optimizations**: Implemented **Tiling (Blocking)** techniques to reduce capacity misses, significantly improving throughput for large-scale matrix operations.



---

## Lab 2: Multi-Level Cache Simulator
**Focus:** Architectural simulation and cache logic implementation.

This project involved building a functional, software-based simulation of a multi-level memory hierarchy in **C**.

* **L1 Cache**: Implementation of a multi-line **Directly-Mapped** cache.
* **L2 Cache**: 
    * Initially implemented as Directly-Mapped.
    * Advanced to a **2-Way Set-Associative** structure.
* **Replacement Policy**: Implemented **LRU (Least Recently Used)** logic for the set-associative L2 cache.
* **Timing Model**: Developed a cycle-accurate accounting system to measure the impact of hits and misses at different levels of the hierarchy (L1 vs. L2 vs. Main Memory).
