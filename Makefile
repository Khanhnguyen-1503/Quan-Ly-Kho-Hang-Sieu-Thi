# Makefile - build va chay chuong trinh / test cho ca nhom
#   make               build chuong trinh chinh quanlykho
#   make run           chay menu voi du lieu trong data/
#   make test          build + chay tat ca test
#   make bench         chay thuc nghiem B-Tree, ghi ketqua/ketqua_btree.csv
#   make gen_data      chuong trinh rieng sinh du lieu mau (tools/gen_data.cpp)
#   make baocao        bien dich bao cao LaTeX -> baocao/baocao.pdf (can XeLaTeX, TeX Live)
#   make clean
# Kiem tra ro ri / loi bo nho (Linux, macOS):
#   make clean test CXXFLAGS="-std=c++17 -g -O1 -Wall -Wextra -fsanitize=address,undefined"
# Tren Windows (MinGW) dung: mingw32-make, chay quanlykho.exe

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
CPPFLAGS += -I.

KHO   = BTree.cpp Date.cpp LotQueue.cpp Phieu.cpp Inventory.cpp
TEPIO = $(KHO) Validate.cpp FileIO.cpp
TATCA = $(TEPIO) Sort.cpp Report.cpp DataGen.cpp Benchmark.cpp

TESTS = test_btree test_date test_kho test_report test_fileio test_dulieu test_menu

quanlykho: $(TATCA) main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

all: quanlykho $(TESTS) bench_btree gen_data

test_btree: BTree.cpp tests/test_btree.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test_date: Date.cpp tests/test_date.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test_kho: $(KHO) tests/test_kho.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test_report: $(KHO) Sort.cpp Report.cpp tests/test_report.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test_fileio: $(TEPIO) DataGen.cpp tests/test_fileio.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test_dulieu: $(TEPIO) DataGen.cpp tests/test_dulieu.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

bench_btree: BTree.cpp Benchmark.cpp tests/bench_btree.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

gen_data: Date.cpp DataGen.cpp tools/gen_data.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

# Chay menu bang kich ban nhap san tren mot ban sao cua data/
test_menu: quanlykho tests/test_menu.sh
	@true

test: $(TESTS)
	./test_btree
	./test_date
	./test_kho
	./test_report
	./test_fileio
	./test_dulieu data
	sh tests/test_menu.sh ./quanlykho

run: quanlykho
	./quanlykho data

bench: bench_btree
	./bench_btree ketqua/ketqua_btree.csv

baocao:
	cd baocao && latexmk -xelatex -interaction=nonstopmode baocao.tex

clean:
	rm -f quanlykho $(TESTS) bench_btree gen_data *.o *.exe

.PHONY: all test run bench baocao clean test_menu
