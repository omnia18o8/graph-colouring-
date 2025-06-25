#include "graphinitialisation.h"
#include "permutation.h"
#include "colourlists.h"
#include "conflictgraph.h"
#include "greedyalgorithm.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include <iomanip>

// Function to output everything
void output_all_results(
    const std::vector<int>& vertices,
    const std::unordered_map<int, int>& pi,
    const std::unordered_map<int, int>& list_sizes,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    const std::unordered_map<int, int>& colouring,
    int edge_count,
    int delta
) {
    // File 1: vertex + permutation + list size + final colour
    std::ofstream out1("Output/vertex_permutation_listsize_colour.txt");
    out1 << std::left << std::setw(8) << "Vertex"
         << std::setw(8) << "π(v)"
         << std::setw(8) << "l(v)"
         << "Colour\n";
    for (int v : vertices) {
        out1 << std::left
             << std::setw(8) << v
             << std::setw(8) << (pi.count(v) ? pi.at(v) : -1)
             << std::setw(8) << (list_sizes.count(v) ? list_sizes.at(v) : -1)
             << (colouring.count(v) ? std::to_string(colouring.at(v)) : "UNCOLOURED")
             << "\n";
    }
    out1.close();

    // File 2: vertex and its colour list
    std::ofstream out2("Output/vertex_colour_lists.txt");
    for (const auto& [v, colours] : colour_lists) {
        out2 << "Vertex " << v << ": ";
        for (int c : colours) out2 << c << " ";
        out2 << "\n";
    }
    out2.close();

    // File 3: summary stats
    std::ofstream out3("Output/summary_stats.txt");
    out3 << "Vertices: " << vertices.size() << "\n";
    out3 << "Edges: " << edge_count << "\n";
    out3 << "Delta: " << delta << "\n";

    double avg_list_size = 0.0;
    for (const auto& [_, size] : list_sizes) avg_list_size += size;
    avg_list_size /= list_sizes.size();
    out3 << "Average l(v): " << avg_list_size << "\n";

    std::map<int, int> colour_freq;
    for (const auto& [v, c] : colouring) colour_freq[c]++;
    out3 << "Unique colours used: " << colour_freq.size() << "\n";
    out3 << "Colour frequencies:\n";
    for (const auto& [c, freq] : colour_freq) {
        out3 << "  Colour " << c << ": " << freq << " vertices\n";
    }
    out3.close();
}


int main() {
    std::string file = "../Graphs/cycle.txt"; // Path to the edge list file

    // Step 1: Collect all vertices from the edge list
    std::unordered_set<int> all_vertices_set;
    std::ifstream vertex_file(file);
    int u, v;
    while (vertex_file >> u >> v) {
        all_vertices_set.insert(u);
        all_vertices_set.insert(v);
    }
    vertex_file.close();

    std::vector<int> vertices(all_vertices_set.begin(), all_vertices_set.end());
    GraphInitialisation::V = vertices.size();
    GraphInitialisation::getdelta(file, false); // true for directed graph

    // Step 2: Generate permutation
    std::vector<int> permuted_vertices;
    std::unordered_map<int, int> position;
    randompermutation(vertices, permuted_vertices, position);

    // Step 3a: Assign l(v) size per vertex
    std::unordered_map<int, int> list_sizes = getlistsizes(
        GraphInitialisation::V,
        GraphInitialisation::Delta,
        position,
        permuted_vertices
    );

    // Step 3b: Assign l(v) colours per vertex
    std::unordered_map<int, std::vector<int>> colour_lists = assigncolours(
        GraphInitialisation::Delta,
        list_sizes,
        permuted_vertices
    );

    // Step 4: Stream edge list again and build conflict subgraphs
    StreamingConflictGraph streamer;
    std::ifstream edge_file(file);  // new stream
    while (edge_file >> u >> v) {
        streamer.conflictsubgraphs(u, v, colour_lists);
    }
    edge_file.close();

    std::set<std::pair<int, int>> conflict_edges = streamer.conflictgraph();

    // Step 5: Greedy colouring of conflict graph
    std::unordered_map<int, int> conflict_colouring = greedy_colour_conflict_graph(
        vertices, position, colour_lists, conflict_edges
    );

    std::unordered_map<int, std::vector<int>> conflict_adj = build_conflict_adj_from_edges(
        vertices, conflict_edges
    );

    colour_non_conflict_vertices(vertices, conflict_adj, colour_lists, conflict_colouring);

    output_all_results(
        permuted_vertices,
        position,
        list_sizes,
        colour_lists,
        conflict_colouring,
        GraphInitialisation::E,
        GraphInitialisation::Delta
    );

    return 0;
}