#include "permutation.h"

void fisher_yates_shuffle(std::vector<int>& vec) {
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        int temp = vec[i];
        vec[i] = vec[j];
        vec[j] = temp;
    }
}

std::unordered_map<int, int> randompermutation(std::vector<int>& vec) {
    fisher_yates_shuffle(vec);
    std::unordered_map<int, int> position;
    for (size_t i = 0; i < vec.size(); ++i) {
        position[vec[i]] = i + 1;
    }
    return position;
}
