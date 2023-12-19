#ifndef __TEST_HH__
#define __TEST_HH__

#include <vector>

#include "graph.hh"

namespace test {
    struct TestData {
        CubeGraph graph;
        std::vector<std::size_t> dist;
        std::size_t src_vertex;
    };

    static TestData generate_test_data_of_cube_graph(std::size_t cube_edge) {
        CubeGraph graph(cube_edge);
        std::vector<std::size_t> dist(graph.size());

        for (std::size_t x = 0; x <= cube_edge; ++x) {
            for (std::size_t y = 0; y <= cube_edge; ++y) {
                for (std::size_t z = 0; z <= cube_edge; ++z) {
                    auto u = graph.node_to_vertex(x, y, z);
                    dist[u] = x + y + z;
                }
            }
        }

        return { graph, dist, graph.node_to_vertex(0, 0, 0) };
}
}

#endif