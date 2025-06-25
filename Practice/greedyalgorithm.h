#ifndef GREEDYALGORITHM_H
#define GREEDYALGORITHM_H

#include <vector>
#include <unordered_map>
#include <set>

std::unordered_map<int, int> greedy_colour_conflict_graph(
    const std::vector<int>& vertices,
    const std::unordered_map<int, int>& pi,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    const std::set<std::pair<int, int>>& conflict_edges
);

void colour_non_conflict_vertices(
    const std::vector<int>& all_vertices,
    const std::unordered_map<int, std::vector<int>>& conflict_adj,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    std::unordered_map<int, int>& colouring
);

std::unordered_map<int, std::vector<int>> build_conflict_adj_from_edges(
    const std::vector<int>& vertices,
    const std::set<std::pair<int, int>>& conflict_edges
);

#endif // GREEDYALGORITHM_H
