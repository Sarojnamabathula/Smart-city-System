# 🏙️ Real-Time Smart City Navigation and Traffic Optimization System

<div align="center">

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=cplusplus)
![React](https://img.shields.io/badge/React-18-61DAFB?style=for-the-badge&logo=react)
![TypeScript](https://img.shields.io/badge/TypeScript-5.0-3178C6?style=for-the-badge&logo=typescript)
![Vite](https://img.shields.io/badge/Vite-4.0-646CFF?style=for-the-badge&logo=vite)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

**A high-performance smart city traffic system featuring multi-priority queue analysis, dynamic shortest path computation, and real-time congestion visualization.**

[🚀 Quick Start](#-quick-start) · [🏗️ Architecture](#️-architecture) · [📊 Datasets](#-datasets) · [📈 Benchmarks](#-benchmarks)

</div>

---

## 📋 Table of Contents

- [Project Overview](#-project-overview)
- [Architecture](#️-architecture)
- [Data Structures](#-data-structures)
- [Algorithms](#-algorithms)
- [Novel Contributions](#-novel-contributions)
- [Quick Start](#-quick-start)
- [Project Structure](#-project-structure)
- [React Dashboard](#-react-dashboard)

---

## 🌆 Project Overview

This project implements a **Real-Time Smart City Navigation System** designed to optimize traffic flow using advanced data structures and algorithms. The system specifically compares different priority queue implementations within Dijkstra's algorithm to determine the most efficient approach for large-scale urban road networks.

### Key Features
- **Multiple Priority Queues**: Comparison of Binary Heap, Binomial Heap, Fibonacci Heap, and more.
- **Dynamic Traffic Updates**: Real-time weight adjustment based on simulated congestion.
- **Novel Algorithms**: Implementation of `AdaptiveCongestionDijkstra (ACD)` and `CongestionSegTree++`.
- **Interactive Dashboard**: Visual representation of the road network and algorithm performance.

---

## 🏗️ Architecture

The system is split into a high-performance C++ backend and a modern React frontend:

- **C++ Backend**: Handles graph representation, shortest path computations, and performance benchmarking.
- **React Dashboard**: Provides a web-based UI for visualizing the city graph, running benchmarks, and observing real-time traffic simulations.

---

## 🗂️ Data Structures

### Priority Queues
The project implements five mandatory priority queues for performance comparison:
1. **Unsorted Array**: O(1) insert, O(V) extract-min.
2. **Binary Heap**: O(log V) balanced performance.
3. **Binomial Heap**: Forest of binomial trees.
4. **Fibonacci Heap**: O(1) amortized decrease-key, ideal for dense graphs.
5. **Balanced BST**: Using tree-based structures as a priority queue.

### Supplementary Structures
- **Adjacency List**: Efficient sparse graph representation.
- **CongestionSegTree++**: A segment tree with lazy propagation for efficient temporal traffic decay.
- **LRU Cache**: Caching frequent routes to reduce redundant computations.

---

## 🚀 Quick Start

### 1. Build the C++ Backend
Ensure you have a C++17 compatible compiler (GCC 9+ or Clang 10+).
```bash
# Compilation (example using g++)
g++ -std=c++17 algorithms/*.cpp core/*.cpp data/*.cpp benchmarks/*.cpp -I. -o smartcity
```

### 2. Run the Dashboard
The dashboard is built with React and Vite.
```bash
cd dashboard
npm install
npm run dev
```

---

## 📁 Project Structure

```bash
SmartCitySystem/
├── algorithms/         # Dijkstra variants and ACD implementation
├── core/               # Graph and data loading logic
├── data/               # Mock data generation and benchmarking results
├── dashboard/          # React + TypeScript frontend
├── priority_queues/    # 5 PQ implementations (Binary, Binomial, Fibonacci, etc.)
├── structures/         # Advanced structures like CongestionSegTree++
├── benchmarks/         # Performance measurement and output generation
└── api/                # Backend API logic (if applicable)
```

---

## 🧪 Benchmarks

The system generates detailed performance metrics, comparing:
- Execution time for different PQ variants.
- Performance under varying graph densities (Sparse vs. Dense).
- Memory overhead (pointer usage vs. cache efficiency).

Results are typically exported to `data/benchmark_results.csv` and visualized in the dashboard.

---

## 📜 License
This project is for academic purposes. Implementation by [Your Team Name/Members].
