// FileIO.cpp - Huynh Tin
// Hien moi co phan quy doi ma lo / ma phieu / loai phieu giua tep va chuong trinh
// (xem quy uoc trong FileIO.h). Cac ham doc/ghi tep se them vao day.
#include "FileIO.h"
#include <cstdio>

namespace {

// s = tienTo + 1..9 chu so, gia tri > 0
bool tachSoSauTienTo(const std::string& s, const char* tienTo, int& out) {
    std::string tt = tienTo;
    if (s.size() <= tt.size() || s.size() - tt.size() > 9 || s.compare(0, tt.size(), tt) != 0)
        return false;
    int v = 0;
    for (size_t i = tt.size(); i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
        v = v * 10 + (s[i] - '0');
    }
    if (v <= 0) return false;
    out = v;
    return true;
}

std::string taoMa(const char* tienTo, int so) {
    char buf[24];
    std::snprintf(buf, sizeof buf, "%s%05d", tienTo, so);
    return buf;
}

const char* tienToPhieu(LoaiPhieu loai) { return loai == PHIEU_NHAP ? "PN" : "PX"; }

}  // namespace

bool tachMaLo(const std::string& s, int& maLo) { return tachSoSauTienTo(s, "LO", maLo); }

std::string taoMaLo(int maLo) { return taoMa("LO", maLo); }

bool tachMaPhieu(const std::string& s, LoaiPhieu loai, int& maPhieu) {
    return tachSoSauTienTo(s, tienToPhieu(loai), maPhieu);
}

std::string taoMaPhieu(int maPhieu, LoaiPhieu loai) { return taoMa(tienToPhieu(loai), maPhieu); }

bool tachLoaiPhieu(const std::string& s, LoaiPhieu& loai) {
    if (s == "NHAP") { loai = PHIEU_NHAP; return true; }
    if (s == "XUAT") { loai = PHIEU_XUAT; return true; }
    return false;
}

const char* tenLoaiPhieu(LoaiPhieu loai) { return loai == PHIEU_NHAP ? "NHAP" : "XUAT"; }
