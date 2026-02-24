#include "graph.h"

Graph::Graph(int n) {
    adjList.resize(n);
}
    //adds edge defined in mainwindow
void Graph::addEdge(int u, int v, int weight) {
    adjList[u].push_back({v, weight});
    adjList[v].push_back({u, weight});
}

int Graph::size() const {
    return adjList.size();
}

const std::vector<std::vector<Edge>>& Graph::getAdjList() const {
    return adjList;
}
