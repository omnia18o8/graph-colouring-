#ifndef GRAPHINITIALISATION_H
#define GRAPHINITIALISATION_H

#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

namespace GraphInitialisation {
    extern int V, E, Delta;
    extern std::unordered_map<int, std::vector<int>> adj;

    std::vector<int> get_vertices(const std::string& filename);
    std::vector<std::pair<int, int>> get_edges(const std::string& filename);
    void getdelta(const std::string& filename);
}

#endif
