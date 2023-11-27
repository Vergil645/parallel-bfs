#include "seq.hh"

#include <queue>

namespace seq {
    std::vector<std::size_t> bfs(const Graph &graph, std::size_t src_vertex) {
        std::vector<std::size_t> dist(graph.size(), INF);
        std::queue<std::size_t> vertices_queue;

        dist[src_vertex] = 0;
        vertices_queue.push(src_vertex);

        while (!vertices_queue.empty()) {
            auto u = vertices_queue.front();
            vertices_queue.pop();

            for (auto v : graph.adj_list[u]) {
                if (dist[v] == INF) {
                    dist[v] = dist[u] + 1;
                    vertices_queue.push(v);
                }
            }
        }

        return dist;
    }
}