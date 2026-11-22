// DataGen.h - Huynh Tin
// Sinh du lieu mau (>= 30 ban ghi) va bo lon 10^4 - 10^5 de do thoi gian.
#pragma once
#include <string>

// Ghi mathang.txt, lo.txt, phieu.txt (moi mat hang 1-3 lo, ~60% co phieu xuat)
void sinhDuLieuMau(int soMatHang, const std::string& fileMatHang,
                   const std::string& fileLo, const std::string& filePhieu);
// Chi ghi mathang.txt, ma vach xao tron (de do B-Tree)
void sinhDuLieuLon(int soMatHang, const std::string& fileMatHang);
