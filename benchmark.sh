#!/bin/sh

cd src
make clean > /dev/null 2>&1

for SZ in 100 500 1000
do
  echo "-----------------------------------${SZ}x${SZ} Matrix"
  DIM="-DM=$SZ -DN=$SZ" make > /dev/null 2>&1
  echo "sequential on cpu:"
  ./heated_plate_sequential -qt
  echo "parallel (openmp) on cpu:"
  ./heated_plate_openmp_cpu -qt
  echo "parallel (openmp) offloaded to gpu:"
  ./heated_plate_openmp_offload -qt
  make clean > /dev/null 2>&1
done

SZ=5000
echo "-----------------------------------${SZ}x${SZ} Matrix"
DIM="-DM=$SZ -DN=$SZ" make > /dev/null 2>&1
echo "parallel (openmp) on cpu:"
./heated_plate_openmp_cpu -qt
echo "parallel (openmp) offloaded to gpu:"
./heated_plate_openmp_offload -qt
make clean > /dev/null 2>&1

cd ../
echo "Done."
