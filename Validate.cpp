// Validate.cpp - Huynh Tin
#include "Validate.h"
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "Date.h"

static bool laKhoangTrang(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

std::string catKhoangTrang(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && laKhoangTrang(s[a])) ++a;
    while (b > a && laKhoangTrang(s[b - 1])) --b;
    return s.substr(a, b - a);
}

bool laSoNguyen(const std::string& s0, int& out) {
    std::string s = catKhoangTrang(s0);
    size_t i = 0;
    bool am = false;
    if (i < s.size() && (s[i] == '+' || s[i] == '-')) am = s[i++] == '-';
    if (i == s.size()) return false;
    long long v = 0;
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
        v = v * 10 + (s[i] - '0');
        if (v > (long long)INT_MAX + 1) return false;   // tran
    }
    if (am) v = -v;
    if (v < INT_MIN || v > INT_MAX) return false;
    out = (int)v;
    return true;
}

bool laSoThuc(const std::string& s0, double& out) {
    std::string s = catKhoangTrang(s0);
    if (s.empty()) return false;
    // Chi nhan chu so, mot dau cham, dau +/- o dau, e/E cho so mu (vd 1.5e6)
    for (char c : s)
        if (!((c >= '0' && c <= '9') || c == '.' || c == '+' || c == '-' || c == 'e' || c == 'E'))
            return false;
    char* het = nullptr;
    double v = std::strtod(s.c_str(), &het);
    if (het == s.c_str() || *het != '\0' || !std::isfinite(v)) return false;
    out = v;
    return true;
}

bool maVachHopLe(const std::string& s) {
    if (s.size() != 8 && s.size() != 13) return false;
    for (char c : s)
        if (c < '0' || c > '9') return false;
    return true;
}

// ---------------------------------------------------------------------------

static void baoLoi(const std::string& lyDo) { std::cout << "  -> Loi: " << lyDo << ". Nhap lai.\n"; }

std::string docDong(const std::string& nhac) {
    std::cout << nhac;
    std::cout.flush();
    std::string s;
    if (!std::getline(std::cin, s)) throw HetDauVao();
    return catKhoangTrang(s);
}

int nhapSoNguyen(const std::string& nhac, int minVal, int maxVal, const int* macDinh) {
    while (true) {
        std::string s = docDong(nhac);
        if (s.empty() && macDinh) return *macDinh;
        int v;
        if (!laSoNguyen(s, v)) baoLoi("phai la so nguyen");
        else if (v < minVal || v > maxVal)
            baoLoi("phai nam trong [" + std::to_string(minVal) + ", " + std::to_string(maxVal) + "]");
        else return v;
    }
}

double nhapSoThuc(const std::string& nhac, double minVal, double maxVal, const double* macDinh) {
    while (true) {
        std::string s = docDong(nhac);
        if (s.empty() && macDinh) return *macDinh;
        double v;
        if (!laSoThuc(s, v)) baoLoi("phai la so (dung dau cham cho phan thap phan)");
        else if (v < minVal || v > maxVal) baoLoi("gia tri ngoai mien cho phep");
        else return v;
    }
}

std::string nhapChuoi(const std::string& nhac, const std::string* macDinh) {
    while (true) {
        std::string s = docDong(nhac);
        if (s.empty() && macDinh) return *macDinh;
        if (s.empty()) baoLoi("khong duoc de trong");
        else if (s.find('|') != std::string::npos) baoLoi("khong duoc chua ky tu '|'");
        else if (s.size() > 60) baoLoi("toi da 60 ky tu");
        else return s;
    }
}

Date nhapNgay(const std::string& nhac, const Date* macDinh) {
    while (true) {
        std::string s = docDong(nhac);
        if (s.empty() && macDinh) return *macDinh;
        Date d;
        if (parseDate(s, d)) return d;
        baoLoi("ngay khong hop le (dd/mm/yyyy, nam 1900-2100)");
    }
}

std::string nhapMaVach(const std::string& nhac) {
    while (true) {
        std::string s = docDong(nhac);
        if (maVachHopLe(s)) return s;
        baoLoi("ma vach phai gom dung 8 hoac 13 chu so");
    }
}

bool nhapCoKhong(const std::string& nhac) {
    while (true) {
        std::string s = docDong(nhac);
        if (s == "c" || s == "C" || s == "y" || s == "Y") return true;
        if (s == "k" || s == "K" || s == "n" || s == "N") return false;
        baoLoi("go c (co) hoac k (khong)");
    }
}
