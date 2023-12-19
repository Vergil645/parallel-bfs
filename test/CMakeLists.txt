set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

project(parallel-bfs_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES "src")

add_executable(test_seq_bfs "${SOURCES}/test_seq_bfs.cc")
add_executable(test_par_bfs "${SOURCES}/test_par_bfs.cc")

# ===== TESTS =====

add_test(NAME test_seq_bfs COMMAND test_seq_bfs)
add_test(NAME test_par_bfs COMMAND test_par_bfs)