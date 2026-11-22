// Inventory.h - Nghiep vu kho: them/xoa mat hang, nhap kho, xuat kho FIFO
// Phu trach: Minh Tan (goi ham cua BTree, LotQueue, Phieu)
#pragma once
#include <string>
#include "Types.h"

struct Kho {
    BTree         cay;
    DanhSachPhieu phieu;
    int           maLoKeTiep;
};

void khoInit(Kho& kho, int bacCay);
void khoGiaiPhong(Kho& kho);   // giai phong cay, mat hang, lo, phieu

// Mat hang
KetQua   themMatHang(Kho& kho, const std::string& maVach, const std::string& ten,
                     const std::string& nhom, double donGia, int nguongCanhBao);
KetQua   suaMatHang(Kho& kho, const std::string& maVach, const std::string& tenMoi,
                    const std::string& nhomMoi, double donGiaMoi, int nguongMoi);
KetQua   xoaMatHang(Kho& kho, const std::string& maVach, bool xoaKeCaTonKho);
MatHang* timMatHang(Kho& kho, const std::string& maVach);

// Phieu nhap: tao lo moi o cuoi hang doi, tang ton kho, ghi phieu
KetQua nhapKho(Kho& kho, const std::string& maVach, int soLuong, double giaNhap,
               const Date& hsd, const Date& ngay, int* maPhieuOut);

// Phieu xuat: lay tu lo cu nhat; lo het thi dequeue va lay tiep lo sau
// Tra KQ_KHONG_DU_HANG neu soLuong > tonKho (khong xuat gi ca)
KetQua xuatKho(Kho& kho, const std::string& maVach, int soLuong, const Date& ngay,
               int* maPhieuOut);

void inChiTietMatHang(const MatHang& mh);   // thong tin + danh sach lo
