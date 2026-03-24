#include "ACD.h"
#include <chrono>

ShortestPathResult acd(int src, int dst, CityGraph& g, 
                       CongestionSegTreePP& cst, int tick, 
                       HeapMetrics& m) {
    ShortestPathResult res;
    if (!g.hasNode(src)) { res.valid = false; return res; }
    res.dist.assign(g.N + 1, 1e18);
    res.parent.assign(g.N + 1, -1);
    
    // Convergence specific paramaters
    double alpha = 0.3;
    double beta = 2.0;
    double T_cycle = 3600.0;
    
    // We will use FibonacciHeap as the primary for ACD given its performance
    FibonacciHeap pq(g.N);
    
    auto start_time = chrono::high_resolution_clock::now();
    res.dist[src] = 0;
    pq.insert(src, 0.0, m);
    
    while(!pq.empty()) {
        int u = pq.extract_min(m);
        if (u == -1) break;
        res.extract_mins++;
        if (res.dist[u] == 1e18) break;
        if (dst != -1 && u == dst) break; // Early exit if point-to-point

        for (const auto& edge : g.adj[u]) {
            int v = edge.v;
            double base_weight = edge.weight;
            int edge_id = edge.edge_id;
            
            // ACD Novel Weight Function
            double congestion_factor = cst.point_query(edge_id);
            double incident_multiplier = (congestion_factor > 1.0) ? beta * congestion_factor : 0.0;
            
            // w(e, t) = Distance_csv * [1 + alpha*sin(2pi*t/T) + beta*incident(e, t)]
            double dynamic_weight = base_weight * (1.0 + alpha * sin(2.0 * M_PI * tick / T_cycle) + incident_multiplier);
            
            // Guarantee w(e,t) > 0 since base_weight >= 1 and alpha = 0.3
            if (dynamic_weight < 0.1) dynamic_weight = 0.1;
            
            res.relaxations++;
            
            if (res.dist[u] + dynamic_weight < res.dist[v]) {
                if (res.dist[v] == 1e18) {
                    res.dist[v] = res.dist[u] + dynamic_weight;
                    res.parent[v] = u;
                    pq.insert(v, res.dist[v], m);
                } else {
                    res.dist[v] = res.dist[u] + dynamic_weight;
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
