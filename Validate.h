// Validate.h - Huynh Tin
// Nhap du lieu tu ban phim co kiem tra sai kieu, sai mien; nhap sai thi hoi lai.
#pragma once
#include <string>
#include "Types.h"

bool laSoNguyen(const std::string& s, int& out);
bool laSoThuc(const std::string& s, double& out);
bool maVachHopLe(const std::string& s);   // dung 8 hoac 13 chu so

int         nhapSoNguyen(const std::string& nhac, int minVal, int maxVal);
double      nhapSoThuc(const std::string& nhac, double minVal, double maxVal);
std::string nhapChuoi(const std::string& nhac, bool choPhepRong = false);   // khong chua '|'
Date        nhapNgay(const std::string& nhac);                               // dd/mm/yyyy hop le
std::string nhapMaVach(const std::string& nhac);
