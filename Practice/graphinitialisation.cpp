#include <vector>
#include <unordered_map>
#include <set>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <unordered_set>

namespace GraphInitialisation {
    int V = 0, E = 0, Delta = 0;

    struct GraphData {
        std::vector<int> vertices;
        std::vector<std::pair<int, int>> edges;
    };

    GraphData initialise_graph(const std::string& filename) {
        std::unordered_set<int> vertex_set;
        std::vector<std::pair<int, int>> edges;
        std::unordered_map<int, int> degree;


        std::ifstream infile(filename);
        std::string line; 

        while (std::getline(infile, line)) {
            for (char& c : line) if (c == ',') c = ' '; // Replace commas with spaces
            std::istringstream iss(line); 
            int u, v;
            if (!(iss >> u >> v)) continue; // Skip lines that do not contain two integers
            edges.emplace_back(u, v);

            vertex_set.insert(u);
            vertex_set.insert(v);

            int du = ++degree[u];
            int dv = ++degree[v];
            if (du > Delta) Delta = du;
            if (dv > Delta) Delta = dv;
        }

        V = vertex_set.size();
        E = edges.size();
        std::vector<int> vertices(vertex_set.begin(), vertex_set.end());
        return {std::move(vertices), std::move(edges)};
    }
}
