#include "apst.h"
#include "graphinitialisation.h"
#include "permutation.h"
#include "colourlists.h"
#include "conflictgraph.h"
#include "greedyalgorithm.h"

#include <iostream>
#include <chrono>
#include <unordered_set>
#include <sys/resource.h>  // For getrusage
#include <fstream>

APSTResult apst(const std::string& filename, double constant) {
    std::srand(time(0));

    auto total_start = std::chrono::high_resolution_clock::now();

    // Graph Initialisation
    auto gdata = GraphInitialisation::initialise_graph(filename);
    std::vector<int> vertices = std::move(gdata.vertices);
    std::vector<std::pair<int, int>>& edges = gdata.edges;

    // Permutation
    std::unordered_map<int, int> position = randompermutation(vertices);

    // List sizes
    std::unordered_map<int, int> list_sizes = getlistsizes(position, vertices, constant);

    // Colour assignment
    std::unordered_map<int, std::vector<int>> colour_lists = assigncolours(GraphInitialisation::Delta, list_sizes, vertices);
    for (auto& [v, list] : colour_lists) {
        if (!(list.size() == 1 && list[0] == 0) && list.size() != static_cast<std::size_t>(GraphInitialisation::Delta + 1))
            std::sort(list.begin(), list.end());
    }

    // Conflict Graph Construction
    StreamingConflictGraph streamer;
    std::set<std::pair<int, int>> conflict_edges = streamer.conflict_graph(edges, colour_lists);

    // Collect conflict vertices
    std::vector<int> conflict_vertices;
    {
        std::unordered_set<int> seen;
        for (const auto& edge : conflict_edges) {
            seen.insert(edge.first);
            seen.insert(edge.second);
        }
        conflict_vertices.assign(seen.begin(), seen.end());
    }

    // Adjacency + Ordering
    std::unordered_map<int, std::vector<int>> conflict_adj = adjency_list(conflict_vertices, conflict_edges);
    std::vector<int> order = vertex_order(conflict_vertices, position, list_sizes);

    // Conflict Greedy Colouring
    std::unordered_map<int, int> conflict_colouring = conflict_greedy(order, conflict_adj, colour_lists);
    full_greedy(vertices, conflict_adj, colour_lists, conflict_colouring);

    auto total_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur_total = total_end - total_start;

    // Colours used
    std::unordered_set<int> used_colours;
    for (const auto& [v, colour] : conflict_colouring) {
        used_colours.insert(colour);
    }


    // ... after generating list_sizes
double sum = 0.0;
for (const auto& kv : list_sizes)
    sum += kv.second;
double avg_list_size = (list_sizes.empty() ? 0.0 : sum / list_sizes.size());

// ... after building conflict_edges
size_t num_conflict_edges = conflict_edges.size();



    // Memory usage
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    size_t memory = usage.ru_maxrss; // in kilobytes on Linux

    APSTResult result;
    result.filename = filename;
    result.nodes = vertices.size();
    result.delta = GraphInitialisation::Delta;
    result.constant = constant; // Store the constant used
    result.colours_used = used_colours.size();
    result.total_time = dur_total.count();
    result.total_memory = memory * 1024; // convert to bytes
    result.avg_list_size = avg_list_size;
result.conflict_edges = num_conflict_edges;

    return result;
}

void save_apst_result_csv(const APSTResult& result, const std::string& csvfile) {
    std::ofstream out(csvfile, std::ios::app);
    // filename,nodes,delta,constant,colours_used,total_time,total_memory,avg_list_size,conflict_edges
    out << result.filename << ','
        << result.nodes << ','
        << result.delta << ','
        << result.constant << ','
        << result.colours_used << ','
        << result.total_time << ','
        << result.total_memory << ','
        << result.avg_list_size << ','
        << result.conflict_edges << '\n';

}

