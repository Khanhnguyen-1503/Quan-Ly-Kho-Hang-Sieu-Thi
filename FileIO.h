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

// ---- Doc / ghi tep ----------------------------------------------------------
// Doc: dong sai bi bo qua va bao ra std::cerr (ten tep, so dong, ly do).
//   KQ_OK: doc het | KQ_DU_LIEU_SAI: co dong bi bo qua | KQ_LOI_TEP: khong mo duoc tep.
//   Chap nhan xuong dong LF va CRLF; bo qua dong trong.
// Ghi: ghi ra <tep>.tmp roi moi thay tep cu, loi giua chung thi tep cu van con.
KetQua docMatHang(Kho& kho, const std::string& tep);
// Doc sau mathang. Lo cua moi mat hang phai theo thu tu nhap (cu -> moi) = thu tu FIFO.
// tonKho = tong soLuong cac lo; maLoKeTiep = maLo lon nhat + 1.
KetQua docLo(Kho& kho, const std::string& tep);
// maPhieu phai tang dan. Khong bat buoc maVach con trong kho (phieu cua mat hang da xoa
// van la lich su). maPhieuKeTiep = maPhieu lon nhat + 1.
KetQua docPhieu(Kho& kho, const std::string& tep);
KetQua ghiMatHang(const Kho& kho, const std::string& tep);   // theo thu tu ma vach
KetQua ghiLo(const Kho& kho, const std::string& tep);        // moi mat hang: front -> rear
KetQua ghiPhieu(const Kho& kho, const std::string& tep);

// thuMuc chua mathang.txt, lo.txt, phieu.txt. Kho phai rong (vua khoInit).
// Thieu mathang.txt -> KQ_LOI_TEP; thieu lo.txt / phieu.txt -> coi nhu rong, KQ_DU_LIEU_SAI.
KetQua docDuLieu(Kho& kho, const std::string& thuMuc);        // goi khi khoi dong
KetQua ghiDuLieu(const Kho& kho, const std::string& thuMuc);  // goi khi thoat
