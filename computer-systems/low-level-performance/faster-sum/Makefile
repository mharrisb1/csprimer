# NOTE: this assumes Google Benchmark is installed system wide
#
# See https://github.com/google/benchmark for installation instructions
#
# If Installed locally, use flags -isystem benchmark/include -Lbenchmark/build/src
# to g++


.ALL: run_benchmark

run_benchmark: bench
	./bench

bench: benchmark.cc sum.o
	g++ sum.o benchmark.cc -std=c++11 -lbenchmark -lpthread -o bench

sum.o: sum.c
	cc -O1 -c sum.c	
