#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include <vector>

struct Step {
    int currentNode;
    std::vector<int> dist;
};

class Dijkstra {
public:
    static std::vector<Step> run(const Graph& graph, int source);
};

#endif
