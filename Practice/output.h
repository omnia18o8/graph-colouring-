#pragma once
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

// No PaletteBitset here
size_t getCurrentRSS();

void print_graph_stats(
    const std::string& input_graph_file,
    const std::vector<int>& vertices,
    const std::vector<std::pair<int, int>>& edges,
    const std::set<std::pair<int, int>>& conflict_edges,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    const std::unordered_map<int, int>& conflict_colouring,
    const std::unordered_map<int, int>& whole_colouring,
    int Delta,
    // timings...
    double init_time,
    double permutation_time,
    double listsizes_time,
    double assigncolours_time,
    double conflictgraph_time,
    double greedy_colour_conflict_graph_time,
    double colour_non_conflict_vertices_time,
    double total_time,
    // memory...
    size_t init_mem,
    size_t permutation_mem,
    size_t listsizes_mem,
    size_t assigncolours_mem,
    size_t conflictgraph_mem,
    size_t greedy_colour_conflict_graph_mem,
    size_t colour_non_conflict_vertices_mem,
    size_t total_mem
);
