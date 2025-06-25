#include <fstream>
#include <iostream>

int main() {
    int n = 10; // Number of nodes in the cycle

    std::ofstream outfile("cycle.txt");
    if (!outfile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    // Add edges in a cycle: (0-1), (1-2), ..., (n-2)-(n-1), (n-1)-0
    for (int i = 0; i < n; ++i) {
        int next = (i + 1) % n;
        outfile << i << " " << next << std::endl;
    }

    outfile.close();
    std::cout << "Cycle graph with " << n << " nodes written to cycle_" << n << ".txt" << std::endl;
    return 0;
}

// g++ -std=c++14 -Wall cycle.cpp -o cycle