#include <par.hh>
#include <test.hh>

#define TEST_CASES 5

static std::size_t cube_edges[TEST_CASES] = {0, 1, 3, 10, 100};

int main(void) {
    std::vector<std::size_t> dist_par;

    for (int test_case = 0; test_case < TEST_CASES; ++test_case) {
        std::size_t cube_edge = cube_edges[test_case];
        test::TestData test_data = test::generate_test_data_of_cube_graph(cube_edge);

        dist_par = par::bfs(test_data.graph, test_data.src_vertex);

        if (dist_par != test_data.dist) {
            return 1;
        }
    }

    return 0;
}