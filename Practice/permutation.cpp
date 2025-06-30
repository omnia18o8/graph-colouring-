#include "permutation.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

void fisher_yates_shuffle(std::vector<int>& vec) {
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        int temp = vec[i];
        vec[i] = vec[j];
        vec[j] = temp;
    }
}


void randompermutation(
    const std::vector<int>& vertices,
    std::vector<int>& permuted_vertices,
    std::unordered_map<int, int>& position
) {
    permuted_vertices = vertices;
    fisher_yates_shuffle(permuted_vertices);

    for (size_t i = 0; i < permuted_vertices.size(); ++i) {
        position[permuted_vertices[i]] = (i) + 1;
    }
}
