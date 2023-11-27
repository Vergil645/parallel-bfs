#include <chrono>
#include <iostream>
#include <vector>

#include "lib/graph/graph.hh"
#include "lib/seq/seq.hh"
#include "lib/par/par.hh"

#define RUN_COUNT 5
#define CUBE_EDGE 500

struct TestData {
    Graph graph;
    std::vector<std::size_t> dist;
    std::size_t src_vertex;
};

static std::size_t cube_node_to_vertex(std::size_t cube_edge, std::size_t x, std::size_t y, std::size_t z) {
    return x * (cube_edge + 1) * (cube_edge + 1) + y * (cube_edge + 1) + z;
}

static TestData generate_test_data_cube_graph(std::size_t cube_edge) {
    Graph graph;
    graph.adj_list.resize((cube_edge + 1) * (cube_edge + 1) * (cube_edge + 1), std::vector<std::size_t>());
    std::vector<std::size_t> dist(graph.size());

    for (std::size_t x = 0; x <= cube_edge; ++x) {
        for (std::size_t y = 0; y <= cube_edge; ++y) {
            for (std::size_t z = 0; z <= cube_edge; ++z) {
                auto u = cube_node_to_vertex(cube_edge, x, y, z);

                dist[u] = x + y + z;

                if (x < cube_edge) {
                    auto v = cube_node_to_vertex(cube_edge, x + 1, y, z);
                    graph.adj_list[u].push_back(v);
                }
                if (y < cube_edge) {
                    auto v = cube_node_to_vertex(cube_edge, x, y + 1, z);
                    graph.adj_list[u].push_back(v);
                }
                if (z < cube_edge) {
                    auto v = cube_node_to_vertex(cube_edge, x, y, z + 1);
                    graph.adj_list[u].push_back(v);
                }
            }
        }
    }

    return { graph, dist, cube_node_to_vertex(cube_edge, 0, 0, 0) };
}

int main(void) {
    auto started = std::chrono::steady_clock::now();
    auto done = started;
    int64_t time_seq;
    int64_t time_par;

    double ratio[RUN_COUNT];
    double ratio_sum = 0;
    double ratio_avg;

    TestData test_data = generate_test_data_cube_graph(CUBE_EDGE);
    std::vector<std::size_t> dist_seq;
    std::vector<std::size_t> dist_par;

    for (int run_no = 0; run_no < RUN_COUNT; ++run_no) {
        started = std::chrono::steady_clock::now();
        dist_seq = seq::bfs(test_data.graph, test_data.src_vertex);
        done = std::chrono::steady_clock::now();
        time_seq = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();

        if (dist_seq != test_data.dist) {
            std::cout << "run[" << run_no << "] FAIL: incorrect seq::bfs dist" << std::endl;
            return 1;
        }

        started = std::chrono::steady_clock::now();
        dist_par = par::bfs(test_data.graph, test_data.src_vertex);
        done = std::chrono::steady_clock::now();
        time_par = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();

        if (dist_par != test_data.dist) {
            std::cout << "run[" << run_no << "] FAIL: incorrect par::bfs dist" << std::endl;
            return 1;
        }

        ratio[run_no] = (double) time_seq / time_par;
        ratio_sum += ratio[run_no];

        std::cout << "run[" << run_no << "] SUCCESS: par::bfs is " << ratio[run_no] << " times faster than seq::bfs" << std::endl;
    }
    ratio_avg = ratio_sum / RUN_COUNT;

    std::cout << "RESULT: par::bfs is faster than seq::bfs on average " << ratio_avg << " times" << std::endl;

    return 0;
}