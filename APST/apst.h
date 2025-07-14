#pragma once
#include <string>

struct APSTResult {
    std::string filename;
    int nodes;
    int delta;
    double constant;
    bool success;
    int colours_used;
    double total_time;
    size_t total_memory;
    double avg_list_size;
    size_t conflict_edges;
};

APSTResult apst(const std::string& filename, double constant);
