#pragma once
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>

namespace GraphInitialisation {
    extern int Delta;

    struct GraphData {
        std::vector<int> vertices;
        std::vector<std::pair<int, int>> edges;
    };

    GraphData initialise_graph(const std::string& filename);
}
