#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include <unordered_map>
#include <vector>
#include <set>
#include <utility> 

class StreamingConflictGraph {
public:
    StreamingConflictGraph(); // Constructor

    // Process an edge in the stream: (u, v) with per-vertex colour lists.
    void conflictsubgraphs(
        int u,
        int v,
        const std::unordered_map<int, std::vector<int>>& colour_lists
    );
    
    std::set<std::pair<int, int>> conflictgraph() const; // Return the set of conflict edges as a set of unique pairs (u, v)

private:
    // For each colour, store a set of unique sorted vertex pairs (conflict edges)
    std::unordered_map<int, std::set<std::pair<int, int>>> sketch;
};

#endif 
