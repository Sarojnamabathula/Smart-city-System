#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <utility>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

using namespace std;

struct HeapMetrics {
    uint64_t insert_count         = 0;
    uint64_t extract_min_count    = 0;
    uint64_t decrease_key_count   = 0;
    uint64_t heap_swaps           = 0;
    uint64_t consolidations       = 0;  // Fibonacci only
    uint64_t cascading_cuts       = 0;  // Fibonacci only
    uint64_t tree_rotations       = 0;  // BST only
    uint64_t heap_height          = 0;
    double   insert_time_ns       = 0.0;
    double   extract_min_time_ns  = 0.0;
    double   decrease_key_time_ns = 0.0;
    double   total_dijkstra_ms    = 0.0;
    size_t   peak_memory_bytes    = 0;
    size_t   bytes_per_node       = 0;
    size_t   pointer_overhead     = 0;
};

struct Edge {
    int v;
    double weight;
    int edge_id;
};

class CityGraph {
public:
    static const int N = 85001;  // 1-indexed: nodes 1..85000

    // Primary: Adjacency List
    vector<vector<Edge>> adj;

    // Secondary: CSR 
    vector<int> csr_row;
    vector<int> csr_col;
    vector<double> csr_wt;

    // Edge List for Kruskal
    vector<tuple<double,int,int>> edge_list;

    bool node_exists[N] = {false};
    vector<int> isolated_nodes;
    double dynamic_weight[600001]; // up to 600K edges for dense graph

    CityGraph() {
        adj.resize(N);
    }

    void loadNodes(const string& path);
    void loadEdges(const string& path, bool is_main = true);
    void loadEdges_override(const string& path);
    void updateWeight(int u, int v, double new_weight, HeapMetrics& m);
    bool hasNode(int id) const { return id >= 1 && id <= 85000 && node_exists[id]; }

    void buildCSR();
};

#endif
