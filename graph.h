#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

struct Edge {
    int to;
    int weight;
};

class Graph {
public:
    Graph(int n);

    void addEdge(int u, int v, int weight);

    int size() const;
    const std::vector<std::vector<Edge>>& getAdjList() const;

private:
    std::vector<std::vector<Edge>> adjList;
};

#endif
