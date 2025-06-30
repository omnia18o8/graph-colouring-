#include "graphinitialisation.h"
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

namespace GraphInitialisation {
    int V = 0, E = 0, Delta = 0;
    std::unordered_map<int, std::vector<int>> adj;

    void replace_commas_with_space(std::string& line) {
        for (char& c : line)
            if (c == ',') c = ' ';
    }

    std::vector<std::pair<int, int>> load_edge_list(const std::string& filename) {
        std::vector<std::pair<int, int>> edges;
        std::ifstream infile(filename);
        std::string line;
        while (std::getline(infile, line)) {
            replace_commas_with_space(line);
            std::istringstream iss(line);
            int u, v;
            if (iss >> u >> v)
                edges.emplace_back(u, v);
        }
        return edges;
    }

    std::vector<int> get_vertices(const std::string& filename) {
        std::unordered_set<int> vertex_set;
        for (const auto& edge : load_edge_list(filename)) {
            vertex_set.insert(edge.first);
            vertex_set.insert(edge.second);
        }
        return std::vector<int>(vertex_set.begin(), vertex_set.end());
    }

    std::vector<std::pair<int, int>> get_edges(const std::string& filename) {
        return load_edge_list(filename);
    }

    void getdelta(const std::string& filename) {
        adj.clear(); 
        Delta = 0;
        for (const auto& edge : load_edge_list(filename)) {
            int u = edge.first, v = edge.second;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        for (const auto& [node, neighbors] : adj) {
            int deg = neighbors.size();
            if (deg > Delta) Delta = deg;
        }
    }
}
