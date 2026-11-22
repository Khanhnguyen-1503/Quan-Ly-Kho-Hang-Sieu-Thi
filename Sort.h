// Sort.h - Long Vu
// Mang dong DsMatHang (thay vector) va Merge Sort tu cai (khong dung std::sort).
#pragma once
#include "Types.h"

// ---- Mang dong MatHang* ----------------------------------------------------
void dsInit(DsMatHang& ds);
void dsThem(DsMatHang& ds, MatHang* mh);   // gap doi cap khi day
void dsClear(DsMatHang& ds);               // chi delete[] mang, KHONG delete MatHang

// ---- Merge Sort ------------------------------------------------------------
// Sap a[0..n-1] theo ham truoc(x, y) = true neu x phai dung truoc y.
// On dinh (phan tu bang nhau giu thu tu cu). O(n log n) thoi gian, O(n) bo nho phu.
template <class T>
void mergeSort(T* a, int n, bool (*truoc)(const T&, const T&));

// Cac kieu sap xep dung trong bao cao
void sapXepTheoTen(DsMatHang& ds);            // ten tang dan (khong phan biet hoa thuong)
void sapXepTheoNhom(DsMatHang& ds);           // nhom, cung nhom thi theo ten
void sapXepTheoTonKho(DsMatHang& ds);         // tonKho tang dan
void sapXepTheoHSD(CanhBaoHSD* a, int n);     // HSD gan nhat truoc
void sapXepBanChay(BanChay* a, int n);        // soLuongBan giam dan
void sapXepGiaTriNhom(GiaTriNhom* a, int n);  // giaTriTon giam dan

// So sanh chuoi khong phan biet hoa thuong: <0, 0, >0
int soSanhKhongPhanBiet(const std::string& a, const std::string& b);

// ---------------------------------------------------------------------------
// Cai dat template (phai nam trong header)
// ---------------------------------------------------------------------------
namespace sort_chi_tiet {
template <class T>
void tron(T* a, T* tam, int trai, int giua, int phai, bool (*truoc)(const T&, const T&)) {
    int i = trai, j = giua, k = trai;
    while (i < giua && j < phai) {
        // Lay ben phai chi khi no that su dung truoc -> giu tinh on dinh
        if (truoc(a[j], a[i])) tam[k++] = a[j++];
        else tam[k++] = a[i++];
    }
    while (i < giua) tam[k++] = a[i++];
    while (j < phai) tam[k++] = a[j++];
    for (k = trai; k < phai; ++k) a[k] = tam[k];
}

template <class T>
void sapDeQuy(T* a, T* tam, int trai, int phai, bool (*truoc)(const T&, const T&)) {
    if (phai - trai < 2) return;
    int giua = trai + (phai - trai) / 2;
    sapDeQuy(a, tam, trai, giua, truoc);
    sapDeQuy(a, tam, giua, phai, truoc);
    tron(a, tam, trai, giua, phai, truoc);
}
}  // namespace sort_chi_tiet

template <class T>
void mergeSort(T* a, int n, bool (*truoc)(const T&, const T&)) {
    if (!a || n < 2) return;
    T* tam = new T[n];
    sort_chi_tiet::sapDeQuy(a, tam, 0, n, truoc);
    delete[] tam;
}
