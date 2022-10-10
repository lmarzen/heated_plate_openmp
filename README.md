# heated_plate_openmp
Solves the 2D steady state heat equation by employing iteration. Uses OpenMP to exploit parallelism with a version for running on CPU and a version that offloads to a GPU target.

This project was adapted from John Burkardt's heated_plate_openmp to add OpenMP GPU offload support. His project is licenced under the GNU LGPL and can be found here:
https://people.math.sc.edu/Burkardt/cpp_src/heated_plate/heated_plate.html


## Results

    100x100 Matrix--------
    openmp cpu     0m0.028s
    openmp gpu     0m0.441s
    500x500 Matrix--------
    openmp cpu     0m1.265s
    openmp gpu     0m5.537s
    1000x1000 Matrix--------
    openmp cpu     0m19.370s
    openmp gpu     0m13.651s
    5000x5000 Matrix--------
    openmp cpu     15m56.474s
    openmp gpu     4m41.812s
    10000x10000 Matrix--------
    openmp cpu     61m5.753s
    openmp gpu     23m7.674s

The above results were obtained on the following hardware:

CPU: Intel Core i7-9750H (6 core / 12 thread)

GPU: NVIDIA GeForce GTX 1660 Ti Mobile


## Getting Started

See the Makefile for building both the CPU and GPU versions. There is also a benchmark script (benchmark.sh) that will generate the output shown in the results section.