#include "output.h"
#include <iostream>
#include <cmath>
#include <unordered_set>
#include <limits>
#include <iomanip>

#if defined(__APPLE__)
#include <mach/mach.h>
#elif defined(__linux__)
#include <sys/resource.h>
#include <unistd.h>
#endif

size_t getCurrentRSS() {
#if defined(__APPLE__)
    mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
        return info.resident_size;
    }
    return 0;
#elif defined(__linux__)
    long rss = 0L;
    FILE* fp = nullptr;
    if ((fp = fopen("/proc/self/statm", "r")) == nullptr) return 0;
    if (fscanf(fp, "%*s%ld", &rss) != 1) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return rss * sysconf(_SC_PAGESIZE);
#else
    return 0;
#endif
}

void print_graph_stats(
    const std::string& input_graph_file,
    const std::vector<int>& vertices,
    const std::vector<std::pair<int, int>>& edges,
    const std::set<std::pair<int, int>>& conflict_edges,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    const std::unordered_map<int, int>& conflict_colouring,
    const std::unordered_map<int, int>& whole_colouring,
    int Delta,
    double init_time,
    double permutation_time,
    double listsizes_time,
    double assigncolours_time,
    double conflictgraph_time,
    double greedy_colour_conflict_graph_time,
    double colour_non_conflict_vertices_time,
    double total_time,
    size_t init_mem,
    size_t permutation_mem,
    size_t listsizes_mem,
    size_t assigncolours_mem,
    size_t conflictgraph_mem,
    size_t greedy_colour_conflict_graph_mem,
    size_t colour_non_conflict_vertices_mem,
    size_t total_mem
) {
    std::cout << std::fixed << std::setprecision(6);

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

    // --- List size stats (average, min, using restricted palette map + full palette count)
    double sum = 0.0;
    int min_list_size = std::numeric_limits<int>::max();

    for (const auto& v : vertices) {
        int lsize = Delta + 1;
        auto it = colour_lists.find(v);
        if (it != colour_lists.end()) lsize = (int)it->second.size();
        sum += lsize;
        if (lsize < min_list_size)
            min_list_size = lsize;
    }
    double avg_list_size = vertices.empty() ? 0.0 : sum / vertices.size();
    std::cout << "Average list size:                " << avg_list_size << "\n";
    std::cout << "Smallest list size:               " << min_list_size << "\n";
    double expected = std::pow(std::log2(vertices.size()), 2);
    std::cout << "Expected avg list size (log2(n))^2: " << expected << "\n";
    

    // Print the palette for each vertex (or first N if large)

// for (size_t i = 0; i < vertices.size(); ++i) {
//     int v = vertices[i];
//     std::cout << "Vertex " << v << ": palette = [";
//     auto it = colour_lists.find(v);
//     if (it != colour_lists.end()) {
//         const std::vector<int>& pal = it->second;
//         for (size_t j = 0; j < pal.size(); ++j) {
//             std::cout << pal[j];
//             if (j + 1 < pal.size()) std::cout << ", ";
//         }
//     } else {
//         std::cout << "full";
//     }
//     std::cout << "]\n";
// }

// --- Check for failed colour assignment (-1 means uncoloured)
bool failed = false;
for (const auto& [v, c] : whole_colouring) {
    if (c == -1) {
        failed = true;
        break;
    }
}
if (failed) {
    std::cout << "WARNING: At least one vertex could not be coloured (colour -1 found). Colouring FAILED.\n";
} else {
    std::cout << "All vertices successfully coloured.\n";
}


    // --- Colours used
    std::unordered_set<int> conflict_colours;
    for (const auto& [v, c] : conflict_colouring) conflict_colours.insert(c);
    std::cout << "Colours used in conflict graph:   " << conflict_colours.size() << "\n";

    std::unordered_set<int> all_colours;
    for (const auto& [v, c] : whole_colouring) all_colours.insert(c);
    std::cout << "Colours used in whole graph:      " << all_colours.size() << "\n";

    std::cout << "======================================\n";

    // --- Timings and memory
    std::cout << std::setw(38) << std::left << "Step"
              << std::setw(12) << "Time (s)"
              << std::setw(12) << "Mem (MB)" << "\n";
    std::cout << "---------------------------------------------------------\n";
    std::cout << std::setw(38) << "Initialisation"
              << std::setw(12) << init_time
              << std::setw(12) << (init_mem / (1024 * 1024)) << "\n";
    std::cout << std::setw(38) << "Permutation"
              << std::setw(12) << permutation_time
              << std::setw(12) << (permutation_mem / (1024 * 1024)) << "\n";
    std::cout << std::setw(38) << "List size calculation"
              << std::setw(12) << listsizes_time
              << std::setw(12) << (listsizes_mem / (1024 * 1024)) << "\n";
    std::cout << std::setw(38) << "Assign colour lists"
              << std::setw(12) << assigncolours_time
              << std::setw(12) << (assigncolours_mem / (1024 * 1024)) << "\n";
    std::cout << std::setw(38) << "Conflict graph construction"
              << std::setw(12) << conflictgraph_time
              << std::setw(12) << (conflictgraph_mem / (1024 * 1024)) << "\n";
    std::cout << std::setw(38) << "Greedy colouring of conflict graph"
              << std::setw(12) << greedy_colour_conflict_graph_time
              << std::setw(12) << (greedy_colour_conflict_graph_mem / (1024 * 1024)) << "\n";
    std::cout << std::setw(38) << "Colour non-conflict vertices"
              << std::setw(12) << colour_non_conflict_vertices_time
              << std::setw(12) << (colour_non_conflict_vertices_mem / (1024 * 1024)) << "\n";
    std::cout << std::setw(38) << "Total pipeline"
              << std::setw(12) << total_time
              << std::setw(12) << (total_mem / (1024 * 1024)) << "\n";
    std::cout << "======================================\n";
}
