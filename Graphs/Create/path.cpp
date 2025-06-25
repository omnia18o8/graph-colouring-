#include <fstream>
#include <iostream>

//g++ -std=c++17 -Wall path.cpp -o path  
int main() {
    std::ofstream outfile("/Users/omniaali/Documents/Dissertation/Code/Dissertation/Graphs/path.txt");
    if (!outfile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }
    // Generate path graph edges
    for (int i = 0; i < 99999; ++i) {
        outfile << i << " " << (i + 1) << std::endl;
    }
    outfile.close();
    return 0;
}
