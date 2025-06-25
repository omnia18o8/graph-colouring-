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
#include <iomanip>

int main() {
    std::string file = "../Graphs/email-Eu-core.txt"; 

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
    StreamingConflictGraph streamer;
    std::ifstream edge_file(file);
    int u, v;
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

    return 0;
}
