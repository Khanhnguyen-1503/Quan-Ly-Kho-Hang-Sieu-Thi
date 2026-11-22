// FileIO.h - Huynh Tin
// Dinh dang (phan cach |, ngay dd/mm/yyyy):
//   mathang.txt = maVach|ten|nhom|donGia|nguongCanhBao
//   lo.txt      = maVach|maLo|ngayNhap|hsd|soLuong|giaNhap
//   phieu.txt   = maPhieu|loai|ngay|maVach|soLuong|donGia
// tonKho KHONG ghi ra tep: khi doc lo thi cong lai.
//
// Ma lo / ma phieu: trong tep co tien to cho de doc, trong chuong trinh la so nguyen (Types.h).
//   maLo:    "LO" + so                 LO00001 <-> 1
//   maPhieu: "PN" + so (phieu nhap)    PN00001 <-> 1
//            "PX" + so (phieu xuat)    PX00006 <-> 6
//   Phieu nhap va phieu xuat dung CHUNG mot day so (DanhSachPhieu::maPhieuKeTiep),
//   nen PN00006 va PX00006 khong duoc cung ton tai.
//   loai:    "NHAP" / "XUAT"
// Khi doc: bo tien to. Khi ghi: them lai tien to, phan so co it nhat 5 chu so.
#pragma once
#include <string>
#include "Types.h"
#include "Inventory.h"

bool        tachMaLo(const std::string& s, int& maLo);                        // false neu sai dinh dang
std::string taoMaLo(int maLo);
bool        tachMaPhieu(const std::string& s, LoaiPhieu loai, int& maPhieu);  // false neu tien to khong khop loai
std::string taoMaPhieu(int maPhieu, LoaiPhieu loai);
bool        tachLoaiPhieu(const std::string& s, LoaiPhieu& loai);
const char* tenLoaiPhieu(LoaiPhieu loai);                                     // "NHAP" / "XUAT"

KetQua docMatHang(Kho& kho, const std::string& tep);
KetQua docLo(Kho& kho, const std::string& tep);       // doc sau mathang; cap nhat tonKho, maLoKeTiep
KetQua docPhieu(Kho& kho, const std::string& tep);
KetQua ghiMatHang(const Kho& kho, const std::string& tep);
KetQua ghiLo(const Kho& kho, const std::string& tep);
KetQua ghiPhieu(const Kho& kho, const std::string& tep);

KetQua docDuLieu(Kho& kho, const std::string& thuMuc);        // goi khi khoi dong
KetQua ghiDuLieu(const Kho& kho, const std::string& thuMuc);  // goi khi thoat
