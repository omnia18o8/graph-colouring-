#ifndef GRAPHINITIALISATION_H
#define GRAPHINITIALISATION_H

#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

namespace GraphInitialisation {
    extern int V, E, Delta;
    extern std::unordered_map<int, std::vector<int>> adj;
    void replace_commas_with_space(std::string& line);
    std::vector<std::pair<int, int>> load_edge_list(const std::string& filename);
    std::vector<int> get_vertices(const std::string& filename);
    std::vector<std::pair<int, int>> get_edges(const std::string& filename);
    void getdelta(const std::string& filename);
}

#endif
