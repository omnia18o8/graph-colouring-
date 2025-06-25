#ifndef COLOURLISTS_H
#define COLOURLISTS_H

#include <vector>
#include <unordered_map>

std::unordered_map<int, int> getlistsizes(
    int V,
    int Delta, 
    const std::unordered_map<int, int>& pi,    // permutation map: vertex -> π(v)
    const std::vector<int>& vertices  // list of vertices (in permutation order)
);


std::unordered_map<int, std::vector<int>> assigncolours(
    int Delta,
    const std::unordered_map<int, int>& list_sizes,  // map: vertex -> list size
    const std::vector<int>& vertices
);

void fisher_yates(std::vector<int>& vec);

#endif
