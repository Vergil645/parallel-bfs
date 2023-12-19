#ifndef __SEQ_HH__
#define __SEQ_HH__

#include <queue>
#include <limits>
#include <vector>

namespace seq {
    const std::size_t INF = std::numeric_limits<std::size_t>::max();

    template<typename G>
    std::vector<std::size_t> bfs(const G &graph, std::size_t src_vertex) {
        std::vector<bool> reached(graph.size(), false);
        std::vector<std::size_t> dist(graph.size(), INF);
        std::queue<std::size_t> vertices_queue;
        std::size_t *neighbours = new std::size_t[graph.max_neighbours_count()];
        std::size_t n_count;

        reached[src_vertex] = true;
        dist[src_vertex] = 0;
        vertices_queue.push(src_vertex);

        while (!vertices_queue.empty()) {
            auto u = vertices_queue.front();
            vertices_queue.pop();

            graph.neighbours(u, neighbours, n_count);

            for (std::size_t i = 0; i < n_count; ++i) {
                auto v = neighbours[i];
                if (!reached[v]) {
                    reached[v] = true;
                    dist[v] = dist[u] + 1;
                    vertices_queue.push(v);
                }
            }
        }

        delete[] neighbours;

        return dist;
    }
}

#endif