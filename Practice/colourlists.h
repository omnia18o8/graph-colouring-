#ifndef COLOURLISTS_H
#define COLOURLISTS_H

#include <vector>
#include <unordered_map>

std::unordered_map<int, int> getlistsizes(
    int V,
    int Delta, 
    const std::unordered_map<int, int>& pi, // map of V: pi(v)
    const std::vector<int>& vertices //list of vertices (used for permuted vertices)
);

std::unordered_map<int, std::vector<int>> assigncolours(
    int Delta,
    const std::unordered_map<int, int>& list_sizes, //map of list sizes for each vertex
    const std::vector<int>& vertices
);

#endif 
