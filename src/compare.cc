#include <chrono>
#include <iostream>
#include <vector>

#include <par.hh>
#include <seq.hh>
#include <test.hh>

#define RUN_COUNT 5
#define CUBE_EDGE 500

int main(void) {
    auto started = std::chrono::high_resolution_clock::now();
    auto done = started;
    int64_t time_seq;
    int64_t time_par;

    double ratio[RUN_COUNT];
    double ratio_sum = 0;
    double ratio_avg;

    test::TestData test_data = test::generate_test_data_of_cube_graph(CUBE_EDGE);
    std::vector<std::size_t> dist_seq;
    std::vector<std::size_t> dist_par;

    for (int run_no = 1; run_no <= RUN_COUNT; ++run_no) {
        // sequential BFS
        {
            started = std::chrono::high_resolution_clock::now();
            dist_seq = seq::bfs(test_data.graph, test_data.src_vertex);
            done = std::chrono::high_resolution_clock::now();

            time_seq = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();

            if (dist_seq != test_data.dist) {
                std::cout << "run[" << run_no << "] FAIL: incorrect seq::bfs dist" << std::endl;
                return 1;
            }
        }

        // parallel BFS
        {
            started = std::chrono::high_resolution_clock::now();
            dist_par = par::bfs(test_data.graph, test_data.src_vertex);
            done = std::chrono::high_resolution_clock::now();

            time_par = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();

            if (dist_par != test_data.dist) {
                std::cout << "run[" << run_no << "] FAIL: incorrect par::bfs dist" << std::endl;
                return 1;
            }
        }

        ratio[run_no] = (double) time_seq / time_par;
        ratio_sum += ratio[run_no];

        std::cout << "run[" << run_no << "] SUCCESS: par::bfs is " << ratio[run_no] << " times faster than seq::bfs" << std::endl;
    }
    ratio_avg = ratio_sum / RUN_COUNT;

    std::cout << "RESULT: par::bfs is faster than seq::bfs on average " << ratio_avg << " times" << std::endl;

    return 0;
}