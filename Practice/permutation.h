#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>
#include <unordered_map>

void fisher_yates_shuffle(std::vector<int>& vec);

void randompermutation(
    const std::vector<int>& vertices,
    std::vector<int>& permuted_vertices,
    std::unordered_map<int, int>& position
);

#endif
