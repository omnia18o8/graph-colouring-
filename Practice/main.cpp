#include "graphinitialisation.h"
#include "permutation.h"
#include "colourlists.h"
#include "conflictgraph.h"
#include "greedyalgorithm.h"
#include "output.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iomanip>
#include <chrono>

int main() {
    std::string file = "../Graphs/facebook_combined.txt"; 

    // Step 1: Initialise graph
    std::vector<int> vertices = GraphInitialisation::get_vertices(file);
    GraphInitialisation::V = vertices.size();
    GraphInitialisation::getdelta(file); 

    // Step 2: Generate permutation
    std::vector<int> permuted_vertices;
    std::unordered_map<int, int> position;
    randompermutation(vertices, permuted_vertices, position);

    // Step 3a: List sizes per vertex
    std::unordered_map<int, int> list_sizes = getlistsizes(
        GraphInitialisation::V,
        GraphInitialisation::Delta,
        position,
        permuted_vertices
    );

    // Step 3b: Assign colour lists
    std::unordered_map<int, std::vector<int>> colour_lists = assigncolours(
        GraphInitialisation::Delta,
        list_sizes,
        permuted_vertices
    );

    // Step 4: Stream edge list again and build conflict subgraphs
    //StreamingConflictGraph streamer;
    StreamingConflictGraph streamer(GraphInitialisation::Delta + 1, vertices.size(), 4096);
    auto conflictsubgraphs_start = std::chrono::high_resolution_clock::now();

    std::ifstream edge_file(file);
    int u, v;
    while (edge_file >> u >> v) {
        streamer.conflictsubgraphs(u, v, colour_lists);
    }
    edge_file.close();

    auto conflictsubgraphs_end = std::chrono::high_resolution_clock::now();
    double conflictsubgraphs_time = std::chrono::duration<double>(conflictsubgraphs_end - conflictsubgraphs_start).count();


    // --- Time conflictgraph extraction ---
    auto conflictgraph_start = std::chrono::high_resolution_clock::now();
    std::set<std::pair<int, int>> conflict_edges = streamer.conflictgraph();
    auto conflictgraph_end = std::chrono::high_resolution_clock::now();
    double conflictgraph_time = std::chrono::duration<double>(conflictgraph_end - conflictgraph_start).count();

    

    // Step 5: Greedy colouring of conflict graph

    auto greedy_colour_conflict_graph_start = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, int> conflict_colouring = greedy_colour_conflict_graph(
        vertices, position, colour_lists, conflict_edges
    );
    auto greedy_colour_conflict_graph_end = std::chrono::high_resolution_clock::now();
    double greedy_colour_conflict_graph_time = std::chrono::duration<double>(
        greedy_colour_conflict_graph_end - greedy_colour_conflict_graph_start
    ).count();

    // Build conflict adjacency
    auto build_conflict_adj_from_edges_start = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, std::vector<int>> conflict_adj = build_conflict_adj_from_edges(
        vertices, conflict_edges
    );
    auto build_conflict_adj_from_edges_end = std::chrono::high_resolution_clock::now();
    double build_conflict_adj_from_edges_time = std::chrono::duration<double>(
        build_conflict_adj_from_edges_end - build_conflict_adj_from_edges_start
    ).count();

    // Colour non-conflict vertices
    auto colour_non_conflict_vertices_start = std::chrono::high_resolution_clock::now();
    colour_non_conflict_vertices(vertices, conflict_adj, colour_lists, conflict_colouring);
    auto colour_non_conflict_vertices_end = std::chrono::high_resolution_clock::now();
    double colour_non_conflict_vertices_time = std::chrono::duration<double>(
        colour_non_conflict_vertices_end - colour_non_conflict_vertices_start
    ).count();


     print_graph_stats(
        file,
        vertices,
        GraphInitialisation::get_edges(file),
        conflict_edges,
        colour_lists,
        conflict_colouring,
        conflict_colouring, // or your final full colouring
        GraphInitialisation::Delta,
        position,
        conflictsubgraphs_time,
    conflictgraph_time,
    greedy_colour_conflict_graph_time,
    build_conflict_adj_from_edges_time,
    colour_non_conflict_vertices_time
    );


    return 0;
}
