#ifndef __SEQ_HH__
#define __SEQ_HH__

#include "lib/graph/graph.hh"

#include <limits>
#include <vector>

namespace seq {
    const std::size_t INF = std::numeric_limits<std::size_t>::max();

    std::vector<std::size_t> bfs(const Graph &graph, std::size_t src_vertex);
}

#endif