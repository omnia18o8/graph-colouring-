#include "permutation.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>

void fisher_yates_shuffle(std::vector<int>& vec) {
    for (int i = static_cast<int>(vec.size()) - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(vec[i], vec[j]);
    }
}

void randompermutation(
    const std::vector<int>& vertices,
    std::vector<int>& permuted_vertices,
    std::unordered_map<int, int>& position
) {
    permuted_vertices = vertices;
    std::srand(static_cast<unsigned int>(std::time(0))); // Seed once per run
    fisher_yates_shuffle(permuted_vertices);

    position.clear();
    for (size_t i = 0; i < permuted_vertices.size(); ++i) {
        position[permuted_vertices[i]] = static_cast<int>(i) + 1;
    }
}
