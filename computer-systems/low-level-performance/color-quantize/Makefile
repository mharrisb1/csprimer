# NOTE: this assumes Google Benchmark is installed system wide
#
# See https://github.com/google/benchmark for installation instructions
#
# If Installed locally, use flags -isystem benchmark/include -Lbenchmark/build/src
# to g++


.PHONY: run_bench clean
	
run_bench: bench
	./bench

convert: convert.o quantize.o
	cc -O1 quantize.o convert.o -o convert

bench: benchmark.cc quantize.o
	g++ quantize.o benchmark.cc -std=c++11 -lbenchmark -lpthread -o bench

quantize.o: quantize.c
	cc -O1 -c quantize.c

convert.o: convert.c
	cc -O1 -c convert.c

clean:
	rm *.o
	rm -f bench
	rm -f convert
