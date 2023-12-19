#ifndef __PAR_HH__
#define __PAR_HH__

#include <atomic>
#include <limits>
#include <vector>

#include <parlay/parallel.h>
#include <parlay/primitives.h>
#include <parlay/sequence.h>

namespace par {
    const std::size_t INF = std::numeric_limits<std::size_t>::max();

    template<typename G>
    std::vector<std::size_t> bfs(const G &graph, std::size_t src_vertex) {
        std::vector<std::atomic<bool>> reached(graph.size());
        std::vector<std::size_t> dist(graph.size(), INF);
        parlay::sequence<std::size_t> frontier;
        parlay::sequence<std::size_t> frontier_next;
        parlay::sequence<std::size_t> deg_pref_sum;
        std::size_t frontier_size;
        std::size_t frontier_next_size;

        reached[src_vertex] = true;
        dist[src_vertex] = 0;
        frontier = {src_vertex};

        for (std::size_t d = 1; !frontier.empty(); ++d) {
            deg_pref_sum.resize(frontier.size());
            parlay::parallel_for(0, frontier.size(), [&] (auto i) {
                deg_pref_sum[i] = graph.neighbours_count(frontier[i]);
            });
            frontier_next_size = parlay::scan_inplace(deg_pref_sum);

            frontier_next.resize(frontier_next_size);
            parlay::parallel_for(0, frontier.size(), [&] (auto i) {
                auto neighbours = graph.neighbours(frontier[i]);

                for (std::size_t j = 0; j < neighbours.size(); ++j) {
                    std::size_t idx_start = deg_pref_sum[i];
                    auto v = neighbours[j];
                    bool expected = false;

                    if (reached[v].compare_exchange_strong(expected, true)) {
                        dist[v] = d;
                        frontier_next[idx_start + j] = v;
                    } else {
                        frontier_next[idx_start + j] = INF;
                    }
                }
            });

            frontier.resize(frontier_next_size);
            frontier_size = parlay::filter_into_uninitialized(frontier_next, frontier, [&] (auto v) {
                return v != INF;
            });
            frontier.resize(frontier_size);
        }

        return dist;
    }
}

#endif