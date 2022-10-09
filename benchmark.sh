#!/bin/sh

cd src
make clean > /dev/null 2>&1

for SZ in 100 500 1000 5000
do
  echo "${SZ}x${SZ} Matrix--------"
  DIM="-DM=$SZ -DN=$SZ" make > /dev/null 2>&1
  cal_t=$((time OMP_NUM_THREADS=1 ./heated_plate_cpu)  2>&1 > /dev/null | grep real | awk '{print $2}')
  echo "sequential on cpu   $cal_t"
  cal_t=$((time ./heated_plate_cpu)  2>&1 > /dev/null | grep real | awk '{print $2}')
  echo "parallel on cpu     $cal_t"
  cal_t=$((time ./heated_plate_gpu)  2>&1 > /dev/null | grep real | awk '{print $2}')
  echo "parallel on gpu     $cal_t"
  make clean > /dev/null 2>&1
done

cd ../
echo "Done."
