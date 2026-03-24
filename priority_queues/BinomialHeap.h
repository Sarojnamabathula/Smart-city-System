#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include "../core/Graph.h"
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

class BinomialHeap {
private:
    struct Node {
        int v;
        double d;
        int degree;
        Node* parent;
        Node* child;
        Node* sibling;
    };
    Node* head;
    vector<Node*> node_ptrs;
    size_t active_nodes;

    Node* merge_roots(Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;
        Node* res = nullptr;
        Node** tail = &res;
        while (h1 && h2) {
            if (h1->degree <= h2->degree) {
                *tail = h1; h1 = h1->sibling;
            } else {
                *tail = h2; h2 = h2->sibling;
            }
            tail = &((*tail)->sibling);
        }
        *tail = h1 ? h1 : h2;
        return res;
    }

    void link_trees(Node* min_node, Node* other) {
        other->parent = min_node;
        other->sibling = min_node->child;
        min_node->child = other;
        min_node->degree++;
    }

    Node* union_heaps(Node* h1, Node* h2) {
        Node* h = merge_roots(h1, h2);
        if (!h) return nullptr;
        Node* prev = nullptr;
        Node* x = h;
        Node* next = x->sibling;
        while (next) {
            if (x->degree != next->degree || (next->sibling && next->sibling->degree == x->degree)) {
                prev = x;
                x = next;
            } else if (x->d <= next->d) {
                x->sibling = next->sibling;
                link_trees(x, next);
            } else {
                if (!prev) h = next;
                else prev->sibling = next;
                link_trees(next, x);
                x = next;
            }
            next = x->sibling;
        }
        return h;
    }

public:
    BinomialHeap(int max_nodes) {
        head = nullptr;
        node_ptrs.assign(max_nodes + 1, nullptr);
        active_nodes = 0;
    }

    void insert(int v, double d, HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        Node* node = new Node{v, d, 0, nullptr, nullptr, nullptr};
        node_ptrs[v] = node;
        head = union_heaps(head, node);
        active_nodes++;
        m.insert_count++;
        auto end = chrono::high_resolution_clock::now();
        m.insert_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    int extract_min(HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        if (!head) return -1;
        
        Node* min_node = head;
        Node* min_prev = nullptr;
        Node* x = head;
        Node* prev = nullptr;
        
        while (x) {
            if (x->d < min_node->d) {
                min_node = x;
                min_prev = prev;
            }
            prev = x;
            x = x->sibling;
        }

        if (!min_prev) head = min_node->sibling;
        else min_prev->sibling = min_node->sibling;

        Node* child = min_node->child;
        Node* rev_child = nullptr;
        while (child) {
            Node* next = child->sibling;
            child->sibling = rev_child;
            child->parent = nullptr;
            rev_child = child;
            child = next;
        }
        
        head = union_heaps(head, rev_child);
        int ret_v = min_node->v;
        node_ptrs[ret_v] = nullptr;
        delete min_node;
        active_nodes--;
        
        m.extract_min_count++;
        auto end = chrono::high_resolution_clock::now();
        m.extract_min_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        return ret_v;
    }

    void decrease_key(int v, double d, HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        Node* x = node_ptrs[v];
        if (x && d < x->d) {
            x->d = d;
            m.decrease_key_count++;
            Node* y = x;
            Node* z = y->parent;
            while (z && y->d < z->d) {
                swap(y->v, z->v);
                swap(y->d, z->d);
                node_ptrs[y->v] = y;
                node_ptrs[z->v] = z;
                m.heap_swaps++;
                y = z;
                z = y->parent;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        m.decrease_key_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    bool empty() const {
        return head == nullptr;
    }

    void record_memory(HeapMetrics& m) {
        m.bytes_per_node = sizeof(Node); 
        m.pointer_overhead = 3 * sizeof(Node*); // parent, child, sibling = 24B
        m.peak_memory_bytes = active_nodes * sizeof(Node) + node_ptrs.capacity() * sizeof(Node*);
    }
};

#endif
