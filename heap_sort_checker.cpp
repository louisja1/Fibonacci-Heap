#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "Fibheap.hpp"
#include <ctime>

using namespace std;

const int N = 10000005;

int a[N];

int main() {
    srand(time(0));
    while (true) {
        Fibheap H;
        int n = 1000000;
        for (int i = 0; i < n; i++) {
            a[i] = rand() % 2000000000 - 1000000000;
            H.insert(a[i]);
        }
        sort(a, a + n);
        for (int i = 0; i < n; i++) {
            if (H.minimum() != a[i]) {
                puts("fuck");
                return 0;
            }
            H.deleteMin();
        }
        puts("passed");
    }
    return 0;
}
