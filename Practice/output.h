#ifndef OUTPUT_H
#define OUTPUT_H

#include <vector>
#include <set>
#include <unordered_map>
#include <string>

void print_graph_stats(
    const std::string& input_graph_file,
    const std::vector<int>& vertices,
    const std::vector<std::pair<int, int>>& edges,
    const std::set<std::pair<int, int>>& conflict_edges,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    const std::unordered_map<int, int>& conflict_colouring,
    const std::unordered_map<int, int>& whole_colouring,
    int Delta,
    const std::unordered_map<int, int>& pi,
    double conflictgraph_time,
    double greedy_colour_conflict_graph_time,
    double build_conflict_adj_from_edges_time,
    double colour_non_conflict_vertices_time
);

#endif
