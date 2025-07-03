#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm> 

class StreamingConflictGraph {
public:
std::set<std::pair<int, int>> conflict_graph(
    const std::vector<std::pair<int, int>>& edges,
    const std::unordered_map<int, std::vector<int>>& colour_lists
) const;
};
