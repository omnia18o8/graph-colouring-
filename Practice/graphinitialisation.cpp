#include "graphinitialisation.h"

#include "fstream"
#include <iostream>

namespace GraphInitialisation {
    int V = 0;
    int E = 0;
    int Delta = 0;

    std::unordered_map<int, std::vector<int>> adj; //hashmap for adjacency list, key is node and value is list of neighbours

    void getdelta(const std::string& filename, bool directed) {
        adj.clear(); 
        Delta = 0;

        std::ifstream infile(filename);
        if (!infile.is_open()) {
            std::cerr << "Error: Could not open file " << filename << "\n";
            return;
        }

        int u, v;
        while (infile >> u >> v) {
            adj[u].push_back(v);
            if (!directed) { // only assume undirected 
                adj[v].push_back(u);
            }
        }
        infile.close();

        for (const auto& [node, neighbors] : adj) {
            int deg = neighbors.size();
            if (deg > Delta) Delta = deg;
        }
    }
}