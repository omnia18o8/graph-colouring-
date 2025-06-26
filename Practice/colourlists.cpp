#include "colourlists.h"
#include <random>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_set>
#include <iostream>
#include <chrono>
#include <fstream>

std::unordered_map<int, int> getlistsizes(
    int V, 
    int Delta, 
    const std::unordered_map<int, int>& pi,
    const std::vector<int>& vertices
) {
    std::unordered_map<int, int> list_sizes;
    long double logn = std::log2(V);
    for (int v : vertices) {
        int piv = pi.at(v);
        int lv = static_cast<int>(std::floor((16.00 * V * logn) / piv));
        int lsize = std::min(Delta + 1, lv);
        list_sizes[v] = lsize;
    }
    return list_sizes;
}

void fisher_yates(std::vector<int>& vec) {
    int n = vec.size();
    for (int i = n - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(vec[i], vec[j]);
    }
}


std::unordered_map<int, std::vector<int>> assigncolours(
    int Delta,
    const std::unordered_map<int, int>& list_sizes,
    const std::vector<int>& vertices
) {
    std::unordered_map<int, std::vector<int>> colour_lists;
    std::srand(static_cast<unsigned int>(std::time(0))); //Why 0?

    for (int v : vertices) {
        int lsize = list_sizes.at(v);

        std::vector<int> palette(Delta + 1);
        std::iota(palette.begin(), palette.end(), 1);

        fisher_yates(palette);

        std::vector<int> selected(palette.begin(), palette.begin() + lsize);
        colour_lists[v] = selected;
    }
    return colour_lists;
}
