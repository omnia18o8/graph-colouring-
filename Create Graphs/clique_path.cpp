#include <iostream>

int main() {
    int clique_size = 1000;     // number of nodes in the clique
    int path_length = 1000;     // number of nodes in the path

    // Create the clique: nodes 0 to clique_size - 1
    for (int u = 0; u < clique_size; ++u) {
        for (int v = u + 1; v < clique_size; ++v) {
            std::cout << u << " " << v << "\n";
        }
    }

    // Create the path: nodes clique_size to clique_size + path_length - 1
    int offset = clique_size;
    for (int i = 0; i < path_length - 1; ++i) {
        int u = offset + i;
        int v = offset + i + 1;
        std::cout << u << " " << v << "\n";
    }

    // Connect the first clique node to the start of the path
    std::cout << 0 << " " << offset << "\n";

    return 0;
}

// g++ -std=c++14 -O2 clique_path.cpp -o gen
// ./gen > clique_path.txt