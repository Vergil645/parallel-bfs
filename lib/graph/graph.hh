#ifndef __GRAPH_HH__
#define __GRAPH_HH__

#include <vector>

struct Graph {
    std::vector<std::vector<std::size_t>> adj_list;

    Graph() = default;

    explicit Graph(const std::vector<std::vector<std::size_t>> &adj_list) : adj_list(adj_list) {}

    int size() const {
        return this->adj_list.size();
    }
};

#endif