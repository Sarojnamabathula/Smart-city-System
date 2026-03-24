#ifndef BALANCED_BST_PQ_H
#define BALANCED_BST_PQ_H

#include "../core/Graph.h"
#include <vector>
#include <chrono>

using namespace std;

class BalancedBST_PQ {
private:
    struct Node {
        int v;
        double d;
        int height;
        Node* left;
        Node* right;
    };
    Node* root;
    vector<Node*> node_ptrs;
    size_t active_nodes;

    int get_height(Node* n) { return n ? n->height : 0; }
    int get_balance(Node* n) { return n ? get_height(n->left) - get_height(n->right) : 0; }
    
    void update_height(Node* n) {
        if (n) n->height = 1 + max(get_height(n->left), get_height(n->right));
    }

    Node* right_rotate(Node* y, HeapMetrics& m) {
        m.tree_rotations++;
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        update_height(y);
        update_height(x);
        return x;
    }

    Node* left_rotate(Node* x, HeapMetrics& m) {
        m.tree_rotations++;
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        update_height(x);
        update_height(y);
        return y;
    }

    Node* insert_node(Node* node, int v, double d, Node*& new_node, HeapMetrics& m) {
        if (!node) {
            new_node = new Node{v, d, 1, nullptr, nullptr};
            return new_node;
        }
        // BST order by distance, then vertex ID to break ties
        if (d < node->d || (d == node->d && v < node->v)) {
            node->left = insert_node(node->left, v, d, new_node, m);
        } else {
            node->right = insert_node(node->right, v, d, new_node, m);
        }

        update_height(node);
        int balance = get_balance(node);

        if (balance > 1 && (d < node->left->d || (d == node->left->d && v < node->left->v)))
            return right_rotate(node, m);
        if (balance < -1 && (d > node->right->d || (d == node->right->d && v > node->right->v)))
            return left_rotate(node, m);
        if (balance > 1 && (d > node->left->d || (d == node->left->d && v > node->left->v))) {
            node->left = left_rotate(node->left, m);
            return right_rotate(node, m);
        }
        if (balance < -1 && (d < node->right->d || (d == node->right->d && v < node->right->v))) {
            node->right = right_rotate(node->right, m);
            return left_rotate(node, m);
        }
        return node;
    }

    Node* min_value_node(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    Node* delete_node(Node* root, int v, double d, HeapMetrics& m) {
        if (!root) return root;

        if (d < root->d || (d == root->d && v < root->v)) {
            root->left = delete_node(root->left, v, d, m);
        } else if (d > root->d || (d == root->d && v > root->v)) {
            root->right = delete_node(root->right, v, d, m);
        } else {
            if ((!root->left) || (!root->right)) {
                Node* temp = root->left ? root->left : root->right;
                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else *root = *temp;
                delete temp;
            } else {
                Node* temp = min_value_node(root->right);
                root->v = temp->v;
                root->d = temp->d;
                root->right = delete_node(root->right, temp->v, temp->d, m);
            }
        }

        if (!root) return root;

        update_height(root);
        int balance = get_balance(root);

        if (balance > 1 && get_balance(root->left) >= 0)
            return right_rotate(root, m);
        if (balance > 1 && get_balance(root->left) < 0) {
            root->left = left_rotate(root->left, m);
            return right_rotate(root, m);
        }
        if (balance < -1 && get_balance(root->right) <= 0)
            return left_rotate(root, m);
        if (balance < -1 && get_balance(root->right) > 0) {
            root->right = right_rotate(root->right, m);
            return left_rotate(root, m);
        }
        return root;
    }

public:
    BalancedBST_PQ(int max_nodes) {
        root = nullptr;
        node_ptrs.assign(max_nodes + 1, nullptr);
        active_nodes = 0;
    }

    void insert(int v, double d, HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        Node* new_node = nullptr;
        root = insert_node(root, v, d, new_node, m);
        node_ptrs[v] = new_node;
        active_nodes++;
        m.insert_count++;
        auto end = chrono::high_resolution_clock::now();
        m.insert_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    int extract_min(HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        if (!root) return -1;
        Node* min_node = min_value_node(root);
        int ret_v = min_node->v;
        double min_d = min_node->d;
        
        root = delete_node(root, ret_v, min_d, m);
        node_ptrs[ret_v] = nullptr;
        active_nodes--;
        
        m.extract_min_count++;
        auto end = chrono::high_resolution_clock::now();
        m.extract_min_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        return ret_v;
    }

    void decrease_key(int v, double d, HeapMetrics& m) {
        auto start = chrono::high_resolution_clock::now();
        Node* node = node_ptrs[v];
        if (node && d < node->d) {
            double old_d = node->d;
            node_ptrs[v] = nullptr;
            root = delete_node(root, v, old_d, m);
            Node* new_node = nullptr;
            root = insert_node(root, v, d, new_node, m);
            node_ptrs[v] = new_node;
            m.decrease_key_count++;
        }
        auto end = chrono::high_resolution_clock::now();
        m.decrease_key_time_ns += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    bool empty() const {
        return root == nullptr;
    }

    void record_memory(HeapMetrics& m) {
        m.bytes_per_node = sizeof(Node);
        m.pointer_overhead = 24; // left, right, (maybe parent conceptually) => 24B
        m.peak_memory_bytes = active_nodes * sizeof(Node) + node_ptrs.capacity() * sizeof(Node*);
        m.heap_height = get_height(root);
    }
};

#endif
