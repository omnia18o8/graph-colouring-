#include <fstream>
#include <iostream>

//g++ -std=c++17 -Wall path.cpp -o path  
int main() {
    std::ofstream outfile("../path.txt");
    if (!outfile) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    int n = 10000; 

    for (int i = 0; i < n; ++i) {
        outfile << i << " " << (i + 1) << std::endl;
    }
    outfile.close();
    return 0;
}
