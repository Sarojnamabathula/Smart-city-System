#ifndef CONGESTION_SEG_TREE_PP_H
#define CONGESTION_SEG_TREE_PP_H

#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class CongestionSegTreePP {
private:
    int n;
    vector<double> tree;
    vector<double> lazy_add;
    vector<double> lazy_mult;

    void push(int node, int start, int end) {
        if (lazy_mult[node] != 1.0 || lazy_add[node] != 0.0) {
            tree[node] = tree[node] * lazy_mult[node] + lazy_add[node];

            if (start != end) {
                lazy_mult[2 * node] *= lazy_mult[node];
                lazy_add[2 * node] = lazy_add[2 * node] * lazy_mult[node] + lazy_add[node];
                lazy_mult[2 * node + 1] *= lazy_mult[node];
                lazy_add[2 * node + 1] = lazy_add[2 * node + 1] * lazy_mult[node] + lazy_add[node];
            }
            lazy_mult[node] = 1.0;
            lazy_add[node] = 0.0;
        }
    }

    void update_range(int node, int start, int end, int l, int r, double m_val, double a_val) {
        push(node, start, end);
        if (start > end || start > r || end < l) return;

        if (start >= l && end <= r) {
            lazy_mult[node] *= m_val;
            lazy_add[node] = lazy_add[node] * m_val + a_val;
            push(node, start, end);
            return;
        }

        int mid = (start + end) / 2;
        update_range(2 * node, start, mid, l, r, m_val, a_val);
        update_range(2 * node + 1, mid + 1, end, l, r, m_val, a_val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    double query_range(int node, int start, int end, int l, int r) {
        push(node, start, end);
        if (start > end || start > r || end < l) return 0.0;

        if (start >= l && end <= r) return tree[node];

        int mid = (start + end) / 2;
        double p1 = query_range(2 * node, start, mid, l, r);
        double p2 = query_range(2 * node + 1, mid + 1, end, l, r);
        return max(p1, p2);
    }

public:
    CongestionSegTreePP(int size) {
        n = size;
        tree.assign(4 * n + 1, 1.0);     // base congestion is 1.0
        lazy_add.assign(4 * n + 1, 0.0);
        lazy_mult.assign(4 * n + 1, 1.0);
    }

    void range_update(int l, int r, double delta) {
        update_range(1, 1, n, l, r, 1.0, delta);
    }

    double range_query_max(int l, int r) {
        return query_range(1, 1, n, l, r);
    }
    
    double point_query(int idx) {
        return query_range(1, 1, n, idx, idx);
    }

    void tick_decay(double lambda = 0.95) {
        // Multiply entire range by lambda in O(1) conceptually because of the root lazy tag
        update_range(1, 1, n, 1, n, lambda, 0.0);
    }

    void incident_inject(int edge_id, double severity = 5.0) {
        // Force the edge to be exactly 5.0
        // Doing this via multiplying by 0 and adding severity
        update_range(1, 1, n, edge_id, edge_id, 0.0, severity);
    }
};

#endif
