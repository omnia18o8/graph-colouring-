#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <climits>
#include <numeric>
#include <iostream>

std::unordered_map<int, std::vector<int>> adjency_list(
    const std::vector<int>& vertices,
    const std::set<std::pair<int, int>>& conflict_edges
);

std::vector<int> vertex_order(
     std::vector<int>& vertices,
    const std::unordered_map<int, int>& permutation,
    const std::unordered_map<int, int>& list_sizes
);

std::unordered_map<int, int> conflict_greedy(
    const std::vector<int>& order,
    const std::unordered_map<int, std::vector<int>>& adj,
    const std::unordered_map<int, std::vector<int>>& colour_lists
);

void full_greedy(
    const std::vector<int>& all_vertices,
    const std::unordered_map<int, std::vector<int>>& conflict_adj,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    std::unordered_map<int, int>& colouring
);

