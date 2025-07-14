#include "conflictgraph.h"
#include "graphinitialisation.h"
#include <set>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <chrono>

std::set<std::pair<int, int>> StreamingConflictGraph::conflict_graph(
    const std::vector<std::pair<int, int>>& edges,
    const std::unordered_map<int, std::vector<int>>& colour_lists
) const {

    std::set<std::pair<int, int>> conflict_edges;
    int palette_size = GraphInitialisation::Delta + 1;


    for (const auto& [u, v] : edges) {
        int x = std::min(u, v), y = std::max(u, v);
        const auto& Lu = colour_lists.at(u);
        const auto& Lv = colour_lists.at(v);

        // Shortcut 1: Full palette
        if ((Lu.size() == 1 && Lu[0] == 0) || (Lv.size() == 1 && Lv[0] == 0)) {
            conflict_edges.emplace(x, y);
            continue;
        }

        // Shortcut 2: Sum of palette sizes exceeds Delta + 1
        if (Lu.size() + Lv.size() > static_cast<std::size_t>(palette_size)) {
            conflict_edges.emplace(x, y);
            continue;
        }

        std::size_t i = 0, j = 0;
        while (i < Lu.size() && j < Lv.size()) {
            if (Lu[i] < Lv[j]) ++i;
            else if (Lu[i] > Lv[j]) ++j;
            else {
                conflict_edges.emplace(x, y);
                break;
            }
        }
    }

    return conflict_edges;
}
