#ifndef __PAR_HH__
#define __PAR_HH__

#include "lib/graph/graph.hh"

#include <limits>
#include <vector>

namespace par {
    const std::size_t INF = std::numeric_limits<std::size_t>::max();

    std::vector<std::size_t> bfs(const Graph &graph, std::size_t src_vertex);
}

#endif