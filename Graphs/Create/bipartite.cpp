#include <fstream>
#include <iostream>

int main() {
    const int m = 1000; 
    const int n = 1000;
    std::ofstream outfile("../bipartite.txt");
    if (!outfile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    // Nodes 0 to m-1 in part A, m to m+n-1 in part B
    for (int u = 0; u < m; ++u) {
        for (int v = m; v < m + n; ++v) {
            outfile << u << " " << v << std::endl;
        }
    }

    outfile.close();
    return 0;
}
// g++ -std=c++14 -Wall bipartite.cpp -o bipartite