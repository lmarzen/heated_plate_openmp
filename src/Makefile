CC=/path/to/clang/install/bin/clang
FFLAGS=-O3 -Wall

all: sequential openmp_cpu openmp_offload

sequential: heated_plate_sequential.c
	$(CC) heated_plate_sequential.c $(FFLAGS) $(DIM) -o heated_plate_sequential
openmp_cpu: heated_plate_openmp_cpu.c
	$(CC) heated_plate_openmp_cpu.c $(FFLAGS) $(DIM) -o heated_plate_openmp_cpu -fopenmp
openmp_offload: heated_plate_openmp_offload.c
	$(CC) heated_plate_openmp_offload.c $(FFLAGS) $(DIM) -o heated_plate_openmp_offload -fopenmp -fopenmp-targets=nvptx64 -L/path/to/clang/install/lib

clean:
	rm -f *- heated_plate_sequential heated_plate_openmp_cpu heated_plate_openmp_offload *.out core
