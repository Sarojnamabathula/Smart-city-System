#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "../core/Graph.h"
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>

using namespace std;

class BinaryHeap {
private:
    struct Node {
        int v;
        double d;
    };
    vector<Node> heap;
    vector<int> pos;

    void swap_nodes(int i, int j, HeapMetrics& m) {
        m.heap_swaps++;
        pos[heap[i].v] = j;
        pos[heap[j].v] = i;
        swap(heap[i], heap[j]);
    }

    void sift_up(int i, HeapMetrics& m) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (heap[i].d < heap[p].d) {
                swap_nodes(i, p, m);
                i = p;
            } else {
                break;
            }
        }
    }

    void sift_down(int i, HeapMetrics& m) {
        int n = heap.size();
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < n && heap[left].d < heap[smallest].d) smallest = left;
            if (right < n && heap[right].d < heap[smallest].d) smallest = right;

            if (smallest != i) {
                swap_nodes(i, smallest, m);
                i = smallest;
            } else {
                break;
            }
        }
    }

public:
    BinaryHeap(int max_nodes) {
        pos.assign(max_nodes + 1, -1);
        heap.reserve(max_nodes);
    }

    void insert(int v, double d, HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        heap.push_back({v, d});
        int idx = heap.size() - 1;
        pos[v] = idx;
        m.insert_count++;
        sift_up(idx, m);
        auto end = chrono::high_resolution_clock::now();
        m.insert_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    int extract_min(HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        if (heap.empty()) return -1;
        int min_v = heap[0].v;
        pos[min_v] = -1;
        
        if (heap.size() > 1) {
            heap[0] = heap.back();
            pos[heap[0].v] = 0;
            heap.pop_back();
            sift_down(0, m);
        } else {
            heap.pop_back();
        }
        
        m.extract_min_count++;
        auto end = chrono::high_resolution_clock::now();
        m.extract_min_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        return min_v;
    }

    void decrease_key(int v, double d, HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        int idx = pos[v];
        if (idx != -1 && d < heap[idx].d) {
            heap[idx].d = d;
            m.decrease_key_count++;
            sift_up(idx, m);
        }
        auto end = chrono::high_resolution_clock::now();
        m.decrease_key_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    bool empty() const {
        return heap.empty();
    }

    void record_memory(HeapMetrics& m) {
        m.bytes_per_node = sizeof(Node) + sizeof(int); // 16B + 4B = 20B overhead array based
        m.pointer_overhead = 0; // Pure array
        m.peak_memory_bytes = heap.capacity() * sizeof(Node) + pos.capacity() * sizeof(int);
        m.heap_height = heap.size() > 0 ? log2(heap.size()) + 1 : 0;
    }
};

#endif
