#include "greedyalgorithm.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <set>

// Build adjacency from edge list
std::unordered_map<int, std::vector<int>> build_conflict_adj_from_edges(
    const std::vector<int>& vertices,
    const std::set<std::pair<int, int>>& conflict_edges
) {
    std::cout << "[build_conflict_adj_from_edges] called with " << vertices.size() << " vertices and " 
              << conflict_edges.size() << " edges." << std::endl;
    std::unordered_map<int, std::vector<int>> conflict_adj;
    for (int v : vertices) conflict_adj[v] = {};
    for (const auto& [u, v] : conflict_edges) {
        conflict_adj[u].push_back(v);
        conflict_adj[v].push_back(u);
    }
    // Print adjacency for a few vertices for debugging
    int print_count = 0;
    for (const auto& [vertex, neighbors] : conflict_adj) {
        std::cout << "  Vertex " << vertex << " has neighbors: ";
        for (int n : neighbors) std::cout << n << " ";
        std::cout << std::endl;
        if (++print_count >= 5) break;
    }
    return conflict_adj;
}

// Sorts a colour palette in-place
void sort_colour_palette(std::vector<int>& palette) {
    std::cout << "[sort_colour_palette] Input palette: ";
    for (int c : palette) std::cout << c << " ";
    std::cout << std::endl;
    std::sort(palette.begin(), palette.end());
    std::cout << "[sort_colour_palette] Sorted palette: ";
    for (int c : palette) std::cout << c << " ";
    std::cout << std::endl;
}

// Collects the colours used by a vertex's coloured neighbours
std::unordered_set<int> get_used_colours(
    int v,
    const std::unordered_map<int, std::vector<int>>& conflict_adj,
    const std::unordered_map<int, int>& colouring
) {
    std::unordered_set<int> used_colours;
    for (int u : conflict_adj.at(v)) {
        if (colouring.count(u)) {
            used_colours.insert(colouring.at(u));
        }
    }
    std::cout << "[get_used_colours] For vertex " << v << ", used colours: ";
    for (int c : used_colours) std::cout << c << " ";
    std::cout << std::endl;
    return used_colours;
}

// Assigns the smallest available colour from the palette
int assign_colour(
    const std::vector<int>& palette,
    const std::unordered_set<int>& used_colours
) {
    std::cout << "[assign_colour] Palette to choose from (sorted): ";
    for (int c : palette) std::cout << c << " ";
    std::cout << "\n[assign_colour] Colours used by neighbours: ";
    for (int uc : used_colours) std::cout << uc << " ";
    std::cout << std::endl;

    for (int c : palette) {
        std::cout << "    Checking colour: " << c << "... ";
        if (!used_colours.count(c)) {
            std::cout << "AVAILABLE. Assigning this colour." << std::endl;
            return c;
        } else {
            std::cout << "UNAVAILABLE (used by neighbour)." << std::endl;
        }
    }
    std::cout << "  No available colours found in palette for this vertex!" << std::endl;
    return -1; // No available colour
}

// Returns vertices sorted by (palette size, then permutation pi)
std::vector<int> order_vertices(
    const std::vector<int>& vertices,
    const std::unordered_map<int, int>& pi,
    const std::unordered_map<int, std::vector<int>>& colour_lists
) {
    std::vector<int> order = vertices;
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        int la = colour_lists.at(a).size();
        int lb = colour_lists.at(b).size();
        if (la != lb) return la < lb;
        return pi.at(a) < pi.at(b);
    });
    std::cout << "[order_vertices] Colouring order: ";
    for (int v : order) std::cout << v << " ";
    std::cout << std::endl;
    return order;
}

// The main greedy colouring function
std::unordered_map<int, int> greedy_colour_conflict_graph(
    const std::vector<int>& vertices,
    const std::unordered_map<int, int>& pi,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    const std::set<std::pair<int, int>>& conflict_edges
) {
    std::cout << "[greedy_colour_conflict_graph] Starting greedy colouring..." << std::endl;
    auto conflict_adj = build_conflict_adj_from_edges(vertices, conflict_edges);
    auto order = order_vertices(vertices, pi, colour_lists);

    std::unordered_map<int, int> colouring;
    bool all_coloured = true;

    for (int v : order) {
        std::cout << "[greedy_colour_conflict_graph] Colouring vertex " << v << std::endl;
        auto used_colours = get_used_colours(v, conflict_adj, colouring);
        std::vector<int> palette = colour_lists.at(v);
        sort_colour_palette(palette);
        int assigned = assign_colour(palette, used_colours);

        if (assigned != -1) {
            colouring[v] = assigned;
            std::cout << "[greedy_colour_conflict_graph] Vertex " << v << " assigned colour " << assigned << std::endl;
        } else {
            std::cout << "No available colour for vertex " << v << " (pi: " << pi.at(v) << ")\n";
            all_coloured = false;
        }
    }

    if (all_coloured)
        std::cout << "Proper colouring assigned\n";
    else
        std::cout << "Proper colouring NOT found\n";

    // Print full colouring
    std::cout << "[greedy_colour_conflict_graph] Final colouring map:" << std::endl;
    for (const auto& [v, c] : colouring) {
        std::cout << "  Vertex " << v << ": Colour " << c << std::endl;
    }

    return colouring;
}

// Colour isolated (non-conflict) vertices with their smallest palette colour
void colour_non_conflict_vertices(
    const std::vector<int>& all_vertices,
    const std::unordered_map<int, std::vector<int>>& conflict_adj,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    std::unordered_map<int, int>& colouring
) {
    std::cout << "[colour_non_conflict_vertices] Colouring isolated vertices..." << std::endl;
    for (int v : all_vertices) {
        if (!colouring.count(v) && conflict_adj.at(v).empty()) {
            const auto& colours = colour_lists.at(v);
            if (!colours.empty()) {
                int min_colour = *std::min_element(colours.begin(), colours.end());
                colouring[v] = min_colour;
                std::cout << "  Isolated vertex " << v << " assigned colour " << min_colour << std::endl;
            }
        }
    }
}
