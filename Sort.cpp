// Sort.cpp - Long Vu
#include "Sort.h"
#include "Date.h"

// ---- Mang dong ---------------------------------------------------------------

void dsInit(DsMatHang& ds) {
    ds.a = nullptr;
    ds.n = 0;
    ds.cap = 0;
}

void dsThem(DsMatHang& ds, MatHang* mh) {
    if (ds.n == ds.cap) {
        int capMoi = ds.cap == 0 ? 16 : ds.cap * 2;
        MatHang** moi = new MatHang*[capMoi];
        for (int i = 0; i < ds.n; ++i) moi[i] = ds.a[i];
        delete[] ds.a;
        ds.a = moi;
        ds.cap = capMoi;
    }
    ds.a[ds.n++] = mh;
}

void dsClear(DsMatHang& ds) {
    delete[] ds.a;
    dsInit(ds);
}

// ---- So sanh ---------------------------------------------------------------

static char thuong(char c) { return (c >= 'A' && c <= 'Z') ? (char)(c - 'A' + 'a') : c; }

int soSanhKhongPhanBiet(const std::string& a, const std::string& b) {
    size_t n = a.size() < b.size() ? a.size() : b.size();
    for (size_t i = 0; i < n; ++i) {
        char x = thuong(a[i]), y = thuong(b[i]);
        if (x != y) return x < y ? -1 : 1;
    }
    if (a.size() == b.size()) return 0;
    return a.size() < b.size() ? -1 : 1;
}

// Trung khoa chinh thi xet ma vach, de ket qua luon xac dinh
static bool truocTheoTen(MatHang* const& x, MatHang* const& y) {
    int c = soSanhKhongPhanBiet(x->ten, y->ten);
    return c != 0 ? c < 0 : x->maVach < y->maVach;
}
static bool truocTheoNhom(MatHang* const& x, MatHang* const& y) {
    int c = soSanhKhongPhanBiet(x->nhom, y->nhom);
    return c != 0 ? c < 0 : truocTheoTen(x, y);
}
static bool truocTheoTonKho(MatHang* const& x, MatHang* const& y) {
    return x->tonKho != y->tonKho ? x->tonKho < y->tonKho : x->maVach < y->maVach;
}
static bool truocTheoHSD(const CanhBaoHSD& x, const CanhBaoHSD& y) {
    int c = soSanhDate(x.lo->hsd, y.lo->hsd);
    return c != 0 ? c < 0 : x.mh->maVach < y.mh->maVach;
}
static bool truocBanChay(const BanChay& x, const BanChay& y) {
    return x.soLuongBan != y.soLuongBan ? x.soLuongBan > y.soLuongBan : x.mh->maVach < y.mh->maVach;
}
static bool truocGiaTriNhom(const GiaTriNhom& x, const GiaTriNhom& y) {
    return x.giaTriTon != y.giaTriTon ? x.giaTriTon > y.giaTriTon : x.nhom < y.nhom;
}

// ---- Cac kieu sap xep ------------------------------------------------------

void sapXepTheoTen(DsMatHang& ds) { mergeSort<MatHang*>(ds.a, ds.n, truocTheoTen); }
void sapXepTheoNhom(DsMatHang& ds) { mergeSort<MatHang*>(ds.a, ds.n, truocTheoNhom); }
void sapXepTheoTonKho(DsMatHang& ds) { mergeSort<MatHang*>(ds.a, ds.n, truocTheoTonKho); }
void sapXepTheoHSD(CanhBaoHSD* a, int n) { mergeSort<CanhBaoHSD>(a, n, truocTheoHSD); }
void sapXepBanChay(BanChay* a, int n) { mergeSort<BanChay>(a, n, truocBanChay); }
void sapXepGiaTriNhom(GiaTriNhom* a, int n) { mergeSort<GiaTriNhom>(a, n, truocGiaTriNhom); }
