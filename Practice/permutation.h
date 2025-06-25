#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>
#include <unordered_map>

void randompermutation(
    std::vector<int>& vertices,
    std::vector<int>& permuted_vertices, //new order of vertices
    std::unordered_map<int, int>& position //hashmap node:position
);

#endif
