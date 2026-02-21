#include "dijkstra.h"
#include <queue>
#include <limits>

std::vector<Step> Dijkstra::run(const Graph& graph, int source)
{
    int n = graph.size();
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    std::vector<bool> visited(n, false);

    std::vector<Step> steps;

    dist[source] = 0;

    using P = std::pair<int,int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, source});

    while(!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if(visited[u]) continue;
        visited[u] = true;

        steps.push_back({u, dist});

        for(const Edge& e : graph.getAdjList()[u]) {
            if(dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
                pq.push({dist[e.to], e.to});
            }
        }
    }

    return steps;
}
