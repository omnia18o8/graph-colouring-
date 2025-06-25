#ifndef GRAPHINITIALISATION_H
#define GRAPHINITIALISATION_H

#include <unordered_map>
#include <vector>

namespace GraphInitialisation {
    extern int V;
    extern int E;
    extern int Delta;

    extern std::unordered_map<int, std::vector<int>> adj;

    //extern makes it global

    void getdelta(const std::string& filename, bool directed = false);
    // directed parameter allows to specify if the graph is directed or undirected
}

#endif