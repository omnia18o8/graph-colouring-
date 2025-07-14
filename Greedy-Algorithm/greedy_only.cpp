#include "greedy_only.h"
#include "../APST/graphinitialisation.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <iostream>

void fisher_yates_shuffle1(std::vector<int>& vec) {
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(vec[i], vec[j]);
    }
}

GreedyResult greedy(const std::string& file) {
    using clock = std::chrono::high_resolution_clock;
    srand(time(0)); 

    auto t_start = clock::now();

    auto gdata = GraphInitialisation::initialise_graph(file);
    std::vector<int> vertices = std::move(gdata.vertices);
    std::vector<std::pair<int, int>>& edges = gdata.edges;
    int Delta = GraphInitialisation::Delta;

    // Fisher-Yates shuffle
    fisher_yates_shuffle1(vertices);

    std::unordered_map<int, std::vector<int>> adj;
    for (int v : vertices) adj[v] = {};
    for (auto [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

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
            colouring[v] = -1;
        } else {
            colouring[v] = chosen;
        }
    }

    std::unordered_set<int> used_colours;
    for (const auto& [v, c] : colouring)
        if (c != -1) used_colours.insert(c);

    auto t_end = clock::now();
    std::chrono::duration<double> total_time = t_end - t_start;

    // Validation: check if colouring is proper
    bool proper = true;
    for (const auto& v : vertices) {
        if (!colouring.count(v) || colouring[v] <= 0) {
            proper = false;
        }
    }
    // Print every conflict
    for (const auto& [u, v] : edges) {
        if (colouring.count(u) && colouring.count(v) && colouring[u] == colouring[v]) {
            proper = false;
        }
    }


    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    size_t memory_bytes = usage.ru_maxrss; // macOS: bytes
    double memory_gb = memory_bytes / (1024.0 * 1024.0);


for (const auto& [u, v] : edges) {
    int col_u = colouring.count(u) ? colouring[u] : -1;
    int col_v = colouring.count(v) ? colouring[v] : -1;
    std::cout << "Edge (" << u << ", " << v << "): Colour[" << u << "]=" << col_u 
              << ", Colour[" << v << "]=" << col_v;
    if (col_u == col_v && col_u != -1) {
        std::cout << "  <-- Conflict";
    }
    std::cout << '\n';
}


    GreedyResult result;
    result.filename = file;
    result.nodes = vertices.size();
    result.delta = Delta;
    result.constant = Delta + 1; 
    result.colours_used = used_colours.size();
    result.total_time = total_time.count();
    result.total_memory = memory_gb;
    result.avg_list_size = Delta + 1;
    result.conflict_edges = 0;
    result.success = proper;
    return result;
}

int main() {
    std::string file = "../Graphs/loc-gowalla_edges.txt";
    GreedyResult result = greedy(file);
    std::cout << "Number of colours used: " << result.colours_used << std::endl;
    return 0;
}

//Run using: 
// g++ -std=c++17 -O2 -Wall -Wextra greedy_only.cpp ../APST/graphinitialisation.cpp -I../APST -o greedy_only
