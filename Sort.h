// Sort.h - Long Vu
// Mang dong DsMatHang (thay vector) va Merge Sort tu cai.
#pragma once
#include "Types.h"

void dsInit(DsMatHang& ds);
void dsThem(DsMatHang& ds, MatHang* mh);   // tu gap doi cap khi day
void dsClear(DsMatHang& ds);               // chi delete[] mang, khong delete MatHang

typedef int (*SoSanhMH)(const MatHang* a, const MatHang* b);   // <0 neu a dung truoc b
void mergeSort(MatHang** a, int n, SoSanhMH cmp);

int ssTheoTen(const MatHang* a, const MatHang* b);
int ssTheoTonKho(const MatHang* a, const MatHang* b);

void sapXepTheoTen(DsMatHang& ds);
void sapXepTheoTonKho(DsMatHang& ds);
void sapXepTheoHSD(CanhBaoHSD* a, int n);      // HSD gan nhat truoc
void sapXepBanChay(BanChay* a, int n);         // soLuongBan giam dan
void sapXepGiaTriNhom(GiaTriNhom* a, int n);   // giaTriTon giam dan
