# heated_plate_openmp
Solves the 2D steady state heat equation by employing iteration. Uses OpenMP to exploit parallelism with a version for running on CPU and a version that offloads to a GPU target.

This project was adapted from John Burkardt's heated_plate_openmp to add OpenMP GPU offload support. His project is licenced under the GNU LGPL and can be found here:
https://people.math.sc.edu/Burkardt/c_src/heated_plate/heated_plate.html


## Results
    
    -----------------------------------100x100 Matrix
    sequential on cpu:
    Execution time = 0.032792s
    parallel (openmp) on cpu:
    Execution time = 0.026418s
    parallel (openmp) offloaded to gpu:
    Execution time = 0.318115s
    -----------------------------------500x500 Matrix
    sequential on cpu:
    Execution time = 5.845101s
    parallel (openmp) on cpu:
    Execution time = 1.090781s
    parallel (openmp) offloaded to gpu:
    Execution time = 3.338174s
    -----------------------------------1000x1000 Matrix
    sequential on cpu:
    Execution time = 27.344904s
    parallel (openmp) on cpu:
    Execution time = 17.867852s
    parallel (openmp) offloaded to gpu:
    Execution time = 6.532999s
    -----------------------------------5000x5000 Matrix
    parallel (openmp) on cpu:
    Execution time = 932.651375s
    parallel (openmp) offloaded to gpu:
    Execution time = 105.804726s



The above results were obtained on the following hardware:

CPU: Intel Core i7-9750H (6 core / 12 thread)

GPU: NVIDIA GeForce GTX 1660 Ti Mobile


## Getting Started

See the Makefile for building both the CPU and GPU versions. There is also a benchmark script (benchmark.sh) that will generate the output shown in the results section.

If you are getting segmentation faults on the openmp gpu offload version you may be running out of stack space. Running `ulimit -s unlimited` may prevent the program from crashing.
