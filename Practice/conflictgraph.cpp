#include "conflictgraph.h"
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

StreamingConflictGraph::StreamingConflictGraph() {}

void StreamingConflictGraph::conflictsubgraphs(
    int u, int v,
    const std::unordered_map<int, std::vector<int>>& colour_lists
) {
    const auto& Lu = colour_lists.at(u);
    const auto& Lv = colour_lists.at(v);

    std::unordered_set<int> Lu_set(Lu.begin(), Lu.end());

    for (int colour : Lv) {
        if (Lu_set.count(colour) && u != v) { // Ignore self-loops
            int x = std::min(u, v);
            int y = std::max(u, v);
            sketch[colour].emplace(x, y);
        }
    }
}

std::set<std::pair<int, int>> StreamingConflictGraph::conflictgraph() const {
    std::set<std::pair<int, int>> conflict_edges;

    for (const auto& colour_entry : sketch) { // Gather all conflict edges across all colours
        const auto& edges = colour_entry.second;
        conflict_edges.insert(edges.begin(), edges.end());
    }
    return conflict_edges;
}
