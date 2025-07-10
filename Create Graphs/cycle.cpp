#include <fstream>
#include <iostream>

int main() {
    int n = 1000; 

    std::ofstream outfile("../cycle.txt");
    if (!outfile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        int next = (i + 1) % n;
        outfile << i << " " << next << std::endl;
    }

    outfile.close();
    return 0;
}

// g++ -std=c++14 -Wall cycle.cpp -o cycle