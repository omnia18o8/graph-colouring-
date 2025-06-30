#include <unordered_set>
#include <set>
#include <vector>
#include <utility>
#include <algorithm>

#include "conflictgraph.h"

std::set<std::pair<int, int>> StreamingConflictGraph::build_conflict_graph_from_edges(
    const std::vector<std::pair<int, int>>& edges,
    const std::unordered_map<int, std::vector<int>>& colour_lists
) const {
    constexpr int THRESHOLD = 16; // Tune as needed

    // Precompute palette sets for large lists
    std::unordered_map<int, std::unordered_set<int>> palette_sets;
    for (const auto& [v, palette] : colour_lists) {
        if (palette.size() >= THRESHOLD) {
            palette_sets[v] = std::unordered_set<int>(palette.begin(), palette.end());
        }
    }

    std::set<std::pair<int, int>> conflict_edges;
    for (const auto& [u, v] : edges) {
        if (u == v) continue;
        const auto& Lu = colour_lists.at(u);
        const auto& Lv = colour_lists.at(v);
        bool u_big = palette_sets.count(u) > 0;
        bool v_big = palette_sets.count(v) > 0;

        bool conflict = false;
        if (!u_big && !v_big) {
            // Both small: linear scan
            for (int c : Lu) {
                if (std::find(Lv.begin(), Lv.end(), c) != Lv.end()) {
                    conflict = true;
                    break;
                }
            }
        } else if (u_big && !v_big) {
            // Lu is set, Lv is vector
            const auto& set_Lu = palette_sets.at(u);
            for (int c : Lv) {
                if (set_Lu.count(c)) {
                    conflict = true;
                    break;
                }
            }
        } else if (!u_big && v_big) {
            // Lv is set, Lu is vector
            const auto& set_Lv = palette_sets.at(v);
            for (int c : Lu) {
                if (set_Lv.count(c)) {
                    conflict = true;
                    break;
                }
            }
        } else {
            // Both are sets, iterate over smaller
            const auto& set_Lu = palette_sets.at(u);
            const auto& set_Lv = palette_sets.at(v);
            if (set_Lu.size() < set_Lv.size()) {
                for (int c : set_Lu) {
                    if (set_Lv.count(c)) {
                        conflict = true;
                        break;
                    }
                }
            } else {
                for (int c : set_Lv) {
                    if (set_Lu.count(c)) {
                        conflict = true;
                        break;
                    }
                }
            }
        }

        if (conflict) {
            int x = std::min(u, v), y = std::max(u, v);
            conflict_edges.emplace(x, y);
        }
    }
    return conflict_edges;
}
