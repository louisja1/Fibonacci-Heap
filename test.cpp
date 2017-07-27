#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "Fibheap.hpp"

using namespace std;

int main() {
    Fibheap H;
    H.insert(4);
    H.insert(1);
    H.insert(2);
    H.deleteMin();
    printf("%d\n", H.minimum());
    //puts("@@@@@@@@@@@");
    H.deleteMin();
    printf("%d\n", H.minimum());
    return 0;
}
