// Report.h - Long Vu
// Tim kiem, liet ke, canh bao va bao cao, doc truc tiep tu Kho (B-Tree + lo + phieu).
// Ham tra mang qua tham so out (kq): nguoi goi delete[] kq. Khong co phan tu thi kq = nullptr.
// DsMatHang kq: nguoi goi dsInit truoc va dsClear sau (chi giai phong mang, khong xoa MatHang).
#pragma once
#include <string>
#include "Types.h"
#include "Inventory.h"

// Ten chua tuKhoa (chuoi con, khong phan biet hoa thuong). Ket qua sap theo ten.
void timTheoTen(const Kho& kho, const std::string& tuKhoa, DsMatHang& kq);

// Mat hang thuoc nhom (khong phan biet hoa thuong), sap theo ten.
// nhom rong = liet ke tat ca, sap theo nhom roi theo ten.
void lietKeTheoNhom(const Kho& kho, const std::string& nhom, DsMatHang& kq);

// tonKho < nguongCanhBao, sap theo tonKho tang dan (thieu nhieu nhat len dau).
void canhBaoSapHetHang(const Kho& kho, DsMatHang& kq);

// Cac lo con hang co HSD trong [homNay, homNay + soNgay], sap theo HSD.
// kemDaHetHan = true: lay ca lo da qua HSD ma van con hang (soNgayConLai < 0).
// Tra ve so phan tu.
int canhBaoSapHetHan(const Kho& kho, const Date& homNay, int soNgay, CanhBaoHSD*& kq,
                     bool kemDaHetHan = true);

// Moi nhom: so mat hang, tong ton, gia tri ton = tong (soLuong * giaNhap) cac lo con lai.
// Sap theo gia tri giam dan. Tra ve so nhom.
int baoCaoGiaTriTheoNhom(const Kho& kho, GiaTriNhom*& kq);

// Top k ban chay trong thang/nam theo phieu xuat:
// demTam = 0 -> duyet phieu xuat trong thang -> btSearch, cong demTam
// -> gom mat hang co demTam > 0 -> Merge Sort giam dan -> lay k.
// Phieu cua mat hang da bi xoa thi bo qua. Tra ve so phan tu (<= k).
int topBanChay(Kho& kho, int thang, int nam, int k, BanChay*& kq);

// In bang
void inBangMatHang(const DsMatHang& ds);
void inBangCanhBaoHSD(const CanhBaoHSD* a, int n);
void inBangGiaTriNhom(const GiaTriNhom* a, int n);
void inBangBanChay(const BanChay* a, int n);
