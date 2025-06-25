#include "greedyalgorithm.h"
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <set>
#include <iostream>

// Build adjacency from edge list
std::unordered_map<int, std::vector<int>> build_conflict_adj_from_edges(
    const std::vector<int>& vertices,
    const std::set<std::pair<int, int>>& conflict_edges
) {
    std::unordered_map<int, std::vector<int>> conflict_adj;
    for (int v : vertices) conflict_adj[v] = {};
    for (const auto& [u, v] : conflict_edges) {
        conflict_adj[u].push_back(v);
        conflict_adj[v].push_back(u);
    }
    return conflict_adj;
}

// Sorts a colour palette in-place
void sort_colour_palette(std::vector<int>& palette) {
    std::sort(palette.begin(), palette.end());
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
    return used_colours;
}

// Assigns the smallest available colour from the palette
int assign_colour(
    const std::vector<int>& palette,
    const std::unordered_set<int>& used_colours
) {
    for (int c : palette) {
        if (!used_colours.count(c)) {
            return c;
        }
    }
    return -1; 
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
    return order;
}

std::unordered_map<int, int> greedy_colour_conflict_graph(
    const std::vector<int>& vertices,
    const std::unordered_map<int, int>& pi,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    const std::set<std::pair<int, int>>& conflict_edges
) {
    auto conflict_adj = build_conflict_adj_from_edges(vertices, conflict_edges);
    auto order = order_vertices(vertices, pi, colour_lists);

    std::unordered_map<int, int> colouring;
    bool all_coloured = true;

    for (int v : order) {
        auto used_colours = get_used_colours(v, conflict_adj, colouring);
        std::vector<int> palette = colour_lists.at(v);
        sort_colour_palette(palette);
        int assigned = assign_colour(palette, used_colours);

        if (assigned != -1) {
            colouring[v] = assigned;
        } else {
            all_coloured = false;
        }
    }

    if (all_coloured)
        std::cout << "Proper colouring assigned of conflict graph\n";
    else
        std::cout << "Proper colouring NOT found of conflict graph\n";

    return colouring;
}

void colour_non_conflict_vertices(
    const std::vector<int>& all_vertices,
    const std::unordered_map<int, std::vector<int>>& conflict_adj,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    std::unordered_map<int, int>& colouring
) {
    for (int v : all_vertices) {
        if (!colouring.count(v) && conflict_adj.at(v).empty()) {
            const auto& colours = colour_lists.at(v);
            if (!colours.empty()) {
                int min_colour = *std::min_element(colours.begin(), colours.end());
                colouring[v] = min_colour;
            }
        }
    }
    std::cout << "Whole graph coloured\n";
}

