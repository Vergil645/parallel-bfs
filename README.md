# parallel-bfs

Sequential and parallel implementation of a [BFS](https://en.wikipedia.org/wiki/Breadth-first_search) algorithm.

## Requirements

You need [Bazel](https://bazel.build/) build system to build and run project code.

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

2. Build all:

   ```sh
   $ bazel build //...
   ```

## Comparison of implementations

[compare](./main/compare.cc) compares execution time of both (sequential and parallel) implementations on **cube graph** with edge length **500** (**5** runs).

Run:

```sh
$ PARLAY_NUM_THREADS=<number of worker threads> bazel run //main:compare
```

Parallel implementation on **4** processes is **~2.6** times faster than sequential implementation:

```sh
$ PARLAY_NUM_THREADS=4 bazel run //main:compare
INFO: ...
run[0] SUCCESS: par::bfs is 2.63673 times faster than seq::bfs
run[1] SUCCESS: par::bfs is 2.56684 times faster than seq::bfs
run[2] SUCCESS: par::bfs is 2.60592 times faster than seq::bfs
run[3] SUCCESS: par::bfs is 2.58344 times faster than seq::bfs
run[4] SUCCESS: par::bfs is 2.56865 times faster than seq::bfs
RESULT: par::bfs is faster than seq::bfs on average 2.59232 times
```