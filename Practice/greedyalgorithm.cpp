#include "greedyalgorithm.h"

bool is_full_palette(const std::vector<int>& palette) {
    return palette.size() == 1 && palette[0] == 0;
}

std::unordered_map<int, std::vector<int>> adjency_list(
    const std::vector<int>& vertices,
    const std::set<std::pair<int, int>>& conflict_edges
) {
    std::unordered_map<int, std::vector<int>> adj;
    for (size_t i = 0; i < vertices.size(); ++i) { // Initialize adjacency list for each vertex
        adj[vertices[i]] = std::vector<int>();
    }
    for (const std::pair<int, int>& edge : conflict_edges) {
    int u = edge.first;
    int v = edge.second;
    adj[u].push_back(v);
    adj[v].push_back(u); // Undirected: both directions 
    }
    return adj;
}

std::vector<int> vertex_order(
    std::vector<int>& vertices,
    const std::unordered_map<int, int>& permutation,
    const std::unordered_map<int, int>& list_sizes // pass this in!
) {
    std::sort(vertices.begin(), vertices.end(), [&](int a, int b) {
        int pa = list_sizes.at(a);
        int pb = list_sizes.at(b);
        if (pa != pb) return pa < pb; //returns pa if true
        return permutation.at(a) < permutation.at(b); 
    });
    return vertices;
}


int assign_colour(const std::vector<int>& palette, const std::unordered_set<int>& forbidden) {
    for (size_t i = 0; i < palette.size(); ++i) {
        int c = palette[i];
        if (!forbidden.count(c)) return c;
    }
    return -1;
}


std::unordered_map<int, int> conflict_greedy(
    const std::vector<int>& order,
    const std::unordered_map<int, std::vector<int>>& adj,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    const std::vector<int>& full_palette
) {
    std::unordered_map<int, int> colouring;

    for (size_t idx = 0; idx < order.size(); ++idx) {
        int v = order[idx];
        std::unordered_set<int> forbidden;
        const std::vector<int>& neighbours = adj.at(v);
        
        for (size_t j = 0; j < neighbours.size(); ++j) {
            int u = neighbours[j];
            if (colouring.count(u)) forbidden.insert(colouring[u]);
        }

        int assigned = -1;
        if (colour_lists.count(v) && !is_full_palette(colour_lists.at(v))) {
            assigned = assign_colour(colour_lists.at(v), forbidden);
        } else {
            assigned = assign_colour(full_palette, forbidden);
        }
        colouring[v] = assigned;
    }
    return colouring;
}

void full_greedy(
    const std::vector<int>& all_vertices,
    const std::unordered_map<int, std::vector<int>>& conflict_adj,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    std::unordered_map<int, int>& colouring,
    const std::vector<int>& full_palette
) {
    for (size_t i = 0; i < all_vertices.size(); ++i) {
        int v = all_vertices[i];
        if (!colouring.count(v) && conflict_adj.at(v).empty()) {
            int min_colour = -1;
            if (colour_lists.count(v) && !is_full_palette(colour_lists.at(v))) {
                min_colour = colour_lists.at(v)[0];

            } else if (!full_palette.empty()) {
                min_colour = full_palette[0];

            }
            if (min_colour != -1) colouring[v] = min_colour;
        }
    }
}
