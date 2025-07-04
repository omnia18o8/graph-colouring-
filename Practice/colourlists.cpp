#include "colourlists.h"
#include "permutation.h" 
std::unordered_map<int, int> getlistsizes(
    int V, 
    int Delta, 
    const std::unordered_map<int, int>& position,
    const std::vector<int>& vertices
) {
    std::unordered_map<int, int> list_sizes;
    list_sizes.reserve(vertices.size());
    double logn = std::log(V);
    double numerator = 40.0 * V * logn;

    for (int v : vertices) {
        int piv = position.at(v);
        int lv = static_cast<int>(numerator / piv);
        list_sizes[v] = std::min(Delta + 1, lv);
    }

    return list_sizes;
}
std::unordered_map<int, std::vector<int>> assigncolours(
    int Delta,
    const std::unordered_map<int, int>& list_sizes,
    const std::vector<int>& vertices
) {
    std::unordered_map<int, std::vector<int>> colour_lists;
    std::vector<int> palette(Delta + 1);
    std::iota(palette.begin(), palette.end(), 1);  // 1..Delta+1

    for (int v : vertices) {
        int lsize = list_sizes.at(v);

        if (lsize == Delta + 1) {
            colour_lists[v] = std::vector<int>{0}; // Full palette flag
        } else {
            std::vector<int> palette_copy = palette;
            fisher_yates_shuffle(palette_copy);
            std::vector<int> assigned(palette_copy.begin(), palette_copy.begin() + lsize);
            std::sort(assigned.begin(), assigned.end()); //Sort the assigned colours - might be a bottleneck
            colour_lists[v] = std::move(assigned); //use move to avoid copying
        }
    }
    return colour_lists;
}
