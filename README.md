# parallel-bfs

Sequential and parallel implementation of a [BFS](https://en.wikipedia.org/wiki/Breadth-first_search) algorithm.

## Requirements

You need [CMake](https://cmake.org/) build system to build and run project code.

## Build

1. Clone project with submodules:

   ```sh
   $ git clone --recurse-submodules <...>
   ```

   or

   ```sh
   $ git clone <...>
   $ cd <project directory>
   $ git submodule update --init --recursive
   ```

2. Build:

   ```sh
   # without tests
   $ cmake -S. -Bbuild -DBUILD_TESTING=false && make -C build

   # with tests and run them
   $ cmake -S. -Bbuild -DBUILD_TESTING=true && make -C build && ctest --test-dir build
   ```

## Comparison of implementations

[compare](./main/compare.cc) compares execution time of both (sequential and parallel) implementations on **cube graph** with edge length **500** (**5** runs).

Run:

```sh
$ PARLAY_NUM_THREADS=<number of worker threads> ./bin/compare
```

Parallel implementation on **4** processes is **~2.45** times faster than sequential implementation:

```sh
$ PARLAY_NUM_THREADS=4 ./bin/compare
run[1] SUCCESS: par::bfs is 2.45453 times faster than seq::bfs
run[2] SUCCESS: par::bfs is 2.45185 times faster than seq::bfs
run[3] SUCCESS: par::bfs is 2.44475 times faster than seq::bfs
run[4] SUCCESS: par::bfs is 2.44563 times faster than seq::bfs
run[5] SUCCESS: par::bfs is 2.44986 times faster than seq::bfs
RESULT: par::bfs is faster than seq::bfs on average 2.44932 times
```