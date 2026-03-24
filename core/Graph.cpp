#include "Graph.h"

void CityGraph::buildCSR() {
    csr_row.assign(N + 1, 0);
    // count edges per vertex
    for (int u = 1; u < N; ++u) {
        csr_row[u + 1] = adj[u].size();
    }
    // prefix sums
    for (int u = 1; u <= N; ++u) {
        csr_row[u] += csr_row[u - 1];
    }
    int total_edges = csr_row[N];
    csr_col.resize(total_edges);
    csr_wt.resize(total_edges);
    
    // reset row counts to use as offsets, then restore
    vector<int> current_offset = csr_row;
    for (int u = 1; u < N; ++u) {
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int offset = current_offset[u]++;
            csr_col[offset] = adj[u][i].v;
            csr_wt[offset] = adj[u][i].weight;
        }
    }
}
