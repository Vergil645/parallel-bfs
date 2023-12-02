#ifndef __GRAPH_HH__
#define __GRAPH_HH__

#include <tuple>
#include <vector>

struct CubeGraph {
    CubeGraph() = default;

    explicit CubeGraph(std::size_t edge) : edge(edge) {}

    int size() const {
        return (edge + 1) * (edge + 1) * (edge + 1);
    }

    std::size_t node_to_vertex(std::size_t x, std::size_t y, std::size_t z) const {
        return x * (edge + 1) * (edge + 1) + y * (edge + 1) + z;
    }

    std::tuple<std::size_t, std::size_t, std::size_t> vertex_to_node(std::size_t u) const {
        std::size_t x = u / ((edge + 1) * (edge + 1));
        std::size_t y = (u / (edge + 1)) % (edge + 1);
        std::size_t z = u % (edge + 1);
        return {x, y, z};
    }

    std::vector<std::size_t> neighbours(std::size_t u) const {
        std::size_t x, y, z;
        std::vector<std::size_t> res;

        std::tie(x, y, z) = vertex_to_node(u);
        if (x < edge) res.push_back(node_to_vertex(x + 1, y, z));
        if (y < edge) res.push_back(node_to_vertex(x, y + 1, z));
        if (z < edge) res.push_back(node_to_vertex(x, y, z + 1));

        return res;
    }

    std::size_t neighbours_count(std::size_t u) const {
        std::size_t x, y, z;
        std::size_t res = 0;

        std::tie(x, y, z) = vertex_to_node(u);
        if (x < edge) ++res;
        if (y < edge) ++res;
        if (z < edge) ++res;

        return res;
    }

private:
    std::size_t edge;
};

#endif