FLAGS=-O2 -std=c++11 -Wall

all: build_benchmark query_gen build_gen query_benchmark

build_benchmark: BuildBenchmark.cpp bitmap.hpp wavelet-matrix.hpp wavelet-tree.hpp utils.hpp
	$(CXX) $(FLAGS) -lboost_timer BuildBenchmark.cpp -o build_benchmark

query_benchmark: QueryBenchmark.cpp bitmap.hpp wavelet-matrix.hpp wavelet-tree.hpp wavelet-tree-pointers.hpp utils.hpp 
	$(CXX) $(FLAGS) -lboost_timer QueryBenchmark.cpp -o query_benchmark

build_gen: BuildGenerator.cpp utils.hpp
	$(CXX) $(FLAGS) BuildGenerator.cpp -o build_gen

query_gen: QueryGenerator.cpp utils.hpp
	$(CXX) $(FLAGS) QueryGenerator.cpp -o query_gen
