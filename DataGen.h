// DataGen.h - Huynh Tin
// Sinh du lieu mau (>= 30 ban ghi) va bo lon 10^4 - 10^5 de do thoi gian.
#pragma once
#include <string>
#include "Types.h"
#include "Inventory.h"

std::string sinhMaVach(int i, bool loai13);                        // ma duy nhat theo chi so i
void   sinhDuLieu(Kho& kho, int soMatHang, unsigned seed);          // sinh thang vao kho (kem lo, phieu)
KetQua sinhTepMau(const std::string& thuMuc, int soMatHang, unsigned seed);   // ghi ra 3 tep
