#include "graphinitialisation.h"
#include "permutation.h"
#include "colourlists.h"
#include "conflictgraph.h"
#include "greedyalgorithm.h"
#include "output.h"

#include <iostream>
#include <chrono>
#include <unordered_set>

int main() {
    
    std::srand(time(0));

    auto total_start = std::chrono::high_resolution_clock::now();

    // Graph Initialisation
    auto t1 = std::chrono::high_resolution_clock::now();
    std::string file = "../Graphs/SNAPS/com-orkut.ungraph.txt";
    auto gdata = GraphInitialisation::initialise_graph(file);
    std::vector<int> vertices = std::move(gdata.vertices);
    std::vector<std::pair<int, int>>& edges = gdata.edges;
    auto t2 = std::chrono::high_resolution_clock::now();

    // Permutation
    auto t3 = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, int> position = randompermutation(vertices);
    auto t4 = std::chrono::high_resolution_clock::now();

    // List sizes
    auto t5 = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, int> list_sizes = getlistsizes(position, vertices);
    auto t6 = std::chrono::high_resolution_clock::now();

    // Colour assignment
    auto t7 = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, std::vector<int>> colour_lists = assigncolours(GraphInitialisation::Delta, list_sizes, vertices);

    for (auto& [v, list] : colour_lists) {
    if (!(list.size() == 1 && list[0] == 0) && list.size() != static_cast<std::size_t>(GraphInitialisation::Delta + 1))
        std::sort(list.begin(), list.end());
}


    auto t8 = std::chrono::high_resolution_clock::now();

    // Conflict Graph Construction
    auto t9 = std::chrono::high_resolution_clock::now();
    StreamingConflictGraph streamer;
    std::set<std::pair<int, int>> conflict_edges = streamer.conflict_graph(edges, colour_lists);
    auto t10 = std::chrono::high_resolution_clock::now();

    // Collect conflict vertices
    auto t11 = std::chrono::high_resolution_clock::now();
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
    auto t12 = std::chrono::high_resolution_clock::now();

    // Conflict Greedy Colouring
    auto t13 = std::chrono::high_resolution_clock::now();
    std::unordered_map<int, int> conflict_colouring = conflict_greedy(order, conflict_adj, colour_lists);
    full_greedy(vertices, conflict_adj, colour_lists, conflict_colouring);
    auto t14 = std::chrono::high_resolution_clock::now();

    // Print timings
    auto total_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur_total = total_end - total_start;
    std::chrono::duration<double> dur1 = t2 - t1;
    std::chrono::duration<double> dur2 = t4 - t3;
    std::chrono::duration<double> dur3 = t6 - t5;
    std::chrono::duration<double> dur4 = t8 - t7;
    std::chrono::duration<double> dur5 = t10 - t9;
    std::chrono::duration<double> dur6 = t12 - t11;
    std::chrono::duration<double> dur7 = t14 - t13;

    std::cout << "\n[FUNCTION TIMINGS]\n";
    std::cout << "  ➤ Graph Initialisation:           " << dur1.count() << "s\n";
    std::cout << "  ➤ Permutation:                    " << dur2.count() << "s\n";
    std::cout << "  ➤ List Size Generation:           " << dur3.count() << "s\n";
    std::cout << "  ➤ Colour Assignment:              " << dur4.count() << "s\n";
    std::cout << "  ➤ Conflict Graph Construction:    " << dur5.count() << "s\n";
    std::cout << "  ➤ Adjacency + Ordering:           " << dur6.count() << "s\n";
    std::cout << "  ➤ Greedy Colouring:               " << dur7.count() << "s\n";
    std::cout << "  ➤ Total Wall Clock Time:          " << dur_total.count() << "s\n";

    // Colours used
    std::unordered_set<int> used_colours;
    for (const auto& [v, colour] : conflict_colouring) {
        used_colours.insert(colour);
    }

    std::cout << "\n[PROGRAM OUTPUT]\n";
    std::cout << "  ➤ Delta (Max Degree):             " << GraphInitialisation::Delta << "\n";
    std::cout << "  ➤ Total Colours Used:             " << used_colours.size() << "\n";
    std::cout << "  ➤ Vertices Coloured:              " << conflict_colouring.size() << "\n";

    // Output the colouring
    std::cout << "\n[COLOURING OUTPUT]\n";
    for (const auto& [v, colour] : conflict_colouring) {
        std::cout << "Vertex " << v << ": Colour " << colour << "\n";
    }

    return 0;
}
