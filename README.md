# heated_plate_openmp
Solves the 2D steady state heat equation by employing iteration. Uses OpenMP to exploit parallelism with a version for running on CPU and a version that offloads to a GPU target.

This project was adapted from John Burkardt's heated_plate_openmp to add OpenMP GPU offload support. His project is licenced under the GNU LGPL and can be found here:
https://people.math.sc.edu/Burkardt/c_src/heated_plate/heated_plate.html


## Results

    -----------------------------------100x100 Matrix
    sequential on cpu:
    Execution time = 0.087342s
    parallel (openmp) on cpu:
    Execution time = 0.043451s
    parallel (openmp) offloaded to gpu:
    Execution time = 0.351723s
    -----------------------------------500x500 Matrix
    sequential on cpu:
    Execution time = 6.560885s
    parallel (openmp) on cpu:
    Execution time = 1.165024s
    parallel (openmp) offloaded to gpu:
    Execution time = 5.074190s
    -----------------------------------1000x1000 Matrix
    sequential on cpu:
    Execution time = 30.760907s
    parallel (openmp) on cpu:
    Execution time = 18.456771s
    parallel (openmp) offloaded to gpu:
    Execution time = 13.143397s
    -----------------------------------5000x5000 Matrix
    parallel (openmp) on cpu:
    Execution time = 926.795656s
    parallel (openmp) offloaded to gpu:
    Execution time = 279.438284s


The above results were obtained on the following hardware:

CPU: Intel Core i7-9750H (6 core / 12 thread)

GPU: NVIDIA GeForce GTX 1660 Ti Mobile


## Getting Started

See the Makefile for building both the CPU and GPU versions. There is also a benchmark script (benchmark.sh) that will generate the output shown in the results section.