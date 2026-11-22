// tests/test_report.cpp - kiem thu Sort, Report (Long Vu)
// Bien dich: g++ -std=c++17 -Wall -Wextra -I. BTree.cpp Date.cpp LotQueue.cpp Phieu.cpp Inventory.cpp
//            Sort.cpp Report.cpp tests/test_report.cpp -o test_report
#include <chrono>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include "Date.h"
#include "Inventory.h"
#include "Report.h"
#include "Sort.h"

static int soLoi = 0;
#define KIEMTRA(dk, ...)                                    \
    do {                                                    \
        if (!(dk)) {                                        \
            if (++soLoi <= 30) {                            \
                std::printf("  LOI (dong %d): ", __LINE__); \
                std::printf(__VA_ARGS__);                   \
                std::printf("\n");                          \
            }                                               \
        }                                                   \
    } while (0)

static Date D(int d, int m, int y) { Date r = {d, m, y}; return r; }

template <class F> static std::string batOutput(F f) {
    std::ostringstream os;
    std::streambuf* cu = std::cout.rdbuf(os.rdbuf());
    f();
    std::cout.rdbuf(cu);
    return os.str();
}

// Danh sach ma vach theo thu tu, vd "33333333,11111111"
static std::string chuoiMa(const DsMatHang& ds) {
    std::string s;
    for (int i = 0; i < ds.n; ++i) s += (i ? "," : "") + ds.a[i]->maVach.substr(0, 1);
    return s;
}

// ---- Merge Sort ------------------------------------------------------------
struct CapSo { int khoa; int viTri; };
static bool truocCap(const CapSo& x, const CapSo& y) { return x.khoa < y.khoa; }
static unsigned hat = 2026;
static unsigned ngauNhien() { hat ^= hat << 13; hat ^= hat >> 17; hat ^= hat << 5; return hat; }

static void testMergeSort() {
    std::printf("mergeSort\n");
    const int kichThuoc[] = {0, 1, 2, 3, 7, 16, 100, 10000};
    for (int n : kichThuoc) {
        CapSo* a = new CapSo[n > 0 ? n : 1];
        long long tongTruoc = 0, tongSau = 0;
        for (int i = 0; i < n; ++i) { a[i] = {(int)(ngauNhien() % 50), i}; tongTruoc += a[i].khoa; }
        mergeSort<CapSo>(a, n, truocCap);
        bool dung = true, onDinh = true;
        for (int i = 0; i < n; ++i) {
            tongSau += a[i].khoa;
            if (i > 0 && a[i - 1].khoa > a[i].khoa) dung = false;
            if (i > 0 && a[i - 1].khoa == a[i].khoa && a[i - 1].viTri > a[i].viTri) onDinh = false;
        }
        KIEMTRA(dung && tongTruoc == tongSau, "n = %d: chua sap dung", n);
        KIEMTRA(onDinh, "n = %d: khong on dinh", n);
        delete[] a;
    }
    mergeSort<CapSo>(nullptr, 5, truocCap);   // khong duoc loi

    // Mang da sap / sap nguoc
    CapSo b[6] = {{6, 0}, {5, 1}, {4, 2}, {3, 3}, {2, 4}, {1, 5}};
    mergeSort<CapSo>(b, 6, truocCap);
    KIEMTRA(b[0].khoa == 1 && b[5].khoa == 6, "mang sap nguoc");

    // sapXepBanChay: giam dan, bang nhau thi theo ma vach
    MatHang x, y, z;
    x.maVach = "30000000"; y.maVach = "10000000"; z.maVach = "20000000";
    BanChay bc[3] = {{&x, 5}, {&y, 9}, {&z, 5}};
    sapXepBanChay(bc, 3);
    KIEMTRA(bc[0].mh == &y && bc[1].mh == &z && bc[2].mh == &x, "sapXepBanChay");

    KIEMTRA(soSanhKhongPhanBiet("Do Uong", "DO UONG") == 0 && soSanhKhongPhanBiet("abc", "ABD") < 0 &&
            soSanhKhongPhanBiet("ab", "a") > 0, "soSanhKhongPhanBiet");
}

// ---- Du lieu mau co ket qua tinh tay ---------------------------------------
//  Ma  Ten                 Nhom            Nguong  Lo (sl, gia nhap, HSD so voi homNay)
//  1   Sua tuoi Vinamilk   Thuc pham tuoi  10      (4, 20000, -1)  (6, 21000, 0)
//  2   Sua chua            Thuc pham tuoi  5       (3, 10000, +7)
//  3   Nuoc ngot Cola      Do uong         20      (50, 7000, +8)  (10, 7500, +3)
//  4   Banh quy bo         Banh keo        0       -
//  5   Tra SUA tran chau   Do uong         8       (5, 20000, +30)
//  6   Bot giat            DO UONG         1       -
static const Date HOM_NAY = {22, 11, 2026};

static void taoKho(Kho& kho) {
    khoInit(kho, 3);
    themMatHang(kho, "11111111", "Sua tuoi Vinamilk", "Thuc pham tuoi", 30000, 10);
    themMatHang(kho, "22222222", "Sua chua", "Thuc pham tuoi", 15000, 5);
    themMatHang(kho, "33333333", "Nuoc ngot Cola", "Do uong", 10000, 20);
    themMatHang(kho, "44444444", "Banh quy bo", "Banh keo", 25000, 0);
    themMatHang(kho, "55555555", "Tra SUA tran chau", "Do uong", 35000, 8);
    themMatHang(kho, "66666666", "Bot giat", "DO UONG", 50000, 1);
    Date n1 = D(1, 11, 2026), n2 = D(2, 11, 2026);
    nhapKho(kho, "11111111", 4, 20000, congNgay(HOM_NAY, -1), n1, nullptr);
    nhapKho(kho, "11111111", 6, 21000, HOM_NAY, n2, nullptr);
    nhapKho(kho, "22222222", 3, 10000, congNgay(HOM_NAY, 7), n1, nullptr);
    nhapKho(kho, "33333333", 50, 7000, congNgay(HOM_NAY, 8), n1, nullptr);
    nhapKho(kho, "33333333", 10, 7500, congNgay(HOM_NAY, 3), n2, nullptr);
    nhapKho(kho, "55555555", 5, 20000, congNgay(HOM_NAY, 30), n1, nullptr);
}

static void testTimKiem(Kho& kho) {
    std::printf("timTheoTen / lietKeTheoNhom\n");
    DsMatHang ds;
    dsInit(ds);
    timTheoTen(kho, "sua", ds);
    KIEMTRA(chuoiMa(ds) == "2,1,5", "tim 'sua' (khong phan biet hoa thuong, sap theo ten): %s", chuoiMa(ds).c_str());
    dsClear(ds);
    timTheoTen(kho, "SUA T", ds);
    KIEMTRA(chuoiMa(ds) == "1,5", "tim 'SUA T' (khop ca 'Tra SUA Tran'): %s", chuoiMa(ds).c_str());
    dsClear(ds);
    timTheoTen(kho, "xyz", ds);
    KIEMTRA(ds.n == 0 && ds.a == nullptr, "khong tim thay");
    dsClear(ds);
    timTheoTen(kho, "", ds);
    KIEMTRA(chuoiMa(ds) == "4,6,3,2,1,5", "tu khoa rong = tat ca theo ten: %s", chuoiMa(ds).c_str());
    dsClear(ds);

    lietKeTheoNhom(kho, "do uong", ds);
    KIEMTRA(chuoiMa(ds) == "6,3,5", "nhom 'do uong' gom ca 'DO UONG': %s", chuoiMa(ds).c_str());
    dsClear(ds);
    lietKeTheoNhom(kho, "", ds);
    KIEMTRA(chuoiMa(ds) == "4,6,3,5,2,1", "tat ca theo nhom roi ten: %s", chuoiMa(ds).c_str());
    dsClear(ds);
    lietKeTheoNhom(kho, "Dien tu", ds);
    KIEMTRA(ds.n == 0, "nhom khong co");
    dsClear(ds);
}

static void testCanhBao(Kho& kho) {
    std::printf("canhBaoSapHetHang / canhBaoSapHetHan\n");
    DsMatHang ds;
    dsInit(ds);
    canhBaoSapHetHang(kho, ds);
    // 1: ton 10 = nguong 10 -> khong; 4: 0 < 0 sai -> khong
    KIEMTRA(chuoiMa(ds) == "6,2,5", "ton < nguong, sap theo ton tang: %s", chuoiMa(ds).c_str());
    dsClear(ds);

    CanhBaoHSD* a = nullptr;
    int n = canhBaoSapHetHan(kho, HOM_NAY, 7, a);
    bool dung = n == 4 && a[0].mh->maVach[0] == '1' && a[0].soNgayConLai == -1 &&
                a[1].mh->maVach[0] == '1' && a[1].soNgayConLai == 0 &&
                a[2].mh->maVach[0] == '3' && a[2].soNgayConLai == 3 &&
                a[3].mh->maVach[0] == '2' && a[3].soNgayConLai == 7;
    KIEMTRA(dung, "7 ngay kem da het han: n = %d", n);
    std::string bang = batOutput([&] { inBangCanhBaoHSD(a, n); });
    KIEMTRA(bang.find("DA HET HAN") != std::string::npos && bang.find("0 ngay") != std::string::npos,
            "inBangCanhBaoHSD");
    delete[] a;

    n = canhBaoSapHetHan(kho, HOM_NAY, 7, a, false);
    KIEMTRA(n == 3 && a[0].soNgayConLai == 0, "khong kem lo da het han: n = %d", n);
    delete[] a;
    n = canhBaoSapHetHan(kho, congNgay(HOM_NAY, -100), 7, a);
    KIEMTRA(n == 0 && a == nullptr, "khong co lo nao: kq = nullptr");
}

static void testBaoCaoVaBanChay(Kho& kho) {
    std::printf("xuat kho -> baoCaoGiaTriTheoNhom / topBanChay\n");
    xuatKho(kho, "11111111", 4, D(5, 11, 2026), nullptr);    // het lo da qua HSD
    xuatKho(kho, "33333333", 20, D(10, 11, 2026), nullptr);
    xuatKho(kho, "33333333", 5, D(30, 10, 2026), nullptr);   // thang 10
    xuatKho(kho, "22222222", 1, D(15, 11, 2026), nullptr);
    xuatKho(kho, "55555555", 2, D(20, 11, 2026), nullptr);
    xuatKho(kho, "55555555", 1, D(21, 11, 2026), nullptr);
    nhapKho(kho, "44444444", 1, 1, D(1, 1, 2027), D(3, 11, 2026), nullptr);   // phieu nhap khong tinh
    xuatKho(kho, "44444444", 1, D(3, 11, 2026), nullptr);

    CanhBaoHSD* h = nullptr;
    int nh = canhBaoSapHetHan(kho, HOM_NAY, 7, h);
    KIEMTRA(nh == 3 && h[0].soNgayConLai == 0, "lo het han da xuat het khong con canh bao (n = %d)", nh);
    delete[] h;

    // Gia tri ton theo gia nhap:
    //  Do uong: 3: 25*7000 + 10*7500 = 250000; 5: 2*20000 = 40000; 6: 0  -> 290000, ton 37, 3 MH
    //  Thuc pham tuoi: 1: 6*21000 = 126000; 2: 2*10000 = 20000          -> 146000, ton 8, 2 MH
    //  Banh keo: 0
    GiaTriNhom* g = nullptr;
    int ng = baoCaoGiaTriTheoNhom(kho, g);
    bool dung = ng == 3 && soSanhKhongPhanBiet(g[0].nhom, "Do uong") == 0 && g[0].soMatHang == 3 &&
                g[0].tongTon == 37 && g[0].giaTriTon == 290000 && g[1].nhom == "Thuc pham tuoi" &&
                g[1].soMatHang == 2 && g[1].tongTon == 8 && g[1].giaTriTon == 146000 &&
                g[2].nhom == "Banh keo" && g[2].giaTriTon == 0;
    KIEMTRA(dung, "baoCaoGiaTriTheoNhom (ng = %d)", ng);
    std::string bang = batOutput([&] { inBangGiaTriNhom(g, ng); });
    KIEMTRA(bang.find("436000") != std::string::npos, "tong gia tri 436000 trong bang");
    delete[] g;

    // Thang 11: 3 -> 20, 1 -> 4, 5 -> 3, 2 -> 1, 4 -> 1
    BanChay* b = nullptr;
    int nb = topBanChay(kho, 11, 2026, 10, b);
    dung = nb == 5 && b[0].mh->maVach[0] == '3' && b[0].soLuongBan == 20 && b[1].mh->maVach[0] == '1' &&
           b[1].soLuongBan == 4 && b[2].mh->maVach[0] == '5' && b[2].soLuongBan == 3 &&
           b[3].mh->maVach[0] == '2' && b[4].mh->maVach[0] == '4';
    KIEMTRA(dung, "top thang 11 (nb = %d)", nb);
    delete[] b;
    nb = topBanChay(kho, 11, 2026, 10, b);   // goi lai: demTam phai duoc dat lai
    KIEMTRA(nb == 5 && b[0].soLuongBan == 20, "goi lan 2 khong cong don demTam");
    delete[] b;
    nb = topBanChay(kho, 11, 2026, 2, b);
    KIEMTRA(nb == 2 && b[1].mh->maVach[0] == '1', "k = 2");
    delete[] b;
    nb = topBanChay(kho, 10, 2026, 10, b);
    KIEMTRA(nb == 1 && b[0].soLuongBan == 5, "thang 10 chi co ma 3");
    delete[] b;
    nb = topBanChay(kho, 11, 2025, 10, b);
    KIEMTRA(nb == 0 && b == nullptr, "nam khac: khong co");
    nb = topBanChay(kho, 11, 2026, 0, b);
    KIEMTRA(nb == 0 && b == nullptr, "k = 0");

    xoaMatHang(kho, "22222222", true);   // phieu cua ma da xoa phai bo qua
    nb = topBanChay(kho, 11, 2026, 10, b);
    KIEMTRA(nb == 4, "bo qua phieu cua mat hang da xoa (nb = %d)", nb);
    bang = batOutput([&] { inBangBanChay(b, nb); });
    KIEMTRA(bang.find("Nuoc ngot Cola") != std::string::npos, "inBangBanChay");
    delete[] b;

    DsMatHang ds;
    dsInit(ds);
    lietKeTheoNhom(kho, "", ds);
    bang = batOutput([&] { inBangMatHang(ds); });
    KIEMTRA(bang.find("Tong: 5 mat hang") != std::string::npos, "inBangMatHang");
    dsClear(ds);
    KIEMTRA(batOutput([&] { inBangMatHang(ds); }).find("Khong co") != std::string::npos, "in bang rong");
}

static void testLon() {
    const int N = 100000;
    std::printf("du lieu lon: %d mat hang\n", N);
    Kho kho;
    khoInit(kho, 5);
    const char* nhom[] = {"Do uong", "Banh keo", "Gia vi", "Do gia dung", "Thuc pham tuoi"};
    char ma[16], ten[32];
    for (int i = 0; i < N; ++i) {
        std::snprintf(ma, sizeof ma, "893%010d", (int)((1LL * i * 7919) % 1000000000LL));
        std::snprintf(ten, sizeof ten, "San pham %d", i);
        themMatHang(kho, ma, ten, nhom[i % 5], 1000 + i % 100, 10);
        nhapKho(kho, ma, 1 + i % 20, 500, congNgay(HOM_NAY, i % 60), D(1, 11, 2026), nullptr);
        if (i % 3 == 0) xuatKho(kho, ma, 1, D(10 + i % 10, 11, 2026), nullptr);
    }
    auto bd = std::chrono::steady_clock::now();
    DsMatHang ds;
    dsInit(ds);
    timTheoTen(kho, "pham 99", ds);
    int nTim = ds.n;
    dsClear(ds);
    canhBaoSapHetHang(kho, ds);
    int nThap = ds.n;
    dsClear(ds);
    CanhBaoHSD* h;
    int nHSD = canhBaoSapHetHan(kho, HOM_NAY, 7, h);
    bool hsdTang = true;
    for (int i = 1; i < nHSD; ++i) hsdTang = hsdTang && soSanhDate(h[i - 1].lo->hsd, h[i].lo->hsd) <= 0;
    delete[] h;
    GiaTriNhom* g;
    int nNhom = baoCaoGiaTriTheoNhom(kho, g);
    delete[] g;
    BanChay* b;
    int nTop = topBanChay(kho, 11, 2026, 10, b);
    delete[] b;
    double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - bd).count();
    std::printf("  tim 'pham 99': %d, ton thap: %d, sap het han: %d, nhom: %d, top: %d (%.0f ms)\n",
                nTim, nThap, nHSD, nNhom, nTop, ms);
    // 'pham 99' khop 99, 990-999, 9900-9999, 99000-99999 = 1 + 10 + 100 + 1000
    KIEMTRA(nTim == 1111 && nNhom == 5 && nTop == 10 && hsdTang, "ket qua tren du lieu lon");
    // HSD = homNay + i%60 ngay -> trong 7 ngay khi i%60 <= 7. Nhung i%60 == 0 thi lo chi co
    // 1 + i%20 = 1 cai va bi xuat het (i%3 == 0) nen da roi khoi hang doi -> con i%60 = 1..7.
    KIEMTRA(nHSD == N / 60 * 7 + (N % 60 > 7 ? 7 : N % 60 - 1), "so lo sap het han = %d", nHSD);
    khoGiaiPhong(kho);
}

int main() {
    testMergeSort();
    Kho kho;
    taoKho(kho);
    testTimKiem(kho);
    testCanhBao(kho);
    testBaoCaoVaBanChay(kho);
    khoGiaiPhong(kho);
    testLon();
    if (soLoi == 0) std::printf("\nTAT CA TEST REPORT DEU QUA\n");
    else std::printf("\nCO %d LOI\n", soLoi);
    return soLoi == 0 ? 0 : 1;
}
