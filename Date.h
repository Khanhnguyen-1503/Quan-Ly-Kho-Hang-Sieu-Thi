// Date.h - Huynh Tin
// Kieu ngay dd/mm/yyyy: kiem tra, doi sang so ngay, cong/tru, so sanh.
#pragma once
#include <string>
#include "Types.h"

bool laNamNhuan(int y);
int  soNgayTrongThang(int m, int y);
bool ngayHopLe(const Date& d);

// Doi ngay <-> so ngay tinh tu 01/01/0001 (de cong/tru/so sanh cho de)
long long toSoNgay(const Date& d);
Date      fromSoNgay(long long n);

Date congNgay(const Date& d, int k);
Date truNgay(const Date& d, int k);
int  khoangCachNgay(const Date& a, const Date& b);   // = b - a (so ngay)
int  soSanhNgay(const Date& a, const Date& b);       // <0 neu a<b, 0 neu bang, >0 neu a>b

bool        parseDate(const std::string& s, Date& out);   // "dd/mm/yyyy"; false neu sai dinh dang/sai mien
std::string formatDate(const Date& d);                    // -> "dd/mm/yyyy"
Date        homNay();
