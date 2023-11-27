#include "par.hh"

#include <atomic>

#include "external/parlaylib/include/parlay/parallel.h"
#include "external/parlaylib/include/parlay/primitives.h"
#include "external/parlaylib/include/parlay/sequence.h"

namespace par {
    std::vector<std::size_t> bfs(const Graph &graph, std::size_t src_vertex) {
        auto reached = parlay::tabulate<std::atomic<bool>>(
            graph.size(),
            [&] (std::size_t _i) { return false; }
        );
        parlay::sequence<std::size_t> dist(graph.size(), INF);
        parlay::sequence<std::size_t> f = {src_vertex};

        reached[src_vertex] = true;
        dist[src_vertex] = 0;

        for (std::size_t d = 1; !f.empty(); ++d) {
            f = parlay::filter(
                parlay::flatten(parlay::map(f, [&] (auto u) { return graph.adj_list[u]; })),
                [&] (auto v) {
                    bool expected = false;
                    if (!reached[v] && reached[v].compare_exchange_strong(expected, true)) {
                        dist[v] = d;
                        return true;
                    }
                    return false;
                }
            );
        }

        return dist.to_vector();
    }
}