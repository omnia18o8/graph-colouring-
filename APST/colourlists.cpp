#include "colourlists.h"
#include "graphinitialisation.h"

std::unordered_map<int, int> getlistsizes(
    const std::unordered_map<int, int>& position,
    const std::vector<int>& vertices,
    double constant
) {
    std::unordered_map<int, int> list_sizes;
    int V = vertices.size();
    int Delta = GraphInitialisation::Delta;
    list_sizes.reserve(vertices.size());
    double numerator = constant * V * std::log(V);

    for (int v : vertices) {
        int piv = position.at(v);
        int lv = static_cast<int>(numerator / piv);
        list_sizes[v] = std::min(Delta + 1, lv);
    }
    return list_sizes;
}


// Xoshiro256++ PRNG
static uint64_t s[4] = { 123456789, 362436069, 521288629, 88675123 };

inline uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

uint64_t xoshiro256pp() {
    uint64_t result = rotl(s[0] + s[3], 23) + s[0];
    uint64_t t = s[1] << 17;

    s[2] ^= s[0]; s[3] ^= s[1];
    s[1] ^= s[2]; s[0] ^= s[3];

    s[2] ^= t;
    s[3] = rotl(s[3], 45);

    return result;
}

inline int fast_rand(int bound) {
    return xoshiro256pp() % bound;
}

// Reusable buffers
static std::vector<int> shared_palette_buffer;
static std::vector<int> shared_local_buffer;
static std::vector<int> shared_assigned;

std::vector<int> sample_small_palette(int palette_size, int lsize) {
    shared_assigned.clear();
    shared_assigned.reserve(lsize);

    if ((int)shared_palette_buffer.size() != palette_size) {
        shared_palette_buffer.resize(palette_size);
        std::iota(shared_palette_buffer.begin(), shared_palette_buffer.end(), 1);
    }

    shared_local_buffer = shared_palette_buffer;

    for (int i = 0; i < lsize; ++i) {
        int j = i + fast_rand(palette_size - i);
        std::swap(shared_local_buffer[i], shared_local_buffer[j]);
        shared_assigned.push_back(shared_local_buffer[i]);
    }

    return shared_assigned;
}

std::vector<int> sample_large_palette(int palette_size, int lsize) {
    shared_assigned.clear();
    shared_assigned.reserve(lsize);

    if ((int)shared_palette_buffer.size() != palette_size) {
        shared_palette_buffer.resize(palette_size);
        std::iota(shared_palette_buffer.begin(), shared_palette_buffer.end(), 1);
    }

    shared_local_buffer = shared_palette_buffer;

    int exclude_count = palette_size - lsize;
    for (int i = 0; i < exclude_count; ++i) {
        int j = i + rand() % (palette_size - i);
        std::swap(shared_local_buffer[i], shared_local_buffer[j]);
    }

    shared_assigned.insert(shared_assigned.end(),
        shared_local_buffer.begin() + exclude_count,
        shared_local_buffer.end());

    return shared_assigned;
}

std::unordered_map<int, std::vector<int>> assigncolours(
    int Delta,
    const std::unordered_map<int, int>& list_sizes,
    const std::vector<int>& vertices
) {
    int palette_size = Delta + 1;

    std::unordered_map<int, std::vector<int>> colour_lists;
    colour_lists.reserve(vertices.size());

    for (int v : vertices) {
        int lsize = list_sizes.at(v);

        if (lsize == palette_size) {
            colour_lists[v] = std::vector<int>{0};
        } else if (lsize <= palette_size / 2) {
            colour_lists[v] = sample_small_palette(palette_size, lsize);
        } else {
            colour_lists[v] = sample_large_palette(palette_size, lsize);
        }
    }

    return colour_lists;
}
