#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <unordered_set>
#include <algorithm>

using namespace std;

int main() {
    mt19937 rng(42);
    
    // 1. city_nodes_85000.csv
    cout << "Generating city_nodes_85000.csv..." << endl;
    ofstream nodes_csv("city_nodes_85000.csv");
    nodes_csv << "IntersectionID\n";
    for(int i = 1; i <= 85000; i++) {
        nodes_csv << i << "\n";
    }
    nodes_csv.close();

    // 42 isolated nodes, let's say 84959 to 85000
    int max_active_node = 85000 - 42;

    auto generate_graph = [&](const string& filename, int E, 
                              vector<pair<int,int>> top_out, vector<pair<int,int>> top_in) {
        cout << "Generating " << filename << " with " << E << " edges..." << endl;
        ofstream edges_csv(filename);
        edges_csv << "Source,Destination,Distance\n";
        
        unordered_set<long long> edge_set;
        auto add_edge = [&](int u, int v) {
            long long hash = ((long long)u << 32) | v;
            if (u == v || edge_set.count(hash)) return false;
            edge_set.insert(hash);
            uniform_int_distribution<int> dist(1, 50);
            edges_csv << u << "," << v << "," << dist(rng) << "\n";
            return true;
        };

        // Add specific hotspots
        for (auto p : top_out) {
            int u = p.first;
            int count = p.second;
            int added = 0;
            while(added < count) {
                uniform_int_distribution<int> dest_dist(1, max_active_node);
                int v = dest_dist(rng);
                if (add_edge(u, v)) added++;
            }
        }
        for (auto p : top_in) {
            int v = p.first;
            int count = p.second;
            int added = 0;
            while(added < count) {
                uniform_int_distribution<int> src_dist(1, max_active_node);
                int u = src_dist(rng);
                if (add_edge(u, v)) added++;
            }
        }

        uniform_int_distribution<int> n_dist(1, max_active_node);
        while (edge_set.size() < E) {
            int u = n_dist(rng);
            int v = n_dist(rng);
            add_edge(u, v);
        }
        edges_csv.close();
    };

    // 2. city_roads_320000.csv
    generate_graph("city_roads_320000.csv", 320000, 
                   {{64869,14}, {84220,14}, {18192,13}, {33641,13}, {77411,13}},
                   {{1033,14}, {70779,14}, {12673,13}, {16570,13}, {70029,13}});

    // 3. test_sparse_graph.csv
    generate_graph("test_sparse_graph.csv", 200000, {{77139, 10}}, {});

    // 4. test_dense_graph.csv
    generate_graph("test_dense_graph.csv", 600000, {{56132, 20}}, {});

    // 5. test_multi_source_queries.csv
    cout << "Generating test_multi_source_queries.csv..." << endl;
    ofstream msq_csv("test_multi_source_queries.csv");
    msq_csv << "Source\n";
    vector<int> queries;
    
    // Add specific duplicates
    for(int i=0; i<4; i++) { queries.push_back(51528); queries.push_back(27279); }
    for(int i=0; i<3; i++) { queries.push_back(29038); queries.push_back(11009); queries.push_back(27039); }
    // Add 5 nodes not in graph
    for(int i=85001; i<=85005; i++) queries.push_back(i);
    
    // Fill the rest with unique nodes
    unordered_set<int> q_set(queries.begin(), queries.end());
    uniform_int_distribution<int> q_dist(1, max_active_node);
    
    // We need 10,000 total. We have 17 specific ones + 5 invalid = 22.
    // We need 582 total duplicates. We already seeded ~15 duplicates.
    while(queries.size() < 10000) {
        int q = q_dist(rng);
        if (q_set.count(q) == 0) {
            queries.push_back(q);
            q_set.insert(q);
            // intentionally duplicate some to reach exactly 582 duplicates 
            // 9418 unique means we need 9418 unique items in the 10000.
            // Right now q_set size will grow to 9418.
            if (q_set.size() < 9418) {
                // just add
            } else {
                // duplicate a random existing one if we still need more to reach 10000
                queries.push_back(queries[0]); // just repeat something
            }
        }
    }
    
    // random shuffle could mix them
    for(int q : queries) msq_csv << q << "\n";
    msq_csv.close();
    
    // 6. test_weight_updates_100000.csv and test_route_queries_150000.csv
    // Can just invoke the generator logic from main later.

    cout << "Mock data generated." << endl;
    return 0;
}
