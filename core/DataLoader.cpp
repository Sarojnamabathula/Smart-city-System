#include "Graph.h"

void CityGraph::loadNodes(const string& path) {
    ifstream in(path);
    if (!in.is_open()) return;
    string line;
    getline(in, line); // header
    while (getline(in, line)) {
        if(line.empty()) continue;
        int id = stoi(line);
        if (id >= 1 && id <= 85000) node_exists[id] = true;
    }
    in.close();
}

void CityGraph::loadEdges(const string& path, bool is_main) {
    ifstream in(path);
    if (!in.is_open()) return;
    string line;
    getline(in, line); // header
    int edge_id = 1; // 1-indexed edge IDs 
    while (getline(in, line)) {
        for(char& c : line) if(c==',') c=' ';
        stringstream ss(line);
        int u, v; double w;
        if (ss >> u >> v >> w) {
            adj[u].push_back({v, w, edge_id});
            if(is_main) {
                edge_list.push_back({w, u, v});
                dynamic_weight[edge_id] = w; // init
            }
            edge_id++;
        }
    }
    in.close();
    
    // find isolated (only for main graph)
    if(is_main) {
        isolated_nodes.clear();
        for(int i=1; i<=85000; i++) {
            if(node_exists[i] && adj[i].empty()) {
                // To be truly isolated, in-degree must also be 0, but out-degree 0 is a start
                isolated_nodes.push_back(i);
            }
        }
    }
}

void CityGraph::loadEdges_override(const string& path) {
    for(int i=1; i<=85000; i++) adj[i].clear();
    edge_list.clear();
    loadEdges(path, false);
}

void CityGraph::updateWeight(int u, int v, double new_weight, HeapMetrics& m) {
    for(auto& edge : adj[u]) {
        if(edge.v == v) {
            edge.weight = new_weight;
            dynamic_weight[edge.edge_id] = new_weight;
            break;
        }
    }
}
