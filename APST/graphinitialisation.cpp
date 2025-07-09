#include "graphinitialisation.h"
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

namespace GraphInitialisation {
    int Delta = 0;

    GraphData initialise_graph(const std::string& filename) {
        std::ifstream infile(filename);
        std::string line;

        std::set<std::pair<int, int>> unique_edges;
        std::vector<int> all_vertices;
        std::unordered_map<int, int> degree;

        while (std::getline(infile, line)) {
            for (char& c : line) if (c == ',') c = ' ';

            std::istringstream iss(line);
            int u, v;
            if (!(iss >> u >> v)) continue;

            int x = std::min(u, v);
            int y = std::max(u, v);
            if (x == y) continue; // ignore self-loops

            unique_edges.emplace(x, y);
            all_vertices.push_back(u);
            all_vertices.push_back(v);

            int du = ++degree[u];
            int dv = ++degree[v];
            Delta = std::max({Delta, du, dv});
        }

        std::sort(all_vertices.begin(), all_vertices.end());
        all_vertices.erase(std::unique(all_vertices.begin(), all_vertices.end()), all_vertices.end());

        // Convert set to vector
        std::vector<std::pair<int, int>> edges(unique_edges.begin(), unique_edges.end());

        return {std::move(all_vertices), std::move(edges)};
    }
}
