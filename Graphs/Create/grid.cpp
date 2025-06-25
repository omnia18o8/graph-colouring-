#include <fstream>
#include <iostream>

int main() {
    int m = 5; // number of rows
    int n = 5; // number of columns

    std::ofstream outfile("grid.txt");
    if (!outfile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    // Node numbering: (row, col) => node = row * n + col
    for (int row = 0; row < m; ++row) {
        for (int col = 0; col < n; ++col) {
            int node = row * n + col;
            // Connect to right neighbor
            if (col + 1 < n) {
                int right = row * n + (col + 1);
                outfile << node << " " << right << std::endl;
            }
            // Connect to down neighbor
            if (row + 1 < m) {
                int down = (row + 1) * n + col;
                outfile << node << " " << down << std::endl;
            }
        }
    }

    outfile.close();
    std::cout << "Grid graph (" << m << "x" << n << ") written to grid_" << m << "_" << n << ".txt" << std::endl;
    return 0;
}

// g++ -std=c++14 -Wall grid.cpp -o grid