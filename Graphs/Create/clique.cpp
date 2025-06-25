#include <fstream>
#include <iostream>

//g++ -std=c++17 -Wall clique.cpp -o path
int main() {
    std::ofstream outfile("clique.txt");
    if (!outfile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    const int n = 100; // Number of nodes
    // For each unique unordered pair (i, j) with i < j
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            outfile << i << " " << j << std::endl;
        }
    }

    outfile.close();
    return 0;
}
