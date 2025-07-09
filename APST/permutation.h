#pragma once
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <algorithm>  

void fisher_yates_shuffle(std::vector<int>& vec);
std::unordered_map<int, int> randompermutation(std::vector<int>& vec);
