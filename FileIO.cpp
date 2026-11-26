// FileIO.cpp - Huynh Tin
// Quy doi ma lo / ma phieu giua tep va chuong trinh, doc / ghi 3 tep du lieu (xem FileIO.h).
#include "FileIO.h"
#include <climits>
#include <cstdio>
#include <fstream>
#include <iostream>
#include "BTree.h"
#include "Date.h"
#include "LotQueue.h"
#include "Phieu.h"
#include "Validate.h"

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

// ===========================================================================
// Doc / ghi tep
// ===========================================================================

namespace {

const int SO_LOI_IN_TOI_DA = 10;

struct BaoLoi {
    std::string tenTep;
    int soLoi;
};

std::string tenNgan(const std::string& duongDan) {
    size_t p = duongDan.find_last_of("/\\");
    return p == std::string::npos ? duongDan : duongDan.substr(p + 1);
}

void boQua(BaoLoi& b, int dong, const char* lyDo, const std::string& noiDung) {
    if (++b.soLoi <= SO_LOI_IN_TOI_DA)
        std::cerr << "  [" << b.tenTep << " dong " << dong << "] " << lyDo << " -> bo qua: " << noiDung << '\n';
}

KetQua ketThucDoc(const BaoLoi& b) {
    if (b.soLoi > SO_LOI_IN_TOI_DA)
        std::cerr << "  ... va " << b.soLoi - SO_LOI_IN_TOI_DA << " dong loi khac trong " << b.tenTep << '\n';
    return b.soLoi ? KQ_DU_LIEU_SAI : KQ_OK;
}

// Doc mot dong, bo '\r' cuoi dong (tep tao tren Windows)
bool docDongTep(std::ifstream& f, std::string& s) {
    if (!std::getline(f, s)) return false;
    if (!s.empty() && s[s.size() - 1] == '\r') s.erase(s.size() - 1);
    return true;
}

// Tach theo '|'. Tra ve so truong (dung lai khi vuot toiDa, tra toiDa + 1).
int tachTruong(const std::string& dong, std::string* tr, int toiDa) {
    int n = 0;
    size_t bd = 0;
    while (true) {
        size_t p = dong.find('|', bd);
        if (n == toiDa) return toiDa + 1;
        tr[n++] = dong.substr(bd, p == std::string::npos ? std::string::npos : p - bd);
        if (p == std::string::npos) return n;
        bd = p + 1;
    }
}

// So nguyen thi ghi khong co phan thap phan (130000 chu khong phai 1.3e+05)
std::string soRaChuoi(double x) {
    char buf[64];
    if (x == (double)(long long)x && x < 1e15 && x > -1e15) std::snprintf(buf, sizeof buf, "%lld", (long long)x);
    else std::snprintf(buf, sizeof buf, "%.15g", x);
    return buf;
}

// Ghi vao tep tam, xong moi thay tep that
KetQua hoanTatGhi(std::ofstream& f, const std::string& tam, const std::string& tep) {
    f.flush();
    bool tot = (bool)f;
    f.close();
    if (!tot || f.fail()) {
        std::remove(tam.c_str());
        return KQ_LOI_TEP;
    }
    std::remove(tep.c_str());   // Windows: rename khong ghi de tep da co
    if (std::rename(tam.c_str(), tep.c_str()) != 0) return KQ_LOI_TEP;
    return KQ_OK;
}

void ghiMotMatHang(MatHang* mh, void* p) {
    std::ofstream& f = *static_cast<std::ofstream*>(p);
    f << mh->maVach << '|' << mh->ten << '|' << mh->nhom << '|' << soRaChuoi(mh->donGia) << '|'
      << mh->nguongCanhBao << '\n';
}

void ghiLoMotMatHang(MatHang* mh, void* p) {
    std::ofstream& f = *static_cast<std::ofstream*>(p);
    for (const Lo* lo = mh->dsLo.front; lo; lo = lo->next)
        f << mh->maVach << '|' << taoMaLo(lo->maLo) << '|' << formatDate(lo->ngayNhap) << '|'
          << formatDate(lo->hsd) << '|' << lo->soLuong << '|' << soRaChuoi(lo->giaNhap) << '\n';
}

}  // namespace

// ---- Doc --------------------------------------------------------------------

KetQua docMatHang(Kho& kho, const std::string& tep) {
    std::ifstream f(tep.c_str());
    if (!f) return KQ_LOI_TEP;
    BaoLoi b{tenNgan(tep), 0};
    std::string s, tr[6];
    int dong = 0;
    while (docDongTep(f, s)) {
        ++dong;
        if (catKhoangTrang(s).empty()) continue;
        if (tachTruong(s, tr, 5) != 5) { boQua(b, dong, "can 5 truong", s); continue; }
        double donGia;
        int nguong;
        if (!maVachHopLe(tr[0])) { boQua(b, dong, "ma vach phai co 8 hoac 13 chu so", s); continue; }
        if (!laSoThuc(tr[3], donGia) || donGia < 0) { boQua(b, dong, "don gia sai", s); continue; }
        if (!laSoNguyen(tr[4], nguong) || nguong < 0) { boQua(b, dong, "nguong canh bao sai", s); continue; }
        KetQua kq = themMatHang(kho, tr[0], tr[1], tr[2], donGia, nguong);
        if (kq == KQ_TRUNG_MA) boQua(b, dong, "trung ma vach", s);
        else if (kq != KQ_OK) boQua(b, dong, "ten hoac nhom rong", s);
    }
    return ketThucDoc(b);
}

KetQua docLo(Kho& kho, const std::string& tep) {
    std::ifstream f(tep.c_str());
    if (!f) return KQ_LOI_TEP;
    BaoLoi b{tenNgan(tep), 0};
    std::string s, tr[7];
    int dong = 0;
    while (docDongTep(f, s)) {
        ++dong;
        if (catKhoangTrang(s).empty()) continue;
        if (tachTruong(s, tr, 6) != 6) { boQua(b, dong, "can 6 truong", s); continue; }
        MatHang* mh = btSearch(kho.cay, tr[0]);
        int maLo, soLuong;
        double giaNhap;
        Date ngayNhap, hsd;
        if (!mh) { boQua(b, dong, "ma vach khong co trong mathang.txt", s); continue; }
        if (!tachMaLo(tr[1], maLo)) { boQua(b, dong, "ma lo sai (LO + so)", s); continue; }
        if (!parseDate(tr[2], ngayNhap) || !parseDate(tr[3], hsd)) { boQua(b, dong, "ngay sai", s); continue; }
        if (soSanhDate(hsd, ngayNhap) < 0) { boQua(b, dong, "HSD truoc ngay nhap", s); continue; }
        if (!laSoNguyen(tr[4], soLuong) || soLuong <= 0) { boQua(b, dong, "so luong phai > 0", s); continue; }
        if (!laSoThuc(tr[5], giaNhap) || giaNhap < 0) { boQua(b, dong, "gia nhap sai", s); continue; }
        if (mh->dsLo.rear && soSanhDate(ngayNhap, mh->dsLo.rear->ngayNhap) < 0) {
            boQua(b, dong, "lo nhap truoc lo dung truoc no (sai thu tu FIFO)", s);
            continue;
        }
        if (soLuong > INT_MAX - mh->tonKho) { boQua(b, dong, "tong ton kho vuot gioi han", s); continue; }
        lqEnqueue(mh->dsLo, taoLo(maLo, ngayNhap, hsd, soLuong, giaNhap));
        mh->tonKho += soLuong;
        if (maLo >= kho.maLoKeTiep) kho.maLoKeTiep = maLo + 1;
    }
    return ketThucDoc(b);
}

KetQua docPhieu(Kho& kho, const std::string& tep) {
    std::ifstream f(tep.c_str());
    if (!f) return KQ_LOI_TEP;
    BaoLoi b{tenNgan(tep), 0};
    std::string s, tr[7];
    int dong = 0;
    int maTruoc = 0;
    while (docDongTep(f, s)) {
        ++dong;
        if (catKhoangTrang(s).empty()) continue;
        if (tachTruong(s, tr, 6) != 6) { boQua(b, dong, "can 6 truong", s); continue; }
        LoaiPhieu loai;
        int maPhieu, soLuong;
        double donGia;
        Date ngay;
        if (!tachLoaiPhieu(tr[1], loai)) { boQua(b, dong, "loai phieu phai la NHAP/XUAT", s); continue; }
        if (!tachMaPhieu(tr[0], loai, maPhieu)) { boQua(b, dong, "ma phieu sai (PN/PX + so, khop loai)", s); continue; }
        if (maPhieu <= maTruoc) { boQua(b, dong, "ma phieu trung hoac khong tang dan", s); continue; }
        if (!parseDate(tr[2], ngay)) { boQua(b, dong, "ngay sai", s); continue; }
        if (!maVachHopLe(tr[3])) { boQua(b, dong, "ma vach sai", s); continue; }
        if (!laSoNguyen(tr[4], soLuong) || soLuong <= 0) { boQua(b, dong, "so luong phai > 0", s); continue; }
        if (!laSoThuc(tr[5], donGia) || donGia < 0) { boQua(b, dong, "don gia sai", s); continue; }
        Phieu* p = new Phieu;
        p->maPhieu = maPhieu;
        p->loai = loai;
        p->ngay = ngay;
        p->maVach = tr[3];
        p->soLuong = soLuong;
        p->donGia = donGia;
        p->next = nullptr;
        dspThemCoSan(kho.phieu, p);
        maTruoc = maPhieu;
    }
    return ketThucDoc(b);
}

KetQua docDuLieu(Kho& kho, const std::string& thuMuc) {
    KetQua kq = docMatHang(kho, thuMuc + "/mathang.txt");
    if (kq == KQ_LOI_TEP) return KQ_LOI_TEP;
    const char* tepPhu[2] = {"/lo.txt", "/phieu.txt"};
    for (int i = 0; i < 2; ++i) {
        KetQua k = i == 0 ? docLo(kho, thuMuc + tepPhu[i]) : docPhieu(kho, thuMuc + tepPhu[i]);
        if (k == KQ_LOI_TEP) std::cerr << "  Khong mo duoc " << thuMuc << tepPhu[i] << ", coi nhu rong\n";
        if (k != KQ_OK) kq = KQ_DU_LIEU_SAI;
    }
    return kq;
}

// ---- Ghi --------------------------------------------------------------------

KetQua ghiMatHang(const Kho& kho, const std::string& tep) {
    std::string tam = tep + ".tmp";
    std::ofstream f(tam.c_str());
    if (!f) return KQ_LOI_TEP;
    btTraverse(kho.cay, ghiMotMatHang, &f);
    return hoanTatGhi(f, tam, tep);
}

KetQua ghiLo(const Kho& kho, const std::string& tep) {
    std::string tam = tep + ".tmp";
    std::ofstream f(tam.c_str());
    if (!f) return KQ_LOI_TEP;
    btTraverse(kho.cay, ghiLoMotMatHang, &f);
    return hoanTatGhi(f, tam, tep);
}

KetQua ghiPhieu(const Kho& kho, const std::string& tep) {
    std::string tam = tep + ".tmp";
    std::ofstream f(tam.c_str());
    if (!f) return KQ_LOI_TEP;
    for (const Phieu* p = kho.phieu.head; p; p = p->next)
        f << taoMaPhieu(p->maPhieu, p->loai) << '|' << tenLoaiPhieu(p->loai) << '|' << formatDate(p->ngay) << '|'
          << p->maVach << '|' << p->soLuong << '|' << soRaChuoi(p->donGia) << '\n';
    return hoanTatGhi(f, tam, tep);
}

KetQua ghiDuLieu(const Kho& kho, const std::string& thuMuc) {
    KetQua a = ghiMatHang(kho, thuMuc + "/mathang.txt");
    KetQua b = ghiLo(kho, thuMuc + "/lo.txt");
    KetQua c = ghiPhieu(kho, thuMuc + "/phieu.txt");
    return (a == KQ_OK && b == KQ_OK && c == KQ_OK) ? KQ_OK : KQ_LOI_TEP;
}
