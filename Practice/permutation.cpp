#include "permutation.h"
#include <algorithm>
#include <cstdlib>      // for rand, srand
#include <ctime>        // for time

void randompermutation(
    std::vector<int>& vertices,
    std::vector<int>& permuted_vertices,
    std::unordered_map<int, int>& position
) {
    permuted_vertices = vertices;
    std::srand(std::time(0)); // Seed the random number generator

    // Fisher-Yates shuffle from Geeks for Geeks 
    for (int i = static_cast<int>(permuted_vertices.size()) - 1; i > 0; --i) { // Start from the last element and swap it with a random element before it
        int j = std::rand() % (i + 1); // Generate a random index from 0 to i
        std::swap(permuted_vertices[i], permuted_vertices[j]);
    }

    // Map each vertex to its permutation position (1-based index)
    position.clear();
    for (size_t i = 0; i < permuted_vertices.size(); ++i) {
        position[permuted_vertices[i]] = static_cast<int>(i) + 1;
    }
}
