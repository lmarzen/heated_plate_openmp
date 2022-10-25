#!/bin/sh

# This script is verifies the OpenMP versions of this project give the same 
# results as the sequential version.

# Function to run a test case.
# Parameters:
#   1 Test Case #
#   2 dimension M
#   3 dimension N
#   4 epsilon (error tolerance)
run_test () {
  echo "--------------------------------------------------------Test Case $1"
  DIM="-DM=$2 -DN=$3" make > /dev/null 2>&1
  ./heated_plate_sequential -q -e $4 -o heated_plate_sequential.out
  ./heated_plate_openmp_cpu -q -e $4 -o heated_plate_openmp_cpu.out
  if cmp --silent -- "heated_plate_sequential.out" "heated_plate_openmp_cpu.out"; then
    echo "PASSED heated_plate_openmp_cpu"
  else
    echo "FAILED heated_plate_openmp_cpu"
  fi
  ./heated_plate_openmp_offload -q -e $4 -o heated_plate_openmp_offload.out
  if cmp --silent -- "heated_plate_sequential.out" "heated_plate_openmp_offload.out"; then
    echo "PASSED heated_plate_openmp_offload"
  else
    echo "FAILED heated_plate_openmp_offload"
  fi
  make clean > /dev/null 2>&1
}


cp -r ../src .
cd src
make clean > /dev/null 2>&1

run_test 1 500 500 1.0e-3
run_test 2 250 250 1.0e-6
run_test 3 98  99  1.0e-12
run_test 4 100 251 1.0e-3
run_test 5 251 100 1.0e-3
run_test 6 10  10  1.0e-14
run_test 7 100 3   1.0e-7
run_test 8 3   100 1.0e-7
run_test 9 500 500 1.0e-6


cd ../
rm -r src
echo "Done."
