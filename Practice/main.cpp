#include "graphinitialisation.h"
#include "permutation.h"
#include "colourlists.h"
#include "conflictgraph.h"
#include "greedyalgorithm.h"
#include "output.h"

#include <vector>
#include <unordered_map>
#include <set>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <unordered_set>


int main() {
    std::string file = "../Graphs/com-dblp.ungraph.txt"; // Change this to your graph file path
    std::srand(time(0));
    auto total_start = std::chrono::high_resolution_clock::now();

    // Step 1: Graph initialisation
    auto init_start = std::chrono::high_resolution_clock::now();
    auto gdata = GraphInitialisation::initialise_graph(file);
    std::vector<int> vertices = std::move(gdata.vertices);
    std::vector<std::pair<int, int>>& edges = gdata.edges;
    auto init_end = std::chrono::high_resolution_clock::now();
    std::cout << "Step 1 - Graph initialisation completed.\n";
    double init_time = std::chrono::duration<double>(init_end - init_start).count();
    size_t init_mem = getCurrentRSS();

    // Step 2: Generate permutation IN-PLACE
    auto permutation_start = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, int> position = randompermutation(vertices);
    auto permutation_end = std::chrono::high_resolution_clock::now();
    std::cout << "Step 2 - Permutation completed.\n";
    double permutation_time = std::chrono::duration<double>(permutation_end - permutation_start).count();
    size_t permutation_mem = getCurrentRSS();

    // Step 3a: List sizes per vertex
    auto listsizes_start = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, int> list_sizes = getlistsizes(
        GraphInitialisation::V, GraphInitialisation::Delta, position, vertices);
    auto listsizes_end = std::chrono::high_resolution_clock::now();
    std::cout << "Step 3a - List sizes computed.\n";
    double listsizes_time = std::chrono::duration<double>(listsizes_end - listsizes_start).count();
    size_t listsizes_mem = getCurrentRSS();

    // Step 3b: Assign colour lists (restricted palettes only!)
    auto assigncolours_start = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, std::vector<int>> colour_lists = assigncolours(
        GraphInitialisation::Delta, list_sizes, vertices);
    auto assigncolours_end = std::chrono::high_resolution_clock::now();
    std::cout << "Step 3b - Colour lists assigned.\n";
    double assigncolours_time = std::chrono::duration<double>(assigncolours_end - assigncolours_start).count();
    size_t assigncolours_mem = getCurrentRSS();


    // Step 5: Build conflict graph
    StreamingConflictGraph streamer;
    auto conflictgraph_start = std::chrono::high_resolution_clock::now();

    std::set<std::pair<int, int>> conflict_edges = streamer.conflict_graph(edges, colour_lists);

    auto conflictgraph_end = std::chrono::high_resolution_clock::now();
    double conflictgraph_time = std::chrono::duration<double>(conflictgraph_end - conflictgraph_start).count();
    size_t conflictgraph_mem = getCurrentRSS();
    std::cout << "Step 5 - Conflict graph built.\n";

    // Step 6a: Prepare order and adjacency only from the conflict graph vertices
    std::vector<int> conflict_vertices;
    {
        std::unordered_set<int> seen;
        for (const auto& edge : conflict_edges) {
            seen.insert(edge.first);
            seen.insert(edge.second);
        }
        conflict_vertices.assign(seen.begin(), seen.end());
    }

    // Step 6b: Colour the conflict graph using greedy algorithm

    auto greedy_colour_conflict_graph_start = std::chrono::high_resolution_clock::now();
    std::vector<int> full_palette(GraphInitialisation::Delta + 1);
    std::iota(full_palette.begin(), full_palette.end(), 1);
    std::unordered_map<int, std::vector<int>> conflict_adj = adjency_list(conflict_vertices, conflict_edges);
    std::vector<int> order = vertex_order(conflict_vertices, position, list_sizes);
    std::unordered_map<int, int> conflict_colouring = conflict_greedy(order, conflict_adj, colour_lists, full_palette);

    auto greedy_colour_conflict_graph_end = std::chrono::high_resolution_clock::now();
    double greedy_colour_conflict_graph_time = std::chrono::duration<double>( greedy_colour_conflict_graph_end - greedy_colour_conflict_graph_start).count();
    size_t greedy_colour_conflict_graph_mem = getCurrentRSS();
    std::cout << "Step 6 - Conflict graph coloured.\n";


    // Step 8: Colour non-conflict vertices
    auto colour_non_conflict_vertices_start = std::chrono::high_resolution_clock::now();

    full_greedy(vertices, conflict_adj, colour_lists, conflict_colouring, full_palette);

    auto colour_non_conflict_vertices_end = std::chrono::high_resolution_clock::now();
    std::cout << "Step 8 - Non-conflict vertices coloured.\n";
    double colour_non_conflict_vertices_time = std::chrono::duration<double>(colour_non_conflict_vertices_end - colour_non_conflict_vertices_start).count();
    size_t colour_non_conflict_vertices_mem = getCurrentRSS();

    auto total_end = std::chrono::high_resolution_clock::now();
    double total_time = std::chrono::duration<double>(total_end - total_start).count();
    size_t total_mem = getCurrentRSS();


    // Output
    print_graph_stats(
        file,
        vertices,
        edges,
        conflict_edges,
        colour_lists,
        conflict_colouring,
        conflict_colouring,
        GraphInitialisation::Delta,
        init_time, permutation_time, listsizes_time, assigncolours_time, conflictgraph_time,
        greedy_colour_conflict_graph_time, colour_non_conflict_vertices_time, total_time,
        init_mem, permutation_mem, listsizes_mem, assigncolours_mem, conflictgraph_mem,
        greedy_colour_conflict_graph_mem, colour_non_conflict_vertices_mem, total_mem
    );
    return 0;
}
