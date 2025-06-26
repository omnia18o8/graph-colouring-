#include "conflictgraph.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <unordered_set>

static uint64_t mix(uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53;
    x ^= x >> 33;
    return x;
}

int StreamingConflictGraph::hash_edge(int c, int u, int v, int j) const {
    // Simple, independent hash for each sketch
    uint64_t seed = colour_hash_seeds.at(c)[j];
    uint64_t raw = mix(uint64_t(u) * 10007 + uint64_t(v) * 10009 + seed);
    return raw % w;
}

StreamingConflictGraph::StreamingConflictGraph(int max_colour, int num_nodes, int sketch_width)
    : w(sketch_width)
{
    t = (int)std::ceil(std::log2(num_nodes));
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_int_distribution<uint64_t> dist;

    for (int c = 1; c <= max_colour; ++c) {
        SketchSet ss;
        ss.count = std::vector<std::vector<int>>(t, std::vector<int>(w, 0));
        ss.sum_u = std::vector<std::vector<int>>(t, std::vector<int>(w, 0));
        ss.sum_v = std::vector<std::vector<int>>(t, std::vector<int>(w, 0));
        colour_sketches[c] = ss;
        // Each colour gets t random seeds
        std::vector<uint64_t> seeds(t);
        for (int j = 0; j < t; ++j) seeds[j] = dist(rng);
        colour_hash_seeds[c] = seeds;
    }
}

void StreamingConflictGraph::conflictsubgraphs(
    int u, int v,
    const std::unordered_map<int, std::vector<int>>& colour_lists,
    int delta
) {
    if (u == v) return;
    const auto& Lu = colour_lists.at(u);
    const auto& Lv = colour_lists.at(v);

    // O(1) check for shared colours
    std::unordered_set<int> Lu_set(Lu.begin(), Lu.end());
    for (int c : Lv) {
        if (Lu_set.count(c)) {
            SketchSet& ss = colour_sketches[c];
            for (int j = 0; j < t; ++j) {
                int b = hash_edge(c, u, v, j);
                ss.count[j][b] += delta;
                ss.sum_u[j][b] += (delta > 0 ? u : -u);
                ss.sum_v[j][b] += (delta > 0 ? v : -v);
            }
        }
    }
}

std::set<std::pair<int, int>> StreamingConflictGraph::conflictgraph() const {
    std::set<std::pair<int, int>> conflict_edges;
    for (const auto& [c, ss] : colour_sketches) {
        // Make copies for peeling
        auto count = ss.count;
        auto sum_u = ss.sum_u;
        auto sum_v = ss.sum_v;
        //const std::vector<uint64_t>& seeds = colour_hash_seeds.at(c);
        bool changed = true;
        while (changed) {
            changed = false;
            for (int j = 0; j < t; ++j) {
                for (int b = 0; b < w; ++b) {
                    if (count[j][b] == 1 || count[j][b] == -1) {
                        int u = sum_u[j][b];
                        int v = sum_v[j][b];
                        if (count[j][b] == -1) std::swap(u, v);
                        if (u > v) std::swap(u, v);
                        // Insert the recovered edge
                        conflict_edges.emplace(u, v);
                        // "Peel" this edge from all sketches for this colour
                        for (int jj = 0; jj < t; ++jj) {
                            int bb = hash_edge(c, u, v, jj);
                            count[jj][bb] -= 1;
                            sum_u[jj][bb] -= u;
                            sum_v[jj][bb] -= v;
                        }
                        changed = true;
                    }
                }
            }
        }
    }
    return conflict_edges;
}
