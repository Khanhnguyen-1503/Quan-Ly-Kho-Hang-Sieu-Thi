// tests/bench_btree.cpp - chay thuc nghiem B-Tree doc lap (khong can menu)
// Bien dich: g++ -std=c++17 -O2 -I. BTree.cpp Benchmark.cpp tests/bench_btree.cpp -o bench_btree
// Chay:      ./bench_btree [tep.csv]     (mac dinh: ketqua_btree.csv)
#include "Benchmark.h"

int main(int argc, char** argv) {
    chayThucNghiem(argc > 1 ? argv[1] : "ketqua_btree.csv");
    return 0;
}
