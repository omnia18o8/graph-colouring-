#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <iostream>
#include <numeric> // for std::iota

#include "../APST/graphinitialisation.h"
#include "../APST/output.h" // for getCurrentRSS()

int main() {
    using clock = std::chrono::high_resolution_clock;
    auto t_start = clock::now();
    size_t mem_start = getCurrentRSS();

    // Step 1: Load graph
    std::string file = "../Graphs/clique.txt";
    auto gdata = GraphInitialisation::initialise_graph(file);
    std::vector<int> vertices = std::move(gdata.vertices);
    std::vector<std::pair<int, int>>& edges = gdata.edges;
    int Delta = GraphInitialisation::Delta;

    auto t_init_end = clock::now();
    size_t mem_after_init = getCurrentRSS();

    // Step 2: Build adjacency list
    std::unordered_map<int, std::vector<int>> adj;
    for (int v : vertices) adj[v] = {};
    for (auto [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    auto t_adj_end = clock::now();
    size_t mem_after_adj = getCurrentRSS();

    // Step 3: Greedy colouring using palette 1..Delta+1
    std::unordered_map<int, int> colouring;
    for (int v : vertices) colouring[v] = -1; // uncoloured

    bool proper = true;
    for (int v : vertices) {
        // Collect forbidden colours assigned to neighbours
        std::unordered_set<int> forbidden;
        for (int u : adj[v]) {
            int cu = colouring[u];
            if (cu >= 1 && cu <= Delta + 1) forbidden.insert(cu);
        }
        // Find smallest allowed colour in 1..Delta+1
        int colour = 1;
        for (; colour <= Delta + 1; ++colour) {
            if (forbidden.find(colour) == forbidden.end()) break;
        }
        if (colour > Delta + 1) {
            std::cerr << "❌ No available colour for vertex " << v << " within palette 1.." << (Delta + 1) << ".\n";
            proper = false;
            colouring[v] = -1;
        } else {
            colouring[v] = colour;
        }
    }

    auto t_colour_end = clock::now();
    size_t mem_after_colour = getCurrentRSS();

    // Step 4: Validation
    for (const auto& [v, c] : colouring) {
        if (c == -1) {
            std::cerr << "❌ Vertex " << v << " was not assigned a colour.\n";
            proper = false;
        }
    }
    for (auto [u, v] : edges) {
        if (colouring[u] == colouring[v] && colouring[u] != -1) {
            std::cerr << "❌ Invalid colouring: edge (" << u << ", " << v << ") has same colour " << colouring[u] << "\n";
            proper = false;
        }
    }

    std::unordered_set<int> used_colours;
    for (const auto& [v, c] : colouring)
        if (c != -1) used_colours.insert(c);

    auto t_end = clock::now();
    size_t mem_end = getCurrentRSS();

    // Step 5: Output
    std::cout << (proper ? "✔️  Proper colouring confirmed.\n" : "❌ Colouring failed.\n");
    std::cout << "Vertices coloured: " << colouring.size() << "\n";
    std::cout << "Delta (max degree): " << Delta << "\n";
    std::cout << "Colours used: " << used_colours.size() << "\n\n";

    std::cout << "=== Time (s) ===\n";
    std::cout << "Initialisation:     " << std::chrono::duration<double>(t_init_end - t_start).count() << "\n";
    std::cout << "Adjacency build:    " << std::chrono::duration<double>(t_adj_end - t_init_end).count() << "\n";
    std::cout << "Greedy colouring:   " << std::chrono::duration<double>(t_colour_end - t_adj_end).count() << "\n";
    std::cout << "Total runtime:      " << std::chrono::duration<double>(t_end - t_start).count() << "\n\n";

    std::cout << "=== Memory (MB) ===\n";
    std::cout << "Start:              " << mem_start / 1024.0 / 1024.0 << "\n";
    std::cout << "Post-init:          " << mem_after_init / 1024.0 / 1024.0 << "\n";
    std::cout << "Post-adjacency:     " << mem_after_adj / 1024.0 / 1024.0 << "\n";
    std::cout << "Post-colouring:     " << mem_after_colour / 1024.0 / 1024.0 << "\n";
    std::cout << "End:                " << mem_end / 1024.0 / 1024.0 << "\n";

    // Optional: print each vertex and its colour
    /*
    for (const auto& [v, c] : colouring) {
        std::cout << v << " " << c << "\n";
    }
    */

    return 0;
}
