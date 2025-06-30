#ifndef COLOURLISTS_H
#define COLOURLISTS_H

#include <vector>
#include <unordered_map>

std::unordered_map<int, int> getlistsizes(
    int V,
    int Delta, 
    const std::unordered_map<int, int>& pi,    // permutation map: vertex -> position
    const std::vector<int>& vertices  // list of vertices from permutation
);

std::unordered_map<int, std::vector<int>> assigncolours(
    int Delta,
    const std::unordered_map<int, int>& list_sizes,  // map: vertex to list size
    const std::vector<int>& vertices
);

#endif
