// Phieu.cpp - Danh sach phieu nhap/xuat bang DSLK don
// Nguoi thuc hien: Minh Tan
#include "Phieu.h"
#include <iostream>
#include <iomanip>

void dspInit(DanhSachPhieu& ds) {
    ds.head = nullptr;
    ds.tail = nullptr;
    ds.count = 0;
    ds.maPhieuKeTiep = 1;
}

// Tao phieu moi tu maPhieu tu tang va them vao cuoi - O(1)
Phieu* dspThem(DanhSachPhieu& ds, LoaiPhieu loai, const Date& ngay,
               const std::string& maVach, int soLuong, double donGia) {
    Phieu* p = new Phieu;
    p->maPhieu = ds.maPhieuKeTiep++;
    p->loai = loai;
    p->ngay = ngay;
    p->maVach = maVach;
    p->soLuong = soLuong;
    p->donGia = donGia;
    p->next = nullptr;

    if (ds.head == nullptr) {
        ds.head = ds.tail = p;
    } else {
        ds.tail->next = p;
        ds.tail = p;
    }
    ds.count++;
    return p;
}

// Them phieu da co san (khi doc du lieu tu file)
void dspThemCoSan(DanhSachPhieu& ds, Phieu* p) {
    if (!p) return;
    p->next = nullptr;
    if (ds.head == nullptr) {
        ds.head = ds.tail = p;
    } else {
        ds.tail->next = p;
        ds.tail = p;
    }
    ds.count++;
    if (p->maPhieu >= ds.maPhieuKeTiep) {
        ds.maPhieuKeTiep = p->maPhieu + 1;
    }
}

// Tim phieu theo ma
Phieu* dspTimTheoMa(const DanhSachPhieu& ds, int maPhieu) {
    for (Phieu* cur = ds.head; cur != nullptr; cur = cur->next) {
        if (cur->maPhieu == maPhieu) return cur;
    }
    return nullptr;
}

// Helper so sanh ngay: < 0 (a < b), == 0, > 0
static int ssNgay(const Date& a, const Date& b) {
    if (a.y != b.y) return a.y - b.y;
    if (a.m != b.m) return a.m - b.m;
    return a.d - b.d;
}

// In danh sach phieu (co the loc theo khoang thoi gian)
void dspIn(const DanhSachPhieu& ds, const Date* tu, const Date* den) {
    std::cout << "\n============================== DANH SACH PHIEU ==============================\n";
    std::cout << std::left << std::setw(10) << "Ma Phieu"
              << std::setw(12) << "Loai"
              << std::setw(14) << "Ngay"
              << std::setw(16) << "Ma Vach"
              << std::setw(10) << "So Luong"
              << std::setw(14) << "Don Gia" << "\n";
    std::cout << "-----------------------------------------------------------------------------\n";

    for (Phieu* cur = ds.head; cur != nullptr; cur = cur->next) {
        if (tu != nullptr && ssNgay(cur->ngay, *tu) < 0) continue;
        if (den != nullptr && ssNgay(cur->ngay, *den) > 0) continue;

        std::string sNgay = (cur->ngay.d < 10 ? "0" : "") + std::to_string(cur->ngay.d) + "/"
                          + (cur->ngay.m < 10 ? "0" : "") + std::to_string(cur->ngay.m) + "/"
                          + std::to_string(cur->ngay.y);

        std::cout << std::left << std::setw(10) << cur->maPhieu
                  << std::setw(12) << (cur->loai == PHIEU_NHAP ? "Nhap kho" : "Xuat kho")
                  << std::setw(14) << sNgay
                  << std::setw(16) << cur->maVach
                  << std::setw(10) << cur->soLuong
                  << std::fixed << std::setprecision(2) << std::setw(14) << cur->donGia << "\n";
    }
    std::cout << "=============================================================================\n";
}

// Giai phong bo nho danh sach phieu
void dspClear(DanhSachPhieu& ds) {
    Phieu* cur = ds.head;
    while (cur != nullptr) {
        Phieu* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    ds.head = ds.tail = nullptr;
    ds.count = 0;
}