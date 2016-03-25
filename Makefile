FLAGS=-O2 -std=c++11

all: build_benchmark query_gen build_gen

build_benchmark: BuildBenchmark.cpp bitmap.hpp wavelet-matrix.hpp wavelet-tree.hpp utils.hpp
	$(CXX) $(FLAGS) -lboost_timer BuildBenchmark.cpp -o build_benchmark

build_gen: BuildGenerator.cpp utils.hpp
	$(CXX) $(FLAGS) BuildGenerator.cpp -o build_gen

query_gen: QueryGenerator.cpp utils.hpp
	$(CXX) $(FLAGS) QueryGenerator.cpp -o query_gen
