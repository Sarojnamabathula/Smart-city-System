#ifndef ACD_H
#define ACD_H

#include "../core/Graph.h"
#include "../structures/CongestionSegTreePP.h"
#include "../priority_queues/FibonacciHeap.h" 
#include "../priority_queues/BinaryHeap.h"

// Novel Algorithm: Adaptive Congestion Dijkstra (ACD)
ShortestPathResult acd(int src, int dst, CityGraph& g, 
                       CongestionSegTreePP& cst, int tick, 
                       HeapMetrics& m);

#endif
