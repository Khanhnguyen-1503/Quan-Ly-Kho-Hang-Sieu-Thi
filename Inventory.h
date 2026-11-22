// Inventory.h - Minh Tan
// Kho = B-Tree mat hang + danh sach phieu. Xuat kho theo FIFO lo.
#pragma once
#include <string>
#include "Types.h"
#include "BTree.h"

struct Kho {
    BTree cay;             // B-Tree mat hang (giu MatHang*)
    DanhSachPhieu dsPhieu; // tat ca phieu nhap/xuat
    int maLoKeTiep;        // cap maLo cho lo moi
};

void khoInit(Kho& kho, int m);   // m = bac B-Tree
void khoDestroy(Kho& kho);       // delete moi MatHang (kem cac lo), phieu, nut cay

// new MatHang, gan tonKho = 0, demTam = 0, dsLo rong
MatHang* taoMatHang(const std::string& maVach, const std::string& ten, const std::string& nhom,
                    double donGia, int nguongCanhBao);

KetQua   themMatHang(Kho& kho, const std::string& maVach, const std::string& ten,
                     const std::string& nhom, double donGia, int nguongCanhBao);   // KQ_TRUNG_MA
KetQua   suaMatHang(Kho& kho, const std::string& maVach, const std::string& ten,
                    const std::string& nhom, double donGia, int nguongCanhBao);    // khong sua maVach
KetQua   xoaMatHang(Kho& kho, const std::string& maVach);   // KQ_CON_TON_KHO neu tonKho > 0
MatHang* timMatHang(Kho& kho, const std::string& maVach);

// Phieu nhap: tao lo moi o cuoi hang doi, cong tonKho, ghi phieu
KetQua nhapKho(Kho& kho, const std::string& maVach, int soLuong, double giaNhap,
               const Date& ngayNhap, const Date& hsd);
// Phieu xuat: lay tu lo cu nhat, thieu thi lay tiep lo sau; KQ_KHONG_DU_HANG neu tonKho < soLuong
KetQua xuatKho(Kho& kho, const std::string& maVach, int soLuong, const Date& ngay);
