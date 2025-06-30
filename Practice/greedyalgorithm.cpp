#include "greedyalgorithm.h"
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <set>
#include <iostream>


std::unordered_map<int, std::vector<int>> build_conflict_adj_from_edges(
    const std::vector<int>& vertices,
    const std::set<std::pair<int, int>>& conflict_edges
) {
    std::unordered_map<int, std::vector<int>> adj;
    for (int v : vertices) adj[v] = {};
    for (const auto& [u, v] : conflict_edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    return adj;
}


std::vector<int> vertex_order(
    const std::vector<int>& vertices,
    const std::unordered_map<int, int>& pi,
    const std::unordered_map<int, std::vector<int>>& colour_lists
) {
    std::vector<int> order = vertices;
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        int sa = colour_lists.at(a).size(), sb = colour_lists.at(b).size();
        if (sa != sb) return sa < sb;
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
    auto adj = build_conflict_adj_from_edges(vertices, conflict_edges);
    auto order = vertex_order(vertices, pi, colour_lists);
    std::unordered_map<int, int> colouring;

    for (int v : order) {
        const auto& palette_unsorted = colour_lists.at(v);
        std::vector<int> palette = palette_unsorted;
        std::sort(palette.begin(), palette.end()); // Ensure order

      
        std::unordered_set<int> forbidden;
        for (int u : adj[v]) {
            auto it = colouring.find(u);
            if (it != colouring.end()) forbidden.insert(it->second);
        }

        int assigned = -1;
        for (int c : palette) {
            if (!forbidden.count(c)) {
                assigned = c;
                break;
            }
        }
        colouring[v] = assigned;
    }

    bool all_coloured = std::all_of(order.begin(), order.end(),
        [&](int v) { return colouring[v] != -1; });

    std::cout << (all_coloured ? "Proper colouring assigned of conflict graph\n"
                               : "Proper colouring NOT found of conflict graph\n");

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
                // Assign the smallest colour
                int min_colour = *std::min_element(colours.begin(), colours.end());
                colouring[v] = min_colour;
            }
        }
    }
    std::cout << "Whole graph coloured\n";
}
