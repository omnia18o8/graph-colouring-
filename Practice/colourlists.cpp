#include "colourlists.h"
#include "permutation.h" 
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>

std::unordered_map<int, int> getlistsizes(
    int V, 
    int Delta, 
    const std::unordered_map<int, int>& position,
    const std::vector<int>& vertices
) {
    std::unordered_map<int, int> list_sizes;
    double logn = std::log(V);
    for (int v : vertices) {
        int piv = position.at(v);
        int lv = static_cast<int>(std::floor((40.0 * V * logn) / piv)); //round down or up?
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

    for (int v : vertices) {
        int lsize = list_sizes.at(v);

        std::vector<int> palette(Delta + 1);
        std::iota(palette.begin(), palette.end(), 1);
        fisher_yates_shuffle(palette);

        std::vector<int> selected(palette.begin(), palette.begin() + lsize);
        colour_lists[v] = selected;


        // std::cout << "[assigncolours] Vertex " << v
        //           << " | lsize = " << lsize
        //           << " | assigned: [";
        // for (int i = 0; i < lsize; ++i) {
        //     std::cout << selected[i];
        //     if (i < lsize - 1) std::cout << ", ";
        // }
        // std::cout << "]\n";
    }
    return colour_lists;
}