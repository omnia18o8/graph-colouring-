#include "apst.h"
#include <iomanip>
#include <vector>

int main() {
    std::string file = "../Graphs/large_twitch_edges.csv";

    std::vector<APSTResult> results;
    for (int c = 1; c <= 40; ++c) {
        double constant = static_cast<double>(c);
        APSTResult result = apst(file, constant);
        save_apst_result_csv(result, "../Plot/apst_results.csv");
        results.push_back(result);
    }
}
