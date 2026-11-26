// Validate.h - Huynh Tin
// Kiem tra du lieu (sai kieu, sai mien) va nhap tu ban phim: nhap sai thi bao ly do va hoi lai.
#pragma once
#include <string>
#include "Types.h"

// Nem ra khi het dau vao (Ctrl+D / Ctrl+Z, hoac chay "< input.txt" da het dong).
// main bat ngoai le nay de luu du lieu va thoat, tranh vong lap vo han.
struct HetDauVao {};

// ---- Kiem tra chuoi (khong doc ban phim) ------------------------------------
std::string catKhoangTrang(const std::string& s);   // bo khoang trang / tab / \r hai dau
bool laSoNguyen(const std::string& s, int& out);     // [+-]chu so, khong tran int
bool laSoThuc(const std::string& s, double& out);    // so huu han; khong nhan nan, inf, hex, dau phay
bool maVachHopLe(const std::string& s);              // dung 8 hoac 13 chu so

// ---- Nhap tu ban phim ---------------------------------------------------------
// macDinh != nullptr: bam Enter (dong rong) thi tra ve *macDinh (vd. giu gia tri cu khi sua).
std::string docDong(const std::string& nhac);   // in nhac, doc mot dong, da cat khoang trang
int         nhapSoNguyen(const std::string& nhac, int minVal, int maxVal, const int* macDinh = nullptr);
double      nhapSoThuc(const std::string& nhac, double minVal, double maxVal, const double* macDinh = nullptr);
// Khong rong, khong chua '|' (ky tu phan cach trong tep), toi da 60 ky tu
std::string nhapChuoi(const std::string& nhac, const std::string* macDinh = nullptr);
Date        nhapNgay(const std::string& nhac, const Date* macDinh = nullptr);   // dd/mm/yyyy, 1900-2100
std::string nhapMaVach(const std::string& nhac);
bool        nhapCoKhong(const std::string& nhac);   // c/k (co the go y/n)
