// Date.h - Huynh Tin
// Kieu ngay dd/mm/yyyy (struct Date {d, m, y} trong Types.h): kiem tra, doi sang so ngay,
// cong/tru, so sanh.
#pragma once
#include <string>
#include "Types.h"

bool laNamNhuan(int nam);
int  soNgayTrongThang(int thang, int nam);
bool laNgayHopLe(Date d);                      // nam 1900..2100

long long dateToDays(Date d);                  // so ngay tuyet doi
Date      daysToDate(long long totalDays);

Date      congNgay(Date d, int soNgay);        // soNgay am = tru ngay
long long khoangCachNgay(Date d1, Date d2);    // = d1 - d2 (so ngay)
int       soSanhDate(Date d1, Date d2);        // -1, 0, 1

bool        parseDate(const std::string& str, Date& outDate);   // "dd/mm/yyyy"; false neu sai
std::string formatDate(Date d);                                 // -> "dd/mm/yyyy"
Date        homNay();
