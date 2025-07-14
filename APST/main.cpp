#include "apst.h"
#include <iostream>
#include <vector>
#include <unordered_map>


int main() {
    std::string file = "../Graphs/loc-gowalla_edges.txt"; 
    double constant = 40.0;
    APSTResult result = apst(file, constant);

    std::cout << "Filename: " << result.filename << std::endl;
    std::cout << "Nodes: " << result.nodes << std::endl;
    std::cout << "Delta: " << result.delta << std::endl;
    std::cout << "Constant: " << result.constant << std::endl;
    std::cout << "Success: " << (result.success ? "Yes" : "No") << std::endl;
    std::cout << "Colours Used: " << result.colours_used << std::endl;
    std::cout << "Total Time: " << result.total_time << " seconds" << std::endl;
    std::cout << "Total Memory: " << result.total_memory << " MB" << std::endl;
    std::cout << "Average List Size: " << result.avg_list_size << std::endl;
    std::cout << "Conflict Edges: " << result.conflict_edges << std::endl;


    return 0;
}
