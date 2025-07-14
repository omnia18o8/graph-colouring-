#include <iostream>
#include <vector>

int main() {
    int num_U = 1000;
    int num_V = 1000; 
    int center_node = num_U + num_V; 

 
    for (int u = 0; u < num_U; ++u) {
        for (int v = num_U; v < num_U + num_V; ++v) {
            std::cout << u << " " << v << "\n";
        }
    }

    for (int u = 0; u < num_U + num_V; ++u) {
        std::cout << center_node << " " << u << "\n";
    }

    return 0;
}

//g++ -std=c++17 -O2 biparite_extreme.cpp -o gen
// ./gen > bipartite_extreme.txt
// This code generates a bipartite graph with 1000 nodes in each partition and a center node connected to all other nodes.
