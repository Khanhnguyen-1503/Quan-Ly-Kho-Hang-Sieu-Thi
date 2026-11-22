// Phieu.h - Danh sach phieu nhap/xuat (DSLK don)
// Phu trach: Minh Tan
#pragma once
#include <string>
#include "Types.h"

void   dspInit(DanhSachPhieu& ds);
// Tao phieu moi voi maPhieu tu tang, them vao cuoi danh sach. O(1)
Phieu* dspThem(DanhSachPhieu& ds, LoaiPhieu loai, const Date& ngay,
               const std::string& maVach, int soLuong, double donGia);
void   dspThemCoSan(DanhSachPhieu& ds, Phieu* p);   // dung khi doc tep (giu maPhieu cu)
Phieu* dspTimTheoMa(const DanhSachPhieu& ds, int maPhieu);
void   dspIn(const DanhSachPhieu& ds, const Date* tu, const Date* den);  // nullptr = khong loc
void   dspClear(DanhSachPhieu& ds);
