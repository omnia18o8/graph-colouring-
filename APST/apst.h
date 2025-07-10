#pragma once
#include <string>
#include <cstddef>
#include <iostream>

struct APSTResult {
    std::string filename;
    int nodes;
    int delta;
    double constant;
    int colours_used;
    double total_time;
    size_t total_memory;
    double avg_list_size;      // <--- NEW
    size_t conflict_edges;     // <--- NEW
};


APSTResult apst(const std::string& filename, double constant);

void save_apst_result_csv(const APSTResult& result, const std::string& csvfile) ;