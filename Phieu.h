// Phieu.h - Minh Tan
// Danh sach phieu nhap/xuat (DSLK don, them vao cuoi).
#pragma once
#include <string>
#include "Types.h"

void   dspInit(DanhSachPhieu& ds);   // head = tail = nullptr, count = 0, maPhieuKeTiep = 1
// Tao phieu moi voi maPhieu = maPhieuKeTiep++, noi vao cuoi danh sach
Phieu* dspThem(DanhSachPhieu& ds, LoaiPhieu loai, const Date& ngay,
               const std::string& maVach, int soLuong, double donGia);
// Noi mot phieu da co san maPhieu (dung khi doc tep); cap nhat maPhieuKeTiep neu can
void   dspNoiCuoi(DanhSachPhieu& ds, Phieu* p);
Phieu* dspTimTheoMa(const DanhSachPhieu& ds, int maPhieu);
void   dspIn(const DanhSachPhieu& ds);
void   dspClear(DanhSachPhieu& ds);   // delete tat ca phieu
