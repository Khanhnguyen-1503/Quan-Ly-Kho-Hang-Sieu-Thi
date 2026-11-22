// tests/test_date.cpp - kiem thu Date.cpp (Huynh Tin)
// Bien dich: g++ -std=c++17 -Wall -Wextra -I. Date.cpp tests/test_date.cpp -o test_date
#include <cstdio>
#include <string>
#include "Date.h"

static int soLoi = 0;
#define KIEMTRA(dk, ...)                                    \
    do {                                                    \
        if (!(dk)) {                                        \
            if (++soLoi <= 20) {                            \
                std::printf("  LOI (dong %d): ", __LINE__); \
                std::printf(__VA_ARGS__);                   \
                std::printf("\n");                          \
            }                                               \
        }                                                   \
    } while (0)

static bool bang(Date a, Date b) { return a.d == b.d && a.m == b.m && a.y == b.y; }
static Date D(int d, int m, int y) { Date r = {d, m, y}; return r; }

// Duyet tung ngay 01/01/1900 .. 31/12/2100 bang cach tang tay, doi chieu voi Date.cpp
static void testMoiNgay() {
    std::printf("duyet moi ngay 1900-2100\n");
    Date d = D(1, 1, 1900);
    long long truoc = dateToDays(d) - 1;
    int soNgay = 0;
    while (d.y <= 2100) {
        KIEMTRA(laNgayHopLe(d), "ngay hop le bi tu choi %s", formatDate(d).c_str());
        long long n = dateToDays(d);
        KIEMTRA(n == truoc + 1, "dateToDays khong lien tiep tai %s", formatDate(d).c_str());
        KIEMTRA(bang(daysToDate(n), d), "daysToDate sai tai %s", formatDate(d).c_str());
        Date p;
        KIEMTRA(parseDate(formatDate(d), p) && bang(p, d), "parse/format sai tai %s",
                formatDate(d).c_str());
        KIEMTRA(bang(congNgay(d, 1), daysToDate(n + 1)), "congNgay +1 sai tai %s", formatDate(d).c_str());
        truoc = n;
        ++soNgay;
        // tang tay
        if (++d.d > soNgayTrongThang(d.m, d.y)) {
            d.d = 1;
            if (++d.m > 12) { d.m = 1; ++d.y; }
        }
    }
    KIEMTRA(soNgay == 73414, "so ngay 1900-2100 = %d (dung: 73414)", soNgay);
}

static void testNamNhuan() {
    std::printf("nam nhuan\n");
    KIEMTRA(laNamNhuan(2024) && laNamNhuan(2000) && !laNamNhuan(1900) && !laNamNhuan(2026),
            "laNamNhuan sai");
    KIEMTRA(soNgayTrongThang(2, 2024) == 29 && soNgayTrongThang(2, 2026) == 28, "thang 2 sai");
    KIEMTRA(soNgayTrongThang(13, 2026) == 0 && soNgayTrongThang(0, 2026) == 0, "thang ngoai mien");
}

static void testParse() {
    std::printf("parseDate\n");
    const char* hopLe[] = {"29/02/2024", "29/02/2000", "01/01/1900", "31/12/2100", "1/1/2026"};
    for (const char* s : hopLe) {
        Date d;
        KIEMTRA(parseDate(s, d), "\"%s\" phai hop le", s);
    }
    const char* sai[] = {"29/02/2025", "29/02/1900", "31/04/2026", "00/01/2026", "32/01/2026",
                         "01/13/2026", "01/00/2026", "31/12/1899", "01/01/2101", "",
                         "abc", "01-01-2026", "01/01/2026x", "01/01/2026 7", "01/01/",
                         "1.5/01/2026", "01/01/2026/"};
    for (const char* s : sai) {
        Date d = D(9, 9, 2009);
        bool ok = parseDate(s, d);
        KIEMTRA(!ok, "\"%s\" phai bi tu choi", s);
        KIEMTRA(bang(d, D(9, 9, 2009)), "\"%s\" sai nhung van ghi de outDate", s);
    }
}

static void testTinhToan() {
    std::printf("cong/tru/so sanh\n");
    KIEMTRA(bang(congNgay(D(31, 12, 2026), 1), D(1, 1, 2027)), "qua nam");
    KIEMTRA(bang(congNgay(D(1, 3, 2028), -1), D(29, 2, 2028)), "lui ve 29/02 nam nhuan");
    KIEMTRA(bang(congNgay(D(1, 3, 2026), -1), D(28, 2, 2026)), "lui ve 28/02");
    KIEMTRA(bang(congNgay(D(16, 11, 2026), 7), D(23, 11, 2026)), "+7 ngay");
    KIEMTRA(bang(congNgay(D(15, 6, 2026), 365), D(15, 6, 2027)), "+365 ngay");
    KIEMTRA(khoangCachNgay(D(23, 11, 2026), D(16, 11, 2026)) == 7, "khoangCachNgay = d1 - d2");
    KIEMTRA(khoangCachNgay(D(1, 1, 2027), D(1, 1, 2026)) == 365, "khoang cach 1 nam");
    KIEMTRA(soSanhDate(D(1, 1, 2026), D(2, 1, 2026)) == -1, "soSanh <");
    KIEMTRA(soSanhDate(D(2, 1, 2026), D(2, 1, 2026)) == 0, "soSanh =");
    KIEMTRA(soSanhDate(D(1, 2, 2026), D(31, 1, 2026)) == 1, "soSanh >");
    KIEMTRA(formatDate(D(5, 3, 2026)) == "05/03/2026", "formatDate them so 0");
    Date h = homNay();
    KIEMTRA(laNgayHopLe(h), "homNay khong hop le");
    std::printf("  homNay = %s\n", formatDate(h).c_str());
}

int main() {
    testMoiNgay();
    testNamNhuan();
    testParse();
    testTinhToan();
    if (soLoi == 0) std::printf("\nTAT CA TEST DATE DEU QUA\n");
    else std::printf("\nCO %d LOI\n", soLoi);
    return soLoi == 0 ? 0 : 1;
}
