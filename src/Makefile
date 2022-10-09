CC = clang
FFLAGS = -O3 -Wall

all: cpu gpu

cpu: heated_plate_openmp.c
	$(CC) heated_plate_openmp.c $(FFLAGS) -o heated_plate_cpu -fopenmp
gpu: heated_plate_openmp_offload.c
	$(CC) heated_plate_openmp_offload.c $(FFLAGS) -o heated_plate_gpu -fopenmp -fopenmp-targets=nvptx64 -L/home/user/offload/clang/install/lib

clean:
	rm -f *- heated_plate_cpu heated_plate_gpu core