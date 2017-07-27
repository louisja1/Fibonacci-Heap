//poj-1502
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>

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
    int id;
    bool mark;
    heap_node(const int &_key, const int &_id) : key(_key), id(_id) {
        degree = 0;
        left = right = this;
        mark = false;
        child = NULL;
        parent = NULL;
    }
    ~heap_node() {}
};
class Fibheap {
private:
    heap_node * root;
    int size;

    void clear(heap_node * x) {
        if (x == NULL) return;
        heap_node * cur = x;
        do {
            heap_node * nxt = cur->right;
            clear(cur->child);
            delete cur;
            cur = nxt;
        } while (cur != x);
    }
    void insert(heap_node * cur) {
        if (root == NULL) {
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
        if (child == NULL) {
            return;
        }
        do {
            heap_node * nxt = child->right;
            child->parent = NULL;
            addNodeBehind(root, child);
            child = nxt;
        } while (child != cur->child);
        cur->child = NULL;
        cur->degree = 0;
    }
    void removeNodeFromRootList(heap_node * cur) {
        cur->left->right = cur->right;
        cur->right->left = cur->left;
    }
    void removeMinNode() {
        heap_node * pre = root;
        if (pre != NULL) {
            addChildToRootList(pre);
            removeNodeFromRootList(pre);
            if (pre->right == pre) {
                root = NULL;
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
        if (x->child == NULL) {
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
        heap_node * A[64] = {NULL};
        heap_node * cur = root;
        do {
            heap_node * x = cur;
            heap_node * nxt = cur->right;
            int d = x->degree;
            while (A[d] != NULL) {
                heap_node * y = A[d];
                if (x->key > y->key) {
                    heap_node * tmp = x;
                    x = y;
                    y = tmp;
                }
                fibHeapLink(y, x);
                A[d] = NULL;
                d ++;
            }
            A[d] = x;
            cur = nxt;
        } while (cur != A[cur->degree] && cur->parent == NULL);
        root = NULL;
        for (int i = 0; i <= D; i++) {
            if (A[i] != NULL) {
                insert(A[i]);
                if (root == NULL || A[i]->key < root->key) {
                    root = A[i];
                }
            }
        }
    }
    void cut(heap_node * x, heap_node * y) {
        x->left->right = x->right;
        x->right->left = x->left;
        x->parent = NULL;
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
        if (z != NULL) {
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
        root = NULL;
        size = 0;
    }
    ~Fibheap() {
        clear(root);
    }
    bool empty() {
        return size == 0;
    }
    heap_node * insert(int key, int id) {
        ++ size;
        heap_node * cur = new heap_node(key, id);
        insert(cur);
        return cur;
    }
    int minimum() {
        return root->key;
    }
    int minimum_id() {
        return root->id;
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
        if (y != NULL && x->key < y->key) {
            cut(x, y);
            cascadingCut(y);
        }
        if (x->key < root->key) {
            root = x;
        }
    }
    void merge(Fibheap & oth) {
        heap_node * root2 = oth.root;
        if (root == NULL) {
            root = root2;
            return;
        }
        if (root2 == NULL) {
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
    }
};

const int N = 105;

struct edge {
    int adj, len;
    edge() {}
    edge(int _adj, int _len) {
        adj = _adj;
        len = _len;
    }
};

vector<edge> G[N];
int dis[N];
heap_node * a[N];
int n;

void dijkstra() {
    Fibheap H;
    for (int i = 1; i <= n; i++) {
        dis[i] = inf;
    }
    dis[1] = 0;
    for (int i = 1; i <= n; i++) {
        a[i] = H.insert(dis[i], i);
    }
    while (!H.empty()) {
        int cur = H.minimum_id();
        int key = H.minimum();
        H.deleteMin();
        for (int i = 0; i < G[cur].size(); i++) {
            int adj = G[cur][i].adj;
            int len = G[cur][i].len;
            if (key + len < dis[adj]) {
                dis[adj] = key + len;
                H.decrease(a[adj], dis[adj]);
            }
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i++)
        if (dis[i] > ans) ans = dis[i];
    printf("%d\n", ans);
}

int main() {
    while (scanf("%d", &n) == 1) {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j < i; j++) {
                char s[10];
                scanf("%s", s);
                if (strcmp(s, "x") == 0) {
                    continue;
                }
                int tmp = 0;
                for (int i = 0; i < strlen(s); i++)
                    tmp = tmp * 10 + s[i] - '0';
                G[i].push_back(edge(j, tmp));
                G[j].push_back(edge(i, tmp));
            }
        dijkstra();
    }
    return 0;
}
