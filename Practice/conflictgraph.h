// conflictgraph.h

#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include <unordered_map>
#include <vector>
#include <set>
#include <utility> // for std::pair

class StreamingConflictGraph {
public:
    // Constructor
    StreamingConflictGraph();

    // Process an edge in the stream: (u, v) with per-vertex colour lists.
    // Should be called for each edge insertion (and, if needed, for deletion logic).
    void conflictsubgraphs(
        int u,
        int v,
        const std::unordered_map<int, std::vector<int>>& colour_lists
    );

    // Returns the union of all per-colour conflict edges.
    // Call only at the end of the stream.
    std::set<std::pair<int, int>> conflictgraph() const;

    // (Optional) For dynamic/deletion support, you could add:
    // void removeedge(int u, int v, const std::unordered_map<int, std::vector<int>>& colour_lists);

private:
    // For each colour, store a set of unique sorted vertex pairs (conflict edges)
    std::unordered_map<int, std::set<std::pair<int, int>>> sketch;
};

#endif // CONFLICTGRAPH_H
