FLAGS=-fopenmp -O2 -march=native
CC=gcc
CXX=g++

all: ver reduction hello reduction-simd info vectoradd-simd reduction-simd-typed monte-carlo-pi

hello: omp_hello.c
	$(CC) $(FLAGS) omp_hello.c -o omp_hello

ver: omp_ver.cc
	$(CXX) $(FLAGS) omp_ver.cc -o omp_ver
	  
reduction: omp_reduction.cc
	$(CXX) $(FLAGS) omp_reduction.cc -o omp_reduction

info: omp_getEnvInfo.c
	$(CC) $(FLAGS) omp_getEnvInfo.c -o omp_getEnvInfo
