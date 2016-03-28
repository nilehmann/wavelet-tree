FLAGS=-O2 -std=c++11 -Wall

#all: build_benchmark query_gen build_gen query_benchmark
all: query_benchmark query_gen build_benchmark

build_benchmark: BuildBenchmark.cpp wavelet-matrix.o wavelet-tree.o utils.o
	$(CXX) $(FLAGS) -lboost_timer BuildBenchmark.cpp utils.o wavelet-matrix.o wavelet-tree.o  -o build_benchmark

wavelet-matrix.o: wavelet-matrix.cpp
	$(CXX) $(FLAGS) -c wavelet-matrix.cpp
wavelet-tree.o: wavelet-tree.cpp
	$(CXX) $(FLAGS) -c wavelet-tree.cpp
utils.o: utils.cpp
	$(CXX) $(FLAGS) -c utils.cpp

query_benchmark: QueryBenchmark.cpp bitmap.hpp wavelet-matrix.o wavelet-tree.o 
	$(CXX) $(FLAGS) -lboost_timer QueryBenchmark.cpp utils.o wavelet-matrix.o wavelet-tree.o -o query_benchmark

#build_gen: BuildGenerator.cpp utils.hpp
	#$(CXX) $(FLAGS) BuildGenerator.cpp -o build_gen

query_gen: QueryGenerator.cpp utils.o
	$(CXX) $(FLAGS) QueryGenerator.cpp utils.o -o query_gen
