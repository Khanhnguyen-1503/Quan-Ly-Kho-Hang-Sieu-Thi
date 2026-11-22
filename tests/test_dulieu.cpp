// tests/test_dulieu.cpp - kiem tra tep du lieu mathang.txt, lo.txt, phieu.txt
// Bien dich: g++ -std=c++17 -O2 -Wall -Wextra -I. BTree.cpp Date.cpp DataGen.cpp FileIO.cpp tests/test_dulieu.cpp -o test_dulieu
// Chay:      ./test_dulieu data                 kiem tra 3 tep trong thu muc data/
//            ./test_dulieu <thuMuc> --sinh N    sinh N mat hang bang sinhDuLieuMau roi kiem tra
//            ./test_dulieu <thuMuc> --lon N     sinh N mat hang bang sinhDuLieuLon, nap vao B-Tree
// LOI = vi pham dinh dang/rang buoc; CANH BAO = lech voi quy uoc trong Types.h / tai lieu.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include "BTree.h"
#include "DataGen.h"
#include "Date.h"
#include "FileIO.h"

static int soLoi = 0, soCanhBao = 0;

// In toi da 5 vi du cho moi loai van de
struct Loai { const char* ten; int dem; };
static Loai dsLoai[64];
static int soLoaiDaCo = 0;
static void baoCao(bool laLoi, const char* loai, const std::string& chiTiet) {
    (laLoi ? soLoi : soCanhBao)++;
    int k = 0;
    while (k < soLoaiDaCo && std::strcmp(dsLoai[k].ten, loai) != 0) ++k;
    if (k == soLoaiDaCo && soLoaiDaCo < 64) dsLoai[soLoaiDaCo++] = {loai, 0};
    if (k < 64 && ++dsLoai[k].dem <= 5)
        std::printf("  %s [%s] %s\n", laLoi ? "LOI      " : "CANH BAO ", loai, chiTiet.c_str());
}

// ---- Doc tep ---------------------------------------------------------------

static int demDong(const std::string& tep) {
    std::ifstream f(tep.c_str());
    std::string s;
    int n = 0;
    while (std::getline(f, s)) ++n;
    return n;
}

// Tach dong theo '|', toi da 8 truong. Tra ve so truong.
static int tach(const std::string& dong, std::string* tr) {
    int n = 0;
    size_t bd = 0;
    while (n < 8) {
        size_t p = dong.find('|', bd);
        tr[n++] = dong.substr(bd, p == std::string::npos ? std::string::npos : p - bd);
        if (p == std::string::npos) break;
        bd = p + 1;
    }
    return n;
}

static bool laSoNguyen(const std::string& s, long long& out) {
    if (s.empty()) return false;
    char* e;
    out = std::strtoll(s.c_str(), &e, 10);
    return *e == '\0';
}
static bool laSoThuc(const std::string& s, double& out) {
    if (s.empty()) return false;
    char* e;
    out = std::strtod(s.c_str(), &e);
    return *e == '\0';
}
static bool maVachHopLe(const std::string& s) {
    if (s.size() != 8 && s.size() != 13) return false;
    for (char c : s)
        if (c < '0' || c > '9') return false;
    return true;
}

struct LoDoc { std::string maVach, maLo; int so; Date nhap, hsd; int sl; };
struct PhieuDoc { std::string ma; int so; bool xuat; Date ngay; std::string maVach; int sl; };

static MatHang* taoMH(const std::string& ma) {
    MatHang* mh = new MatHang;
    mh->maVach = ma;
    mh->donGia = 0;
    mh->tonKho = 0;          // tong soLuong cac lo
    mh->nguongCanhBao = 0;
    mh->dsLo.front = mh->dsLo.rear = nullptr;
    mh->dsLo.count = 0;
    mh->demTam = 0;          // dung tam: tong nhap - tong xuat theo phieu
    return mh;
}

static void thuVienDong(std::string& s, bool& coCR) {
    if (!s.empty() && s[s.size() - 1] == '\r') { s.erase(s.size() - 1); coCR = true; }
}

// ---- Kiem tra --------------------------------------------------------------

// Kiem thu cac ham quy doi ma trong FileIO
static void testQuyDoiMa() {
    int so = -1;
    LoaiPhieu lp;
    struct { const char* s; bool ok; int so; } lo[] = {
        {"LO00001", true, 1}, {"LO12345", true, 12345}, {"LO123456", true, 123456}, {"LO7", true, 7},
        {"LO999999999", true, 999999999}, {"LO1234567890", false, 0}, {"LO00000", false, 0},
        {"LO", false, 0}, {"lo00001", false, 0}, {"L000001", false, 0}, {"00001", false, 0},
        {"LO0000a", false, 0}, {"LO-0001", false, 0}, {"LO 0001", false, 0}, {"", false, 0}};
    for (auto& t : lo) {
        bool ok = tachMaLo(t.s, so);
        if (ok != t.ok || (ok && so != t.so)) baoCao(true, "tachMaLo", t.s);
    }
    if (taoMaLo(1) != "LO00001" || taoMaLo(123456) != "LO123456") baoCao(true, "taoMaLo", "");
    if (!tachMaPhieu("PN00006", PHIEU_NHAP, so) || so != 6) baoCao(true, "tachMaPhieu", "PN00006");
    if (!tachMaPhieu("PX00042", PHIEU_XUAT, so) || so != 42) baoCao(true, "tachMaPhieu", "PX00042");
    if (tachMaPhieu("PX00006", PHIEU_NHAP, so)) baoCao(true, "tachMaPhieu nhan sai loai", "PX00006 + NHAP");
    if (tachMaPhieu("PN00006", PHIEU_XUAT, so)) baoCao(true, "tachMaPhieu nhan sai loai", "PN00006 + XUAT");
    if (tachMaPhieu("6", PHIEU_NHAP, so)) baoCao(true, "tachMaPhieu nhan so tran", "6");
    if (taoMaPhieu(6, PHIEU_NHAP) != "PN00006" || taoMaPhieu(6, PHIEU_XUAT) != "PX00006")
        baoCao(true, "taoMaPhieu", "");
    if (!tachLoaiPhieu("NHAP", lp) || lp != PHIEU_NHAP || !tachLoaiPhieu("XUAT", lp) || lp != PHIEU_XUAT ||
        tachLoaiPhieu("nhap", lp) || tachLoaiPhieu("", lp))
        baoCao(true, "tachLoaiPhieu", "");
    if (std::string(tenLoaiPhieu(PHIEU_NHAP)) != "NHAP" || std::string(tenLoaiPhieu(PHIEU_XUAT)) != "XUAT")
        baoCao(true, "tenLoaiPhieu", "");
    // Doc roi ghi lai phai ra dung chuoi cu
    for (int v = 1; v <= 200000; v += 997) {
        if (!tachMaLo(taoMaLo(v), so) || so != v) baoCao(true, "LO khu hoi", taoMaLo(v));
        if (!tachMaPhieu(taoMaPhieu(v, PHIEU_XUAT), PHIEU_XUAT, so) || so != v)
            baoCao(true, "PX khu hoi", taoMaPhieu(v, PHIEU_XUAT));
    }
}

static void kiemTraThuMuc(const std::string& thuMuc) {
    std::string tepMH = thuMuc + "/mathang.txt", tepLo = thuMuc + "/lo.txt", tepP = thuMuc + "/phieu.txt";
    BTree cay;
    btInit(cay, 5);
    bool coCR = false;
    std::string tr[8], s;
    char buf[256];

    // mathang.txt
    int capMH = demDong(tepMH) + 1;
    MatHang** dsMH = new MatHang*[capMH];
    int nMH = 0, dong = 0;
    std::ifstream fMH(tepMH.c_str());
    if (!fMH) baoCao(true, "thieu tep", tepMH);
    while (std::getline(fMH, s)) {
        ++dong;
        thuVienDong(s, coCR);
        if (s.empty()) continue;
        std::snprintf(buf, sizeof buf, "mathang.txt dong %d: ", dong);
        std::string vt = buf;
        long long ng; double gia;
        if (tach(s, tr) != 5) { baoCao(true, "so truong", vt + s); continue; }
        if (!maVachHopLe(tr[0])) baoCao(true, "ma vach", vt + tr[0]);
        if (tr[1].empty() || tr[2].empty()) baoCao(true, "ten/nhom rong", vt + s);
        if (!laSoThuc(tr[3], gia) || gia <= 0) baoCao(true, "donGia", vt + tr[3]);
        if (!laSoNguyen(tr[4], ng) || ng < 0) baoCao(true, "nguongCanhBao", vt + tr[4]);
        MatHang* mh = taoMH(tr[0]);
        mh->ten = tr[1];
        mh->nhom = tr[2];
        if (!btInsert(cay, mh)) { baoCao(true, "trung ma vach", vt + tr[0]); delete mh; continue; }
        dsMH[nMH++] = mh;
    }

    // lo.txt
    int capLo = demDong(tepLo) + 1;
    LoDoc* dsLo = new LoDoc[capLo];
    int nLo = 0;
    dong = 0;
    Date hn = homNay();
    int loHetHan = 0;
    std::ifstream fLo(tepLo.c_str());
    if (!fLo) baoCao(true, "thieu tep", tepLo);
    while (std::getline(fLo, s)) {
        ++dong;
        thuVienDong(s, coCR);
        if (s.empty()) continue;
        std::snprintf(buf, sizeof buf, "lo.txt dong %d: ", dong);
        std::string vt = buf;
        long long sl; double gia;
        LoDoc lo;
        if (tach(s, tr) != 6) { baoCao(true, "so truong", vt + s); continue; }
        lo.maVach = tr[0];
        lo.maLo = tr[1];
        MatHang* mh = btSearch(cay, tr[0]);
        if (!mh) baoCao(true, "lo cua ma vach khong ton tai", vt + tr[0]);
        if (!tachMaLo(tr[1], lo.so)) { baoCao(true, "maLo sai dinh dang (LO + so)", vt + tr[1]); continue; }
        if (taoMaLo(lo.so) != tr[1])
            baoCao(false, "maLo khong o dang chuan", vt + tr[1] + " -> khi ghi lai se thanh " + taoMaLo(lo.so));
        bool ngayOk = parseDate(tr[2], lo.nhap) && parseDate(tr[3], lo.hsd);
        if (!ngayOk) baoCao(true, "ngay sai", vt + tr[2] + " / " + tr[3]);
        else if (soSanhDate(lo.nhap, lo.hsd) >= 0) baoCao(true, "hsd <= ngayNhap", vt + s);
        if (!laSoNguyen(tr[4], sl) || sl <= 0) { baoCao(true, "soLuong", vt + tr[4]); sl = 0; }
        if (!laSoThuc(tr[5], gia) || gia <= 0) baoCao(true, "giaNhap", vt + tr[5]);
        for (int k = 0; k < nLo; ++k)
            if (dsLo[k].so == lo.so) { baoCao(true, "trung maLo", vt + lo.maLo + " va " + dsLo[k].maLo); break; }
        if (ngayOk && soSanhDate(lo.hsd, hn) < 0) ++loHetHan;
        lo.sl = (int)sl;
        if (mh) mh->tonKho += lo.sl;
        if (ngayOk) dsLo[nLo++] = lo;
    }

    // phieu.txt
    int capP = demDong(tepP) + 1;
    PhieuDoc* dsP = new PhieuDoc[capP];
    int nP = 0;
    dong = 0;
    std::ifstream fP(tepP.c_str());
    if (!fP) baoCao(true, "thieu tep", tepP);
    while (std::getline(fP, s)) {
        ++dong;
        thuVienDong(s, coCR);
        if (s.empty()) continue;
        std::snprintf(buf, sizeof buf, "phieu.txt dong %d: ", dong);
        std::string vt = buf;
        long long sl; double gia;
        PhieuDoc p;
        if (tach(s, tr) != 6) { baoCao(true, "so truong", vt + s); continue; }
        p.ma = tr[0];
        LoaiPhieu loai;
        if (!tachLoaiPhieu(tr[1], loai)) { baoCao(true, "loai phieu (NHAP/XUAT)", vt + tr[1]); continue; }
        p.xuat = loai == PHIEU_XUAT;
        if (!tachMaPhieu(tr[0], loai, p.so)) {
            baoCao(true, "maPhieu sai dinh dang hoac tien to khong khop loai (PN/PX + so)", vt + s);
            continue;
        }
        if (taoMaPhieu(p.so, loai) != tr[0])
            baoCao(false, "maPhieu khong o dang chuan", vt + tr[0] + " -> khi ghi lai se thanh " + taoMaPhieu(p.so, loai));
        if (!parseDate(tr[2], p.ngay)) { baoCao(true, "ngay sai", vt + tr[2]); continue; }
        p.maVach = tr[3];
        MatHang* mh = btSearch(cay, tr[3]);
        if (!mh) baoCao(true, "phieu cua ma vach khong ton tai", vt + tr[3]);
        if (!laSoNguyen(tr[4], sl) || sl <= 0) { baoCao(true, "soLuong", vt + tr[4]); sl = 0; }
        if (!laSoThuc(tr[5], gia) || gia <= 0) baoCao(true, "donGia", vt + tr[5]);
        for (int k = 0; k < nP; ++k)
            if (dsP[k].so == p.so) {   // PN va PX dung chung day so
                baoCao(true, "trung so phieu", vt + p.ma + " va " + dsP[k].ma);
                break;
            }
        p.sl = (int)sl;
        if (mh) mh->demTam += p.xuat ? -p.sl : p.sl;
        dsP[nP++] = p;
    }

    bool kiemTraKy = (long long)nP * (nLo + nP) <= 200000000LL;

    // Moi lo phai co phieu nhap (cung ma vach, cung ngay) va con lai <= so da nhap.
    // FIFO: neu mot lo con hang thi moi lo nhap sau no phai con nguyen (chua bi xuat).
    if (kiemTraKy) {
        for (int k = 0; k < nLo; ++k) {
            const LoDoc& lo = dsLo[k];
            int goc = -1;
            for (int i = 0; i < nP && goc < 0; ++i)
                if (!dsP[i].xuat && dsP[i].maVach == lo.maVach && soSanhDate(dsP[i].ngay, lo.nhap) == 0)
                    goc = dsP[i].sl;
            if (goc < 0) { baoCao(true, "lo khong co phieu nhap", lo.maLo + " " + lo.maVach); continue; }
            if (lo.sl > goc) baoCao(true, "lo con nhieu hon so da nhap", lo.maLo + " " + lo.maVach);
            for (int i = 0; i < nP; ++i) {
                const PhieuDoc& p = dsP[i];
                if (p.xuat || p.maVach != lo.maVach || soSanhDate(p.ngay, lo.nhap) <= 0) continue;
                bool conNguyen = false;
                for (int q = 0; q < nLo; ++q)
                    if (dsLo[q].maVach == p.maVach && soSanhDate(dsLo[q].nhap, p.ngay) == 0) {
                        conNguyen = dsLo[q].sl == p.sl;
                        break;
                    }
                if (!conNguyen)
                    baoCao(true, "sai FIFO: lo cu con hang nhung lo moi hon da bi xuat",
                           lo.maLo + " " + lo.maVach);
            }
        }
    }

    // Phieu xuat khong duoc vuot so hang da nhap tinh den ngay xuat
    if (kiemTraKy) {
        for (int i = 0; i < nP; ++i) {
            if (!dsP[i].xuat) continue;
            long long daNhap = 0, daXuat = 0;
            for (int k = 0; k < nLo; ++k)
                if (dsLo[k].maVach == dsP[i].maVach && soSanhDate(dsLo[k].nhap, dsP[i].ngay) <= 0)
                    daNhap += dsLo[k].sl;
            for (int k = 0; k < nP; ++k)
                if (dsP[k].xuat && dsP[k].maVach == dsP[i].maVach && soSanhDate(dsP[k].ngay, dsP[i].ngay) <= 0)
                    daXuat += dsP[k].sl;
            if (daXuat > daNhap)
                baoCao(true, "xuat vuot hang da nhap den ngay do", dsP[i].ma + " " + dsP[i].maVach);
        }
    } else {
        std::printf("  (du lieu lon: bo qua kiem tra lo-phieu nhap, FIFO va xuat vuot nhap)\n");
    }

    // tonKho tinh tu lo phai bang tong nhap - tong xuat theo phieu
    int lech = 0;
    for (int i = 0; i < nMH; ++i) {
        MatHang* mh = dsMH[i];
        if (mh->tonKho != mh->demTam) {
            ++lech;
            std::snprintf(buf, sizeof buf, "%s: tong lo = %d, nhap - xuat theo phieu = %d",
                          mh->maVach.c_str(), mh->tonKho, mh->demTam);
            baoCao(true, "tong lo != nhap - xuat (lo.txt chua tru phan da xuat?)", buf);
        }
    }

    std::printf("\n  Doc duoc: %d mat hang, %d lo, %d phieu. B-Tree m = 5: cao %d, %d nut\n",
                nMH, nLo, nP, btHeight(cay), btSoNut(cay));
    std::printf("  So mat hang co tong lo != nhap - xuat: %d / %d\n", lech, nMH);
    std::printf("  So lo da het han tinh den %s: %d\n", formatDate(hn).c_str(), loHetHan);
    if (coCR) std::printf("  Ghi chu: tep dung xuong dong CRLF (Windows) - FileIO phai bo '\\r' cuoi dong\n");

    btDestroy(cay);
    for (int i = 0; i < nMH; ++i) delete dsMH[i];
    delete[] dsMH;
    delete[] dsLo;
    delete[] dsP;
}

static void kiemTraLon(const std::string& tep, int n) {
    sinhDuLieuLon(n, tep);
    BTree cay;
    btInit(cay, 5);
    MatHang** ds = new MatHang*[n + 1];
    int k = 0, trung = 0, saiMa = 0;
    std::ifstream f(tep.c_str());
    std::string s, tr[8];
    while (std::getline(f, s) && k <= n) {
        bool cr = false;
        thuVienDong(s, cr);
        if (tach(s, tr) != 5) continue;
        if (!maVachHopLe(tr[0])) ++saiMa;
        MatHang* mh = taoMH(tr[0]);
        if (!btInsert(cay, mh)) { ++trung; delete mh; continue; }
        ds[k++] = mh;
    }
    std::printf("  sinhDuLieuLon(%d): doc %d ma, trung %d, sai dinh dang %d, B-Tree cao %d\n",
                n, k, trung, saiMa, btHeight(cay));
    if (k != n || trung || saiMa) { ++soLoi; std::printf("  LOI: du lieu lon khong hop le\n"); }
    btDestroy(cay);
    for (int i = 0; i < k; ++i) delete ds[i];
    delete[] ds;
}

int main(int argc, char** argv) {
    testQuyDoiMa();
    std::printf("Ham quy doi maLo/maPhieu: %s\n", soLoi == 0 ? "dung" : "CO LOI");
    std::string thuMuc = argc > 1 ? argv[1] : "data";
    if (argc > 3 && std::strcmp(argv[2], "--sinh") == 0) {
        int n = std::atoi(argv[3]);
        std::printf("Sinh %d mat hang vao %s\n", n, thuMuc.c_str());
        sinhDuLieuMau(n, thuMuc + "/mathang.txt", thuMuc + "/lo.txt", thuMuc + "/phieu.txt");
    } else if (argc > 3 && std::strcmp(argv[2], "--lon") == 0) {
        kiemTraLon(thuMuc + "/mathang_lon.txt", std::atoi(argv[3]));
        return soLoi == 0 ? 0 : 1;
    }
    std::printf("Kiem tra du lieu trong %s\n", thuMuc.c_str());
    kiemTraThuMuc(thuMuc);
    std::printf("\nKet qua: %d LOI, %d CANH BAO\n", soLoi, soCanhBao);
    return soLoi == 0 ? 0 : 1;
}
