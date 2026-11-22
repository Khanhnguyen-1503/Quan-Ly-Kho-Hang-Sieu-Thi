// Report.h - Long Vu
// Tim kiem, liet ke, canh bao va bao cao. Mang tra ve qua tham so out: nguoi goi delete[].
#pragma once
#include <string>
#include "Types.h"
#include "Inventory.h"

void timTheoTen(const Kho& kho, const std::string& tuKhoa, DsMatHang& kq);   // chuoi con, khong phan biet hoa thuong
void lietKeTheoNhom(const Kho& kho, const std::string& nhom, DsMatHang& kq);
void canhBaoSapHetHang(const Kho& kho, DsMatHang& kq);                       // tonKho < nguongCanhBao

// Cac lo con hang co HSD trong [homNay, homNay + soNgay]; sap theo HSD. Tra ve so phan tu.
int canhBaoSapHetHan(const Kho& kho, const Date& homNay, int soNgay, CanhBaoHSD*& kq);
int baoCaoGiaTriTheoNhom(const Kho& kho, GiaTriNhom*& kq);
// demTam = 0 -> duyet phieu xuat trong thang -> btSearch, cong demTam -> gom demTam > 0 -> sap giam -> lay k
int topBanChay(Kho& kho, int thang, int nam, int k, BanChay*& kq);

void inBangMatHang(const DsMatHang& ds);
void inBangCanhBaoHSD(const CanhBaoHSD* a, int n);
void inBangGiaTriNhom(const GiaTriNhom* a, int n);
void inBangBanChay(const BanChay* a, int n);
