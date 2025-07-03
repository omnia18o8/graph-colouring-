#pragma once
#include <vector>
#include <string>
#include <utility>

namespace GraphInitialisation {
    extern int V, E, Delta;

    struct GraphData {
        std::vector<int> vertices; 
        std::vector<std::pair<int, int>> edges;
    };

    GraphData initialise_graph(const std::string& filename);
}
