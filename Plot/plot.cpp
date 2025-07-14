#include "../APST/apst.h"
#include "../Greedy-Algorithm/greedy_only.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem> 

int main() {
    std::string file = "../Graphs/large_twitch_edges.csv";
    std::string out_txt_path = "../Plot/results.txt"; 

    // Greedy algorithm 
    GreedyResult greedy_result = greedy(file);
    std::cout << "Greedy: colours used = " << greedy_result.colours_used 
              << ", success = " << (greedy_result.success ? "Yes" : "No") << std::endl;

    // APST algorithm for all constants
    std::vector<APSTResult> apst_results;
    for (int c = 30; c <= 40; ++c) {
        double constant = static_cast<double>(c);
        APSTResult result = apst(file, constant);
        apst_results.push_back(result);
    }

    // Write results table
    std::ofstream out(out_txt_path, std::ios::app);
    out << "filename constant nodes delta success apst_colours apst_time apst_memory greedy_colours greedy_time greedy_memory\n";
    std::string shortfile = file.substr(file.find_last_of("/\\") + 1);
    out << std::fixed << std::setprecision(4);

    for (const auto& result : apst_results) {
        out << shortfile << ' '
            << result.constant << ' '
            << result.nodes << ' '
            << result.delta << ' '
            << result.success << ' '
            << result.colours_used << ' '
            << result.total_time << ' '
            << result.total_memory << ' '
            << greedy_result.colours_used << ' '
            << greedy_result.total_time << ' '
            << greedy_result.total_memory << '\n';
    }

    out.close();
    return 0;
}
