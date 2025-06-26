#ifndef CONFLICTGRAPH_H
#define CONFLICTGRAPH_H

#include <unordered_map>
#include <vector>
#include <set>

class StreamingConflictGraph {
public:
    StreamingConflictGraph(int max_colour, int num_nodes, int sketch_width = 4096);

    void conflictsubgraphs(int u, int v, const std::unordered_map<int, std::vector<int>>& colour_lists, int delta = +1);
    std::set<std::pair<int, int>> conflictgraph() const;

private:
    struct SketchSet {
        std::vector<std::vector<int>> count, sum_u, sum_v;
    };

    int t; // Number of sketches (t = ceil(log2 n))
    int w; // Sketch width (buckets per sketch)
    std::unordered_map<int, SketchSet> colour_sketches;
    std::unordered_map<int, std::vector<uint64_t>> colour_hash_seeds; // [t] seeds per colour

    // Hash function for (u,v) in sketch j for colour c
    int hash_edge(int c, int u, int v, int j) const;
};

#endif
