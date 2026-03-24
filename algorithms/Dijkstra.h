#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../core/Graph.h"

struct ShortestPathResult {
    vector<double>   dist;          // dist[v] = shortest distance src->v
    vector<int>      parent;        // for path reconstruction
    uint64_t         relaxations = 0;   // edge relaxations performed
    uint64_t         decrease_keys = 0; // decrease-key calls made
    uint64_t         extract_mins = 0;  // extract-min calls made
    double           runtime_ms = 0;    // wall-clock time
    bool             valid = true;         // false if src not in graph (5 MSQ cases)
};

ShortestPathResult dijkstra_array       (int src, CityGraph& g, HeapMetrics& m);
ShortestPathResult dijkstra_binary_heap (int src, CityGraph& g, HeapMetrics& m);
ShortestPathResult dijkstra_binomial    (int src, CityGraph& g, HeapMetrics& m);
ShortestPathResult dijkstra_fibonacci   (int src, CityGraph& g, HeapMetrics& m);
ShortestPathResult dijkstra_bst         (int src, CityGraph& g, HeapMetrics& m);

#endif
