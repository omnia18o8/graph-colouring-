#include <iostream>

int main() {
    int center = 0;             // central hub
    int leaf_count = 1000;      // number of outer nodes

    // Star connections
    for (int i = 1; i <= leaf_count; ++i) {
        std::cout << center << " " << i << "\n";
    }

    // Cycle on leaves
    for (int i = 1; i <= leaf_count; ++i) {
        int next = (i % leaf_count) + 1;
        std::cout << i << " " << next << "\n";
    }

    // Path on leaves
    for (int i = 1; i < leaf_count; ++i) {
        std::cout << i << " " << i + 1 << "\n";
    }

    return 0;
}

// g++ -std=c++14 -O2 star.cpp -o gen
// ./gen > star.txt