#include "output.h"
#include <iostream>
#include <cmath>
#include <unordered_set>
#include <limits>
#include <iomanip>

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
    double conflictsubgraphs_time,
    double conflictgraph_time,
    double greedy_colour_conflict_graph_time,
    double build_conflict_adj_from_edges_time,
    double colour_non_conflict_vertices_time
) {
    std::cout << "========== GRAPH STATISTICS ==========\n";
    std::cout << "Input graph file:                " << input_graph_file << "\n";
    std::cout << "--------------------------------------\n";
    std::cout << "Vertices in original graph:       " << vertices.size() << "\n";
    std::cout << "Edges in original graph:          " << edges.size() << "\n";

    std::unordered_set<int> conflict_vertices;
    for (const auto& [u, v] : conflict_edges) {
        conflict_vertices.insert(u);
        conflict_vertices.insert(v);
    }
    std::cout << "Vertices in conflict graph:       " << conflict_vertices.size() << "\n";
    std::cout << "Edges in conflict graph:          " << conflict_edges.size() << "\n";

    std::cout << "Delta (max degree):               " << Delta << "\n";

    // List size stats
    double sum = 0.0;
    int min_list_size = std::numeric_limits<int>::max();
    for (const auto& [v, clist] : colour_lists) {
        sum += clist.size();
        if ((int)clist.size() < min_list_size)
            min_list_size = clist.size();
    }
    double avg_list_size = (colour_lists.empty() ? 0.0 : sum / colour_lists.size());
    std::cout << "Average list size:                " << avg_list_size << "\n";
    std::cout << "Smallest list size:               " << min_list_size << "\n";
    double expected = std::pow(std::log2(vertices.size()), 2);
    std::cout << "Expected avg list size (log2(n))^2: " << expected << "\n";

    // Colours used (conflict graph)
    std::unordered_set<int> conflict_colours;
    for (const auto& [v, c] : conflict_colouring) conflict_colours.insert(c);
    std::cout << "Colours used in conflict graph:   " << conflict_colours.size() << "\n";
    std::cout << "Conflict graph colours:\n";
    

    // Colours used (whole graph)
    std::unordered_set<int> all_colours;
    for (const auto& [v, c] : whole_colouring) all_colours.insert(c);
    std::cout << "Colours used in whole graph:      " << all_colours.size() << "\n";
    std::cout << "Whole graph colours:\n";
    // for (const auto& [v, c] : whole_colouring) {
    //     std::cout << "  Vertex " << v << ": colour " << c << "\n";
    // }
    

    // List size & permutation for a sample of vertices
    std::cout << "Sample list sizes and permutation:\n";
    int count = 0;
    for (const auto& [v, clist] : colour_lists) {
        int perm = pi.at(v);
        std::cout << "  Vertex " << v << ": size " << clist.size() << ", pi(v) = " << perm << "\n";
        if (++count >= 5) break;
    }

    // Timing
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "conflictsubgraphs.time:           " << conflictsubgraphs_time << " seconds\n";
    std::cout << "conflictgraph.time:               " << conflictgraph_time << " seconds\n";
    std::cout << "greedy_colour_conflict_graph.time: " << greedy_colour_conflict_graph_time << " seconds\n";
std::cout << "build_conflict_adj_from_edges.time: " << build_conflict_adj_from_edges_time << " seconds\n";
std::cout << "colour_non_conflict_vertices.time: " << colour_non_conflict_vertices_time << " seconds\n";

    std::cout << "======================================\n";
}
