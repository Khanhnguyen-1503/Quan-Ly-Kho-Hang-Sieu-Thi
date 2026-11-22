// FileIO.h - Huynh Tin
// Dinh dang (phan cach |, ngay dd/mm/yyyy):
//   mathang.txt = maVach|ten|nhom|donGia|nguongCanhBao
//   lo.txt      = maVach|maLo|ngayNhap|hsd|soLuong|giaNhap
//   phieu.txt   = maPhieu|loai|ngay|maVach|soLuong|donGia
// tonKho KHONG ghi ra tep: khi doc lo thi cong lai.
#pragma once
#include <string>
#include "Types.h"
#include "Inventory.h"

KetQua docMatHang(Kho& kho, const std::string& tep);
KetQua docLo(Kho& kho, const std::string& tep);       // doc sau mathang; cap nhat tonKho, maLoKeTiep
KetQua docPhieu(Kho& kho, const std::string& tep);
KetQua ghiMatHang(const Kho& kho, const std::string& tep);
KetQua ghiLo(const Kho& kho, const std::string& tep);
KetQua ghiPhieu(const Kho& kho, const std::string& tep);

KetQua docDuLieu(Kho& kho, const std::string& thuMuc);        // goi khi khoi dong
KetQua ghiDuLieu(const Kho& kho, const std::string& thuMuc);  // goi khi thoat
