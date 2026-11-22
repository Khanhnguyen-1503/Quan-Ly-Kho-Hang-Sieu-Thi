// Benchmark.cpp - Khanh Nguyen, Huynh Tin
// Thuc nghiem B-Tree: chen n ma vach theo thu tu ngau nhien, roi tim lai tat ca
// (cung theo thu tu ngau nhien). Do thoi gian va so nut truy xuat trung binh moi lan tim.
#include "Benchmark.h"
#include "BTree.h"
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace {

// Bo sinh so ngau nhien tu cai (xorshift32): ket qua giong nhau tren moi trinh bien dich,
// khong phu thuoc RAND_MAX (MinGW chi co 32767).
struct NgauNhien {
    unsigned s;
    explicit NgauNhien(unsigned seed) : s(seed ? seed : 2463534242u) {}
    unsigned tiep() {
        s ^= s << 13;
        s ^= s >> 17;
        s ^= s << 5;
        return s;
    }
};

void tronMang(int* a, int n, NgauNhien& r) {
    for (int i = n - 1; i > 0; --i) {
        int j = (int)(r.tiep() % (unsigned)(i + 1));
        int t = a[i]; a[i] = a[j]; a[j] = t;
    }
}

// Ma vach 13 chu so, duy nhat theo i
std::string maVachThu(int i) {
    char buf[16];
    std::snprintf(buf, sizeof buf, "893%010d", i * 7 + 1);
    return buf;
}

MatHang* taoMatHangDo(const std::string& ma) {
    MatHang* mh = new MatHang;
    mh->maVach = ma;
    mh->donGia = 0;
    mh->tonKho = 0;
    mh->nguongCanhBao = 0;
    mh->dsLo.front = mh->dsLo.rear = nullptr;
    mh->dsLo.count = 0;
    mh->demTam = 0;
    return mh;
}

double msTu(std::chrono::steady_clock::time_point bd) {
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - bd).count();
}

}  // namespace

KetQuaDo doBTree(int n, int m, unsigned seed) {
    KetQuaDo kq{};
    kq.n = n;
    kq.m = m;
    if (n <= 0) return kq;

    NgauNhien r(seed);
    MatHang** ds = new MatHang*[n];
    int* thuTu = new int[n];
    for (int i = 0; i < n; ++i) {
        ds[i] = taoMatHangDo(maVachThu(i));
        thuTu[i] = i;
    }

    BTree t;
    btInit(t, m);

    // Chen
    tronMang(thuTu, n, r);
    auto bd = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i) btInsert(t, ds[thuTu[i]]);
    kq.tgChenMs = msTu(bd);

    // Tim lai tat ca
    tronMang(thuTu, n, r);
    t.soNutTruyXuat = 0;
    int timThay = 0;
    bd = std::chrono::steady_clock::now();
    for (int i = 0; i < n; ++i)
        if (btSearch(t, ds[thuTu[i]]->maVach)) ++timThay;
    kq.tgTimMs = msTu(bd);
    if (timThay != n) std::cerr << "Canh bao: chi tim thay " << timThay << "/" << n << " khoa\n";

    kq.tbNutTruyXuat = (double)t.soNutTruyXuat / n;
    kq.chieuCao = btHeight(t);
    kq.soNut = btSoNut(t);

    btDestroy(t);
    for (int i = 0; i < n; ++i) delete ds[i];
    delete[] ds;
    delete[] thuTu;
    return kq;
}

void chayThucNghiem(const std::string& tepCsv) {
    const int dsN[] = {1000, 5000, 10000, 20000, 50000, 100000};
    const int dsM[] = {3, 5, 10};
    const int soLanLap = 3;   // lay trung binh 3 lan (3 seed khac nhau) cho thoi gian on dinh hon

    std::ofstream f(tepCsv.c_str());
    if (!f) std::cerr << "Khong mo duoc tep " << tepCsv << ", chi in ra man hinh\n";
    else f << "n,m,tgChenMs,tgTimMs,tbNutTruyXuat,chieuCao,soNut\n";

    std::cout << std::left << std::setw(8) << "n" << std::setw(5) << "m"
              << std::right << std::setw(12) << "Chen (ms)" << std::setw(12) << "Tim (ms)"
              << std::setw(14) << "TB nut/tim" << std::setw(11) << "Chieu cao"
              << std::setw(10) << "So nut" << '\n';
    std::cout << std::string(72, '-') << '\n';

    for (int n : dsN) {
        for (int m : dsM) {
            KetQuaDo tb{};
            tb.n = n;
            tb.m = m;
            for (int lan = 0; lan < soLanLap; ++lan) {
                KetQuaDo k = doBTree(n, m, 12345u + 1000u * lan);
                tb.tgChenMs += k.tgChenMs / soLanLap;
                tb.tgTimMs += k.tgTimMs / soLanLap;
                tb.tbNutTruyXuat += k.tbNutTruyXuat / soLanLap;
                if (k.chieuCao > tb.chieuCao) tb.chieuCao = k.chieuCao;
                tb.soNut += k.soNut / soLanLap;
            }
            std::cout << std::left << std::setw(8) << n << std::setw(5) << m << std::right
                      << std::fixed << std::setprecision(3)
                      << std::setw(12) << tb.tgChenMs << std::setw(12) << tb.tgTimMs
                      << std::setprecision(2) << std::setw(14) << tb.tbNutTruyXuat
                      << std::setw(11) << tb.chieuCao << std::setw(10) << tb.soNut << '\n';
            if (f)
                f << n << ',' << m << ',' << std::fixed << std::setprecision(3) << tb.tgChenMs << ','
                  << tb.tgTimMs << ',' << std::setprecision(2) << tb.tbNutTruyXuat << ','
                  << tb.chieuCao << ',' << tb.soNut << '\n';
        }
    }
    if (f) std::cout << "\nDa ghi ket qua vao " << tepCsv << '\n';
}
