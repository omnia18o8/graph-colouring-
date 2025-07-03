#include <fstream>
#include <iostream>

//g++ -std=c++17 -Wall clique.cpp -o path
int main() {
    std::ofstream outfile("../clique.txt");
    const int n = 1000; 

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            outfile << i << " " << j << std::endl;
        }
    }

    outfile.close();
    return 0;
}
