#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <cmath>

std::unordered_map<int, int> getlistsizes(
    const std::unordered_map<int, int>& position,
    const std::vector<int>& vertices,
    double constant
);


std::unordered_map<int, std::vector<int>> assigncolours(
    int Delta,
    const std::unordered_map<int, int>& list_sizes,
    const std::vector<int>& vertices
);
