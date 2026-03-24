#ifndef ARRAY_PQ_H
#define ARRAY_PQ_H

#include "../core/Graph.h"
#include <vector>
#include <chrono>

using namespace std;

class ArrayPQ {
public:
    int N;
    vector<double> dist;
    vector<bool> visited;
    
    ArrayPQ(int size) : N(size), dist(size + 1, 1e18), visited(size + 1, false) {}
    
    void insert(int v, double d, HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        dist[v] = d;
        m.insert_count++;
        auto end = chrono::high_resolution_clock::now();
        m.insert_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }
    
    int extract_min(HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        int best_u = -1;
        double min_d = 1e18;
        for (int u = 1; u <= N; ++u) {
            if (!visited[u] && dist[u] < min_d) {
                min_d = dist[u];
                best_u = u;
            }
        }
        if (best_u != -1) visited[best_u] = true;
        m.extract_min_count++;
        auto end = chrono::high_resolution_clock::now();
        m.extract_min_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        return best_u;
    }
    
    void decrease_key(int v, double d, HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        if (d < dist[v]) dist[v] = d;
        m.decrease_key_count++;
        auto end = chrono::high_resolution_clock::now();
        m.decrease_key_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }
    
    bool empty() {
        for (int u = 1; u <= N; ++u) {
            if (!visited[u] && dist[u] < 1e18) return false;
        }
        return true;
    }
    
    void record_memory(HeapMetrics& m) {
        m.bytes_per_node = 0; // Array based 0 overhead
        m.pointer_overhead = 0;
        m.peak_memory_bytes = dist.capacity() * sizeof(double) + visited.capacity() * sizeof(bool);
    }
};

#endif
