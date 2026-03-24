#include "Dijkstra.h"
#include "../priority_queues/ArrayPQ.h"
#include "../priority_queues/BinaryHeap.h"
#include "../priority_queues/BinomialHeap.h"
#include "../priority_queues/FibonacciHeap.h"
#include "../priority_queues/BalancedBST_PQ.h"
#include <chrono>

template<typename PQ>
ShortestPathResult run_dijkstra(int src, CityGraph& g, HeapMetrics& m) {
    ShortestPathResult res;
    if (!g.hasNode(src)) { res.valid = false; return res; }
    res.dist.assign(g.N + 1, 1e18);
    res.parent.assign(g.N + 1, -1);
    
    auto start_time = chrono::high_resolution_clock::now();
    PQ pq(g.N);
    
    res.dist[src] = 0;
    pq.insert(src, 0.0, m);
    
    while(!pq.empty()) {
        int u = pq.extract_min(m);
        if (u == -1) break;
        res.extract_mins++;
        if (res.dist[u] == 1e18) break;

        for (const auto& edge : g.adj[u]) {
            int v = edge.v;
            double weight = edge.weight;
            res.relaxations++;
            
            if (res.dist[u] + weight < res.dist[v]) {
                if (res.dist[v] == 1e18) {
                    res.dist[v] = res.dist[u] + weight;
                    res.parent[v] = u;
                    pq.insert(v, res.dist[v], m);
                } else {
                    res.dist[v] = res.dist[u] + weight;
                    res.parent[v] = u;
                    pq.decrease_key(v, res.dist[v], m);
                    res.decrease_keys++;
                }
            }
        }
    }
    pq.record_memory(m);
    
    auto end_time = chrono::high_resolution_clock::now();
    res.runtime_ms = chrono::duration<double, std::milli>(end_time - start_time).count();
    m.total_dijkstra_ms += res.runtime_ms;
    
    return res;
}

ShortestPathResult dijkstra_array(int src, CityGraph& g, HeapMetrics& m) {
    return run_dijkstra<ArrayPQ>(src, g, m);
}

ShortestPathResult dijkstra_binary_heap(int src, CityGraph& g, HeapMetrics& m) {
    return run_dijkstra<BinaryHeap>(src, g, m);
}

ShortestPathResult dijkstra_binomial(int src, CityGraph& g, HeapMetrics& m) {
    return run_dijkstra<BinomialHeap>(src, g, m);
}

ShortestPathResult dijkstra_fibonacci(int src, CityGraph& g, HeapMetrics& m) {
    return run_dijkstra<FibonacciHeap>(src, g, m);
}

ShortestPathResult dijkstra_bst(int src, CityGraph& g, HeapMetrics& m) {
    return run_dijkstra<BalancedBST_PQ>(src, g, m);
}
