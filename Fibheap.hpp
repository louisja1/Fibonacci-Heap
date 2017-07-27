#ifndef FIBHEAP_HPP
#define FIBHEAP_HPP

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;

const int inf = 2000000000;

class heap_node {
public:
    friend class Fibheap;
    int key;
    int degree;
    heap_node * left;
    heap_node * right;
    heap_node * parent;
    heap_node * child;
    bool mark;
    heap_node(const int &_key) : key(_key) {
        degree = 0;
        left = right = this;
        mark = false;
        child = nullptr;
        parent = nullptr;
    }
    ~heap_node() {}
};
class Fibheap {
private:
    heap_node * root;
    int size;

    void clear(heap_node * x) {
        if (x == nullptr) return;
        heap_node * cur = x;
        do {
            heap_node * nxt = cur->right;
            clear(cur->child);
            delete cur;
            cur = nxt;
        } while (cur != x);
    }
    void insert(heap_node * cur) {
        if (root == nullptr) {
            root = cur;
            root->left = root->right = root;
        } else {
            root->left->right = cur;
            cur->left = root->left;
            cur->right = root;
            root->left = cur;
            if (cur->key < root->key) {
                root = cur;
            }
        }
    }
    void addNodeBehind(heap_node * cur, heap_node * node) {
        cur->right->left = node;
        node->right = cur->right;
        node->left = cur;
        cur->right = node;
    }
    void addChildToRootList(heap_node * cur) {
        heap_node * child = cur->child;
        if (child == nullptr) {
            return;
        }
        do {
            heap_node * nxt = child->right;
            child->parent = nullptr;
            addNodeBehind(root, child);
            child = nxt;
        } while (child != cur->child);
        cur->child = nullptr;
        cur->degree = 0;
    }
    void removeNodeFromRootList(heap_node * cur) {
        cur->left->right = cur->right;
        cur->right->left = cur->left;
    }
    void removeMinNode() {
        heap_node * pre = root;
        if (pre != nullptr) {
            addChildToRootList(pre);
            removeNodeFromRootList(pre);
            if (pre->right == pre) {
                root = nullptr;
            } else {
                root = pre->right; // don't care
                heap_node * tmp = root;
                consolidate();
            }
            delete pre;
            -- size;
        }
    }
    void addChild(heap_node * x, heap_node * y) {
        if (x->child == nullptr) {
            x->child = y;
            y->left = y->right = y;
        } else {
            addNodeBehind(x->child, y);
        }
        y->parent = x;
        y->mark = false;
        x->degree ++;
    }
    void fibHeapLink(heap_node * y, heap_node * x) {
        removeNodeFromRootList(y);
        addChild(x, y);
    }
    void consolidate() {
        int D = int(log(size * 1.0) / log(2.0)) + 1;
        heap_node * A[64] = {nullptr};
        heap_node * cur = root;
        do {
            heap_node * x = cur;
            heap_node * nxt = cur->right;
            int d = x->degree;
            while (A[d] != nullptr) {
                heap_node * y = A[d];
                if (x->key > y->key) {
                    heap_node * tmp = x;
                    x = y;
                    y = tmp;
                }
                fibHeapLink(y, x);
                A[d] = nullptr;
                d ++;
            }
            A[d] = x;
            cur = nxt;
        } while (cur != A[cur->degree] && cur->parent == nullptr);
        root = nullptr;
        for (int i = 0; i <= D; i++) {
            if (A[i] != nullptr) {
                insert(A[i]);
                if (root == nullptr || A[i]->key < root->key) {
                    root = A[i];
                }
            }
        }
    }
    void cut(heap_node * x, heap_node * y) {
        x->left->right = x->right;
        x->right->left = x->left;
        x->parent = nullptr;
        x->mark = false;
        y->degree --;
        if (y->degree == 0) {
            y->child = NULL;
        } else if (y->child == x) {
            y->child = x->right;
        }
        insert(x);
    }
    void cascadingCut(heap_node * y) {
        heap_node * z = y->parent;
        if (z != nullptr) {
            if (y->mark == false) {
                y->mark = true;
            } else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }
public:
    Fibheap() {
        root = nullptr;
        size = 0;
    }
    ~Fibheap() {
        clear(root);
    }
    heap_node * insert(int key) {
        ++ size;
        heap_node * cur = new heap_node(key);
        insert(cur);
        return cur;
    }
    int minimum() {
        return root->key;
    }
    void deleteMin() {
        removeMinNode();
    }
    void deleteNode(heap_node * cur) {
        decrease(cur, -inf);
        deleteMin();
    }
    void decrease(heap_node * x, int val) {
        if (val > x->key) {
            return;
        }
        x->key = val;
        heap_node * y = x->parent;
        if (y != nullptr && x->key < y->key) {
            cut(x, y);
            cascadingCut(y);
        }
        if (x->key < root->key) {
            root = x;
        }
    }
    void merge(Fibheap & oth) {
        heap_node * root2 = oth.root;
        if (root == nullptr) {
            root = root2;
            return;
        }
        if (root2 == nullptr) {
            return;
        }
        heap_node * right1 = root->right;
        heap_node * left2 = root2->left;
        root->right = root2;
        root2->left = root;
        left2->right = right1;
        right1->left = left2;
        if (root2->key < root->key) {
            root = root2;
        }
        size += oth.size;
        oth.root = nullptr;
    }
};

#endif
