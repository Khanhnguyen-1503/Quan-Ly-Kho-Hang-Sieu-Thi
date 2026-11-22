#include "Date.h"
#include <ctime>
#include <cstdio>
#include <sstream>

bool laNamNhuan(int nam) {
    return (nam % 4 == 0 && nam % 100 != 0) || (nam % 400 == 0);
}

int soNgayTrongThang(int thang, int nam) {
    if (thang < 1 || thang > 12) return 0;
    static const int ngayThang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (thang == 2 && laNamNhuan(nam)) return 29;
    return ngayThang[thang];
}

bool laNgayHopLe(Date d) {
    if (d.y < 1900 || d.y > 2100) return false;
    if (d.m < 1 || d.m > 12) return false;
    int maxNgay = soNgayTrongThang(d.m, d.y);
    return d.d >= 1 && d.d <= maxNgay;
}

// Thuật toán quy đổi Date sang số ngày tuyệt đối (mốc 0000-03-01)
long long dateToDays(Date d) {
    long long y = d.y;
    long long m = d.m;
    if (m <= 2) {
        y--;
        m += 12;
    }
    return 365 * y + y / 4 - y / 100 + y / 400 + (153 * m - 457) / 5 + d.d - 306;
}

Date daysToDate(long long totalDays) {
    long long z = totalDays + 306;
    long long h = 100 * z - 25;
    long long a = h / 3652425;
    long long b = a - a / 4;
    long long y = (100 * b + h) / 36525;
    long long c = b + z - 365 * y - y / 4;
    long long m = (5 * c + 456) / 153;
    int ngay = static_cast<int>(c - (153 * m - 457) / 5);
    if (m > 12) {
        y++;
        m -= 12;
    }
    Date res = {ngay, static_cast<int>(m), static_cast<int>(y)};
    return res;
}

Date congNgay(Date d, int soNgay) {
    return daysToDate(dateToDays(d) + soNgay);
}

long long khoangCachNgay(Date d1, Date d2) {
    return dateToDays(d1) - dateToDays(d2);
}

int soSanhDate(Date d1, Date d2) {
    long long diff = khoangCachNgay(d1, d2);
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

bool parseDate(const std::string& str, Date& outDate) {
    int d, m, y;
    char sep1, sep2;
    std::istringstream iss(str);
    if (!(iss >> d >> sep1 >> m >> sep2 >> y)) return false;
    if (sep1 != '/' || sep2 != '/') return false;
    // Kiểm tra không còn ký tự thừa phía sau
    std::string extra;
    if (iss >> extra) return false;

    Date temp = {d, m, y};
    if (!laNgayHopLe(temp)) return false;
    outDate = temp;
    return true;
}

std::string formatDate(Date d) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%02d/%02d/%04d", d.d, d.m, d.y);
    return std::string(buf);
}

Date homNay() {
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);
    Date d = {
        ltm->tm_mday,
        ltm->tm_mon + 1,
        ltm->tm_year + 1900
    };
    return d;
}