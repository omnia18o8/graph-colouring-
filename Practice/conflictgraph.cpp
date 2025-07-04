#include "conflictgraph.h"

std::set<std::pair<int, int>> StreamingConflictGraph::conflict_graph(
    const std::vector<std::pair<int, int>>& edges,
    const std::unordered_map<int, std::vector<int>>& colour_lists
) const {
    std::set<std::pair<int, int>> conflict_edges;
    for (std::size_t i = 0; i < edges.size(); ++i) {
        int u = edges[i].first;
        int v = edges[i].second;
        int x = std::min(u, v), y = std::max(u, v); // Ensure x < y for consistency

        bool u_full = (colour_lists.at(u).size() == 1 && colour_lists.at(u)[0] == 0);
        bool v_full = (colour_lists.at(v).size() == 1 && colour_lists.at(v)[0] == 0);
        if (u_full || v_full) {
            conflict_edges.emplace(x, y); //use emplace to avoid unnecessary copying
            continue;
        }

    const std::vector<int>& Lu = colour_lists.at(u);
    const std::vector<int>& Lv = colour_lists.at(v);
    std::size_t index_u = 0, index_v = 0;
    bool conflict = false;

    while (index_u < Lu.size() && index_v < Lv.size()) {
        if (Lu[index_u] < Lv[index_v]) ++index_u;
        else if (Lu[index_u] > Lv[index_v]) ++index_v;
        else {
            conflict = true;
            break;
        }
    }
        if (conflict) {
            conflict_edges.emplace(x, y);
        }
    }
    return conflict_edges;
}
