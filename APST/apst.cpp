#include "apst.h"
#include "graphinitialisation.h"
#include "permutation.h"
#include "colourlists.h"
#include "conflictgraph.h"
#include "greedyalgorithm.h"

#include <unordered_set>
#include <vector>
#include <set>
#include <chrono>
#include <sys/resource.h>
#include <algorithm>

APSTResult apst(const std::string& filename, double constant) {
    std::srand(time(0));
    

    auto gdata = GraphInitialisation::initialise_graph(filename);
    std::vector<int> vertices = std::move(gdata.vertices);
    std::vector<std::pair<int, int>>& edges = gdata.edges;

    std::unordered_map<int, int> position = randompermutation(vertices);
    std::unordered_map<int, int> list_sizes = getlistsizes(position, vertices, constant);

    std::unordered_map<int, std::vector<int>> colour_lists = assigncolours(GraphInitialisation::Delta, list_sizes, vertices);
    for (auto& [v, list] : colour_lists) {
        if (!(list.size() == 1 && list[0] == 0) && list.size() != static_cast<std::size_t>(GraphInitialisation::Delta + 1))
            std::sort(list.begin(), list.end());
    }
auto total_start = std::chrono::high_resolution_clock::now();
    StreamingConflictGraph streamer;
    std::set<std::pair<int, int>> conflict_edges = streamer.conflict_graph(edges, colour_lists);

    std::vector<int> conflict_vertices;
    {
        std::unordered_set<int> seen;
        for (const auto& edge : conflict_edges) {
            seen.insert(edge.first);
            seen.insert(edge.second);
        }
        conflict_vertices.assign(seen.begin(), seen.end());
    }

    std::unordered_map<int, std::vector<int>> conflict_adj = adjency_list(conflict_vertices, conflict_edges);
    std::vector<int> order = vertex_order(conflict_vertices, position, list_sizes);

    std::unordered_map<int, int> conflict_colouring = conflict_greedy(order, conflict_adj, colour_lists);
    full_greedy(vertices, conflict_adj, colour_lists, conflict_colouring);

    auto total_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur_total = total_end - total_start;

    std::unordered_set<int> used_colours;
    for (const auto& [v, colour] : conflict_colouring) {
        used_colours.insert(colour);
    }

    double sum = 0.0;
    for (const auto& kv : list_sizes) sum += kv.second;
    double avg_list_size = (list_sizes.empty() ? 0.0 : sum / list_sizes.size());

    size_t num_conflict_edges = conflict_edges.size();



        bool proper = true;
    // (1) Any uncoloured vertices?
    for (const auto& v : vertices) {
        if (!conflict_colouring.count(v) || conflict_colouring[v] <= 0) {
            proper = false;
            break;
        }
    }
    // (2) Any edge with the same colour at both ends?
    if (proper) {
        for (const auto& [u, v] : edges) {
            if (conflict_colouring.count(u) && conflict_colouring.count(v) && conflict_colouring[u] == conflict_colouring[v]) {
                proper = false;
                break;
            }
        }
    }


    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    size_t memory_bytes = usage.ru_maxrss;        // ru_maxrss is bytes on macOS
    double memory = memory_bytes / (1024.0 * 1024.0);  // now in MB

 

    APSTResult result;
    result.filename = filename;
    result.nodes = vertices.size();
    result.delta = GraphInitialisation::Delta;
    result.constant = constant;
    result.success = proper;  // true if colouring is proper, false otherwise
    result.colours_used = used_colours.size();
    result.total_time = dur_total.count();
    result.total_memory = memory; // to MB
    result.avg_list_size = avg_list_size;
    result.conflict_edges = num_conflict_edges;






    return result;
}
