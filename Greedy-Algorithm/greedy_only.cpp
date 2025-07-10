#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sys/resource.h> // For memory measurement
#include <string>
#include <algorithm>
#include <numeric>

#include "../APST/graphinitialisation.h"

int main() {
    using clock = std::chrono::high_resolution_clock;
    auto t_start = clock::now();

    // --- 1. Load graph ---
    std::string file = "../Graphs/large_twitch_edges.csv";  // <<<< Set your input graph here
    auto gdata = GraphInitialisation::initialise_graph(file);
    std::vector<int> vertices = std::move(gdata.vertices);
    std::vector<std::pair<int, int>>& edges = gdata.edges;
    int Delta = GraphInitialisation::Delta;

    // --- 2. Build adjacency list ---
    std::unordered_map<int, std::vector<int>> adj;
    for (int v : vertices) adj[v] = {};
    for (const auto& edge : edges) {
        int u = edge.first, v = edge.second;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // --- 3. Greedy coloring ---
    std::unordered_map<int, int> colouring;
    for (int v : vertices) colouring[v] = -1;

    for (int v : vertices) {
        std::unordered_set<int> forbidden;
        for (int u : adj[v]) {
            int cu = colouring[u];
            if (cu >= 1 && cu <= Delta + 1) forbidden.insert(cu);
        }
        int chosen = 1;
        while (forbidden.count(chosen)) ++chosen;
        if (chosen > Delta + 1) {
            std::cerr << "No available colour for vertex " << v << ".\n";
            colouring[v] = -1;
        } else {
            colouring[v] = chosen;
        }
    }

    // --- 4. Count used colours ---
    std::unordered_set<int> used_colours;
    for (const auto& [v, c] : colouring)
        if (c != -1) used_colours.insert(c);

    // --- 5. Timing and memory ---
    auto t_end = clock::now();
    std::chrono::duration<double> total_time = t_end - t_start;

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    size_t memory = usage.ru_maxrss; // KB on Linux

    // --- 6. Output to ../Plot/greedy_results.csv ---
    std::string out_csv = "../Plot/greedy_results.csv";
    double constant = Delta + 1;          // Palette size
    double avg_list_size = Delta + 1;     // All lists are Delta+1
    int conflict_edges = 0;               // Not relevant for greedy

    std::ofstream out(out_csv, std::ios::app); // append mode
    out << file << ","
        << vertices.size() << ","
        << Delta << ","
        << constant << ","
        << used_colours.size() << ","
        << total_time.count() << ","
        << (memory * 1024) << "," // bytes
        << avg_list_size << ","
        << conflict_edges << "\n";
    out.close();

    return 0;
}
