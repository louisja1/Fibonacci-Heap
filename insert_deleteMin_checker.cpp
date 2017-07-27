#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "Fibheap.hpp"
#include <queue>
#include <ctime>

using namespace std;

int main() {
    srand(time(0));
    while (true) {
        Fibheap H;
        priority_queue<int, vector<int>, greater<int> > Q;
            int size = 0;
    for (int i = 0; i < 1000000; i++) {
        int type = rand() % 4;
        while (size == 0 && type == 0) type = rand() % 4;
        if (type) {
            int key = rand() % 2000000000 - 1000000000;
            heap_node * tmp = H.insert(key);
            Q.push(key);
            if (H.minimum() != Q.top()) {
                puts("fuck");
                return 0;
            }
            ++ size;
        } else {
            H.deleteMin();
            Q.pop();
            -- size;
            if (size > 0 && H.minimum() != Q.top()) {
                puts("fuck");
                return 0;
            }
        }
    }
    puts("passed");
}
    return 0;
}
