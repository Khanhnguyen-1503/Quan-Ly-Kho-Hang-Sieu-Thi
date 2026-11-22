# Makefile - build va chay test cho ca nhom
#   make test          build + chay tat ca test
#   make bench         chay thuc nghiem B-Tree, ghi ketqua/ketqua_btree.csv
#   make gen_data      chuong trinh sinh du lieu mau (main.cpp hien tai)
#   make clean
# Kiem tra ro ri / loi bo nho (Linux, macOS):
#   make clean test CXXFLAGS="-std=c++17 -g -O1 -Wall -Wextra -fsanitize=address,undefined"
# Tren Windows (MinGW) dung: mingw32-make test

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
CPPFLAGS += -I.

KHO = BTree.cpp Date.cpp LotQueue.cpp Phieu.cpp Inventory.cpp

TESTS = test_btree test_date test_kho test_dulieu

all: $(TESTS) bench_btree gen_data

test_btree: BTree.cpp tests/test_btree.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test_date: Date.cpp tests/test_date.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test_kho: $(KHO) tests/test_kho.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test_dulieu: BTree.cpp Date.cpp DataGen.cpp tests/test_dulieu.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

bench_btree: BTree.cpp Benchmark.cpp tests/bench_btree.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

gen_data: Date.cpp DataGen.cpp main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test: $(TESTS)
	./test_btree
	./test_date
	./test_kho
	./test_dulieu data

bench: bench_btree
	./bench_btree ketqua/ketqua_btree.csv

clean:
	rm -f $(TESTS) bench_btree gen_data *.o *.exe

.PHONY: all test bench clean
