#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include <vector>
#include <set>
#include <unordered_map>

class StreamingConflictGraph {
public:
    std::set<std::pair<int, int>> build_conflict_graph_from_edges(
        const std::vector<std::pair<int, int>>& edges,
        const std::unordered_map<int, std::vector<int>>& colour_lists
    ) const;
};


#endif
