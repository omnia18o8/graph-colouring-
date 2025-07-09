#include "permutation.h"

void fisher_yates_shuffle(std::vector<int>& vec) {
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(vec[i], vec[j]);
    }
}

std::unordered_map<int, int> randompermutation(std::vector<int>& vec) {
    fisher_yates_shuffle(vec);
    std::unordered_map<int, int> position;
    position.reserve(vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
        position[vec[i]] = static_cast<int>(i + 1);
    }
    return position;
}
