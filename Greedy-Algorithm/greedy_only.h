#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <sys/resource.h>
#include <algorithm>
#include <numeric>

struct GreedyResult {
    std::string filename;
    int nodes;
    int delta;
    double constant;
    int colours_used;
    double total_time;
    size_t total_memory;
    double avg_list_size;
    size_t conflict_edges;
    bool success;
};

GreedyResult greedy(const std::string& filename);
