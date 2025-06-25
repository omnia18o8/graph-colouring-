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
    long double logn = std::log(V);
    for (int v : vertices) {
        int piv = pi.at(v);
        int lv = static_cast<int>(std::floor((40.00 * V * logn) / piv));
        int lsize = std::min(Delta + 1, lv);
        list_sizes[v] = lsize;

        // Print statement for debugging
        std::cout << "Vertex: " << v 
                  << " | permutation (pi): " << piv
                  << " | list size: " << lsize 
                  << std::endl;
    }
    return list_sizes;
}


std::unordered_map<int, std::vector<int>> assigncolours(
    int Delta,
    const std::unordered_map<int, int>& list_sizes,
    const std::vector<int>& vertices
) {
    std::unordered_map<int, std::vector<int>> colour_lists;

    // Seed the random number generator once
    std::srand(static_cast<unsigned int>(std::time(0)));

    for (int v : vertices) {
        int lsize = list_sizes.at(v);

        // Create a palette of colours from 1 to Delta + 1
        std::vector<int> palette(Delta + 1);
        std::iota(palette.begin(), palette.end(), 1);

        // Fisher-Yates shuffle (manual version using rand())
        for (int i = Delta; i > 0; --i) {
            int j = std::rand() % (i + 1);
            std::swap(palette[i], palette[j]);
        }
        // Select the first lsize colours from the shuffled palette
        std::vector<int> selected(palette.begin(), palette.begin() + lsize);;
        colour_lists[v] = selected;
    }
    return colour_lists;
}

