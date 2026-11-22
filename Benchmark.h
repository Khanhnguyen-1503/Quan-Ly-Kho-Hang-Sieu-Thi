// Benchmark.h - Khanh Nguyen, Huynh Tin
// Do B-Tree voi n = 10^3 .. 10^5, m = 3, 5, 10; ghi ket qua ra CSV.
#pragma once
#include <string>

struct KetQuaDo {
    int n;
    int m;
    double tgChenMs;          // tong thoi gian chen n khoa
    double tgTimMs;           // tong thoi gian tim n khoa
    double tbNutTruyXuat;     // so nut truy xuat trung binh moi lan tim
    int chieuCao;
    int soNut;
};

KetQuaDo doBTree(int n, int m, unsigned seed);
void     chayThucNghiem(const std::string& tepCsv);
