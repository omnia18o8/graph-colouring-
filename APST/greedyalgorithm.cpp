#include "greedyalgorithm.h"
#include "graphinitialisation.h"

#include <numeric>
#include <iostream>
#include <algorithm>

bool is_full_palette(const std::vector<int>& palette) {
    return palette.size() == 1 && palette[0] == 0;
}

std::unordered_map<int, std::vector<int>> adjency_list(
    const std::vector<int>& all_vertices,
    const std::set<std::pair<int, int>>& conflict_edges
) {
    std::unordered_map<int, std::vector<int>> adj;
    adj.reserve(all_vertices.size());

    for (int v : all_vertices)
        adj[v] = {};

    for (const auto& [u, v] : conflict_edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    return adj;
}

std::vector<int> vertex_order(
    std::vector<int>& vertices,
    const std::unordered_map<int, int>& permutation,
    const std::unordered_map<int, int>& list_sizes 
) {
    std::sort(vertices.begin(), vertices.end(), [&](int u, int v) {
        int size_u = list_sizes.count(u) ? list_sizes.at(u) : -1;
        int size_v = list_sizes.count(v) ? list_sizes.at(v) : -1;
        if (size_u != size_v) return size_u < size_v;
        return permutation.at(u) < permutation.at(v);
    });
    return vertices;
}

int assign_colour(const std::vector<int>& palette, const std::vector<bool>& forbidden) {
    for (int c : palette) {
        if (c < (int)forbidden.size() && !forbidden[c])
            return c;
    }
    return -1;
}

std::unordered_map<int, int> conflict_greedy(
    const std::vector<int>& order,
    const std::unordered_map<int, std::vector<int>>& adj,
    const std::unordered_map<int, std::vector<int>>& colour_lists
) {
    int Delta = GraphInitialisation::Delta;
    std::vector<int> full_palette(Delta + 1);
    std::iota(full_palette.begin(), full_palette.end(), 1);

    std::unordered_map<int, int> colouring;
    std::vector<bool> forbidden(Delta + 2, false);  // Colours are 1-based, add buffer

    for (int v : order) {
        const auto& neighbors = adj.at(v);

        // Mark forbidden colours from neighbours
        for (int u : neighbors) {
            if (colouring.count(u))
                forbidden[colouring[u]] = true;
        }

        int assigned = -1;
        const auto& palette_it = colour_lists.at(v);

        if (!is_full_palette(palette_it))
            assigned = assign_colour(palette_it, forbidden);
        else
            assigned = assign_colour(full_palette, forbidden);

        colouring[v] = assigned;

        // Unmark
        for (int u : neighbors) {
            if (colouring.count(u))
                forbidden[colouring[u]] = false;
        }
    }

    return colouring;
}

void full_greedy(
    const std::vector<int>& all_vertices,
    const std::unordered_map<int, std::vector<int>>& conflict_adj,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    std::unordered_map<int, int>& colouring
) {
    int Delta = GraphInitialisation::Delta;
    std::vector<int> full_palette(Delta + 1);
    std::iota(full_palette.begin(), full_palette.end(), 1);

    for (int v : all_vertices) {
        if (!conflict_adj.count(v)) continue;
        if (colouring.count(v)) continue;
        if (!conflict_adj.at(v).empty()) continue;

        int min_colour = -1;
        const auto& palette = colour_lists.at(v);

        if (!is_full_palette(palette))
            min_colour = palette.front();  // assume sorted
        else
            min_colour = full_palette.front();

        if (min_colour != -1)
            colouring[v] = min_colour;
    }
}
