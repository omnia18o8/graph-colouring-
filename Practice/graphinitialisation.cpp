#include "graphinitialisation.h"
#include <fstream>
#include <iostream>
#include <unordered_set>

namespace GraphInitialisation {
    int V = 0, E = 0, Delta = 0;
    std::unordered_map<int, std::vector<int>> adj;

    std::vector<int> get_vertices(const std::string& filename) {
        std::unordered_set<int> vertex_set;
        std::ifstream infile(filename);
        int u, v;
        while (infile >> u >> v) {
            vertex_set.insert(u);
            vertex_set.insert(v);
        }
        infile.close();
        return std::vector<int>(vertex_set.begin(), vertex_set.end());
    }

    std::vector<std::pair<int, int>> get_edges(const std::string& filename) {
        std::vector<std::pair<int, int>> edges;
        std::ifstream infile(filename);
        int u, v;
        while (infile >> u >> v) {
            edges.emplace_back(u, v);
        }
        infile.close();
        return edges;
    }

    void getdelta(const std::string& filename) {
        adj.clear(); 
        Delta = 0;
        std::ifstream infile(filename);
        int u, v;
        while (infile >> u >> v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        infile.close();

        for (const auto& [node, neighbors] : adj) {
            int deg = neighbors.size();
            if (deg > Delta) Delta = deg;
        }
    }
}
