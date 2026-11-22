// tests/test_kho.cpp - kiem thu LotQueue, Phieu, Inventory (Minh Tan)
// Bien dich: g++ -std=c++17 -Wall -Wextra -I. BTree.cpp Date.cpp LotQueue.cpp Phieu.cpp Inventory.cpp tests/test_kho.cpp -o test_kho
#include <climits>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include "BTree.h"
#include "Inventory.h"
#include "LotQueue.h"
#include "Phieu.h"

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

// Chay f va tra ve nhung gi duoc in ra cout
template <class F> static std::string batOutput(F f) {
    std::ostringstream os;
    std::streambuf* cu = std::cout.rdbuf(os.rdbuf());
    f();
    std::cout.rdbuf(cu);
    return os.str();
}
static int demDong(const std::string& s) {
    int n = 0;
    for (char c : s) n += c == '\n';
    return n;
}

// ---------------------------------------------------------------------------
static void testLotQueue() {
    std::printf("LotQueue\n");
    LotQueue q;
    lqInit(q);
    KIEMTRA(lqRong(q) && q.count == 0 && lqFront(q) == nullptr, "hang doi moi phai rong");
    KIEMTRA(lqDequeue(q) == nullptr, "dequeue hang doi rong phai tra nullptr");
    KIEMTRA(lqTongSoLuong(q) == 0, "tong hang doi rong");
    lqEnqueue(q, nullptr);
    KIEMTRA(q.count == 0, "enqueue nullptr khong duoc tang count");

    Lo* a = taoLo(1, D(1, 11, 2026), D(1, 12, 2026), 10, 5000);
    Lo* b = taoLo(2, D(2, 11, 2026), D(2, 12, 2026), 20, 5000);
    Lo* c = taoLo(3, D(3, 11, 2026), D(3, 12, 2026), 30, 5000);
    KIEMTRA(a->next == nullptr && a->maLo == 1 && a->soLuong == 10, "taoLo");
    lqEnqueue(q, a);
    lqEnqueue(q, b);
    lqEnqueue(q, c);
    KIEMTRA(q.count == 3 && lqFront(q) == a && q.rear == c, "front = lo cu nhat, rear = lo moi nhat");
    KIEMTRA(lqTongSoLuong(q) == 60, "tong = 60");
    KIEMTRA(lqDequeue(q) == a && a->next == nullptr, "dequeue lan 1 = lo cu nhat, next = nullptr");
    KIEMTRA(lqDequeue(q) == b && lqFront(q) == c && q.count == 1, "dequeue lan 2");
    KIEMTRA(lqDequeue(q) == c && lqRong(q) && q.rear == nullptr && q.count == 0,
            "dequeue het: front = rear = nullptr");
    delete a; delete b; delete c;

    // enqueue sau khi da rong, roi clear
    lqEnqueue(q, taoLo(4, D(1, 1, 2027), D(1, 2, 2027), 5, 1000));
    lqEnqueue(q, taoLo(5, D(2, 1, 2027), D(2, 2, 2027), 6, 1000));
    KIEMTRA(q.count == 2 && lqTongSoLuong(q) == 11, "enqueue lai sau khi rong");
    lqClear(q);
    KIEMTRA(lqRong(q) && q.rear == nullptr && q.count == 0, "lqClear");
}

// ---------------------------------------------------------------------------
static void testPhieu() {
    std::printf("Phieu\n");
    DanhSachPhieu ds;
    dspInit(ds);
    KIEMTRA(ds.head == nullptr && ds.count == 0 && ds.maPhieuKeTiep == 1, "dspInit");
    Phieu* p1 = dspThem(ds, PHIEU_NHAP, D(1, 11, 2026), "12345678", 10, 5000);
    Phieu* p2 = dspThem(ds, PHIEU_XUAT, D(5, 11, 2026), "12345678", 4, 7000);
    KIEMTRA(p1->maPhieu == 1 && p2->maPhieu == 2 && ds.maPhieuKeTiep == 3, "maPhieu tu tang");
    KIEMTRA(ds.head == p1 && ds.tail == p2 && p1->next == p2 && p2->next == nullptr, "them vao cuoi");

    // Phieu doc tu tep giu ma cu; maPhieuKeTiep phai vuot ma lon nhat
    Phieu* p50 = new Phieu{50, PHIEU_NHAP, D(10, 11, 2026), "87654321", 3, 1000, nullptr};
    dspThemCoSan(ds, p50);
    KIEMTRA(ds.tail == p50 && ds.count == 3 && ds.maPhieuKeTiep == 51, "dspThemCoSan cap nhat maPhieuKeTiep");
    Phieu* p7 = new Phieu{7, PHIEU_XUAT, D(20, 11, 2026), "87654321", 1, 2000, nullptr};
    dspThemCoSan(ds, p7);
    KIEMTRA(ds.maPhieuKeTiep == 51, "ma nho hon khong lam giam maPhieuKeTiep");
    KIEMTRA(dspThem(ds, PHIEU_NHAP, D(21, 11, 2026), "87654321", 1, 1)->maPhieu == 51, "phieu moi sau khi doc tep");
    dspThemCoSan(ds, nullptr);
    KIEMTRA(ds.count == 5, "dspThemCoSan(nullptr) khong lam gi");

    KIEMTRA(dspTimTheoMa(ds, 50) == p50 && dspTimTheoMa(ds, 2) == p2, "dspTimTheoMa");
    KIEMTRA(dspTimTheoMa(ds, 999) == nullptr, "tim ma khong co");

    // In: 7 dong khung + so phieu
    int tatCa = demDong(batOutput([&] { dspIn(ds, nullptr, nullptr); }));
    Date tu = D(5, 11, 2026), den = D(20, 11, 2026);
    int loc = demDong(batOutput([&] { dspIn(ds, &tu, &den); }));
    KIEMTRA(tatCa - loc == 2, "loc theo ngay [05/11, 20/11] phai bo 2 phieu (in %d / %d dong)", loc, tatCa);

    dspClear(ds);
    KIEMTRA(ds.head == nullptr && ds.tail == nullptr && ds.count == 0, "dspClear");
}

// ---------------------------------------------------------------------------
static void testMatHang() {
    std::printf("Inventory: them / sua / xoa / tim\n");
    Kho kho;
    khoInit(kho, 3);
    KIEMTRA(themMatHang(kho, "8930000000001", "Sua tuoi", "Thuc pham tuoi", 30000, 10) == KQ_OK, "them hop le 13 so");
    KIEMTRA(themMatHang(kho, "12345678", "Keo", "Banh keo", 5000, 0) == KQ_OK, "them hop le 8 so");
    KIEMTRA(themMatHang(kho, "8930000000001", "Trung", "Thuc pham tuoi", 1, 1) == KQ_TRUNG_MA, "trung ma");
    const char* maSai[] = {"1234567", "123456789", "893000000000", "89300000000011", "8930000O00001", "", "1234 678"};
    for (const char* m : maSai)
        KIEMTRA(themMatHang(kho, m, "X", "Y", 1, 1) == KQ_DU_LIEU_SAI, "ma vach sai \"%s\" phai bi tu choi", m);
    KIEMTRA(themMatHang(kho, "11111111", "", "Y", 1, 1) == KQ_DU_LIEU_SAI, "ten rong");
    KIEMTRA(themMatHang(kho, "11111111", "X", "", 1, 1) == KQ_DU_LIEU_SAI, "nhom rong");
    KIEMTRA(themMatHang(kho, "11111111", "X", "Y", -1, 1) == KQ_DU_LIEU_SAI, "don gia am");
    KIEMTRA(themMatHang(kho, "11111111", "X", "Y", NAN, 1) == KQ_DU_LIEU_SAI, "don gia NaN");
    KIEMTRA(themMatHang(kho, "11111111", "X", "Y", INFINITY, 1) == KQ_DU_LIEU_SAI, "don gia vo cung");
    KIEMTRA(themMatHang(kho, "11111111", "X", "Y", 1, -1) == KQ_DU_LIEU_SAI, "nguong am");
    KIEMTRA(kho.cay.size == 2, "chi 2 mat hang hop le duoc them (size = %d)", kho.cay.size);

    MatHang* mh = timMatHang(kho, "8930000000001");
    KIEMTRA(mh && mh->tonKho == 0 && mh->demTam == 0 && lqRong(mh->dsLo), "mat hang moi: ton 0, chua co lo");
    KIEMTRA(timMatHang(kho, "99999999") == nullptr, "tim ma khong co");

    KIEMTRA(suaMatHang(kho, "99999999", "A", "B", 1, 1) == KQ_KHONG_TIM_THAY, "sua ma khong co");
    KIEMTRA(suaMatHang(kho, "12345678", "", "B", 1, 1) == KQ_DU_LIEU_SAI, "sua ten rong");
    KIEMTRA(suaMatHang(kho, "12345678", "Keo deo", "Banh keo", 6000, 5) == KQ_OK, "sua hop le");
    MatHang* keo = timMatHang(kho, "12345678");
    KIEMTRA(keo->ten == "Keo deo" && keo->donGia == 6000 && keo->nguongCanhBao == 5, "noi dung sau khi sua");

    KIEMTRA(nhapKho(kho, "12345678", 5, 4000, D(1, 1, 2027), D(1, 11, 2026), nullptr) == KQ_OK, "nhap");
    KIEMTRA(xoaMatHang(kho, "12345678", false) == KQ_CON_TON_KHO, "xoa khi con ton phai tu choi");
    KIEMTRA(timMatHang(kho, "12345678") != nullptr, "mat hang van con sau khi tu choi xoa");
    KIEMTRA(xoaMatHang(kho, "12345678", true) == KQ_OK, "xoa ke ca ton kho");
    KIEMTRA(timMatHang(kho, "12345678") == nullptr && kho.cay.size == 1, "da go khoi cay");
    KIEMTRA(xoaMatHang(kho, "12345678", true) == KQ_KHONG_TIM_THAY, "xoa lan 2");
    KIEMTRA(xoaMatHang(kho, "8930000000001", false) == KQ_OK, "xoa mat hang ton = 0");
    khoGiaiPhong(kho);
    KIEMTRA(kho.cay.root == nullptr && kho.phieu.head == nullptr, "khoGiaiPhong");
}

// ---------------------------------------------------------------------------
static void testNhapXuat() {
    std::printf("Inventory: nhap kho / xuat kho FIFO\n");
    Kho kho;
    khoInit(kho, 5);
    const std::string ma = "8930000000002";
    themMatHang(kho, ma, "Nuoc ngot", "Do uong", 12000, 10);
    MatHang* mh = timMatHang(kho, ma);
    int maPhieu = -1;

    // Nhap sai
    KIEMTRA(nhapKho(kho, "00000000", 5, 1, D(1, 1, 2027), D(1, 11, 2026), nullptr) == KQ_KHONG_TIM_THAY, "nhap ma khong co");
    KIEMTRA(nhapKho(kho, ma, 0, 1, D(1, 1, 2027), D(1, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "nhap so luong 0");
    KIEMTRA(nhapKho(kho, ma, -3, 1, D(1, 1, 2027), D(1, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "nhap so luong am");
    KIEMTRA(nhapKho(kho, ma, 5, -1, D(1, 1, 2027), D(1, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "gia nhap am");
    KIEMTRA(nhapKho(kho, ma, 5, 1, D(31, 10, 2026), D(1, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "hsd truoc ngay nhap");
    KIEMTRA(nhapKho(kho, ma, 5, 1, D(30, 2, 2027), D(1, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "hsd 30/02");
    KIEMTRA(nhapKho(kho, ma, 5, 1, D(1, 1, 2027), D(31, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "ngay nhap 31/11");
    KIEMTRA(nhapKho(kho, ma, 5, 1, D(1, 1, 2027), D(1, 11, 1899), nullptr) == KQ_DU_LIEU_SAI, "nam ngoai 1900-2100 (Date.h)");
    KIEMTRA(nhapKho(kho, ma, 5, 1, D(1, 1, 2101), D(1, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "hsd nam 2101");
    KIEMTRA(mh->tonKho == 0 && kho.phieu.count == 0 && kho.maLoKeTiep == 1, "nhap sai khong duoc thay doi gi");

    // Nhap 3 lo: 10, 20, 30
    KIEMTRA(nhapKho(kho, ma, 10, 8000, D(1, 12, 2026), D(1, 11, 2026), &maPhieu) == KQ_OK && maPhieu == 1, "nhap lo 1");
    KIEMTRA(nhapKho(kho, ma, 20, 8500, D(1, 1, 2027), D(5, 11, 2026), &maPhieu) == KQ_OK && maPhieu == 2, "nhap lo 2");
    KIEMTRA(nhapKho(kho, ma, 30, 9000, D(1, 2, 2027), D(10, 11, 2026), &maPhieu) == KQ_OK && maPhieu == 3, "nhap lo 3");
    KIEMTRA(mh->tonKho == 60 && mh->dsLo.count == 3 && kho.maLoKeTiep == 4, "ton = 60, 3 lo");
    KIEMTRA(mh->dsLo.front->maLo == 1 && mh->dsLo.rear->maLo == 3, "thu tu lo");
    KIEMTRA(mh->dsLo.front->hsd.m == 12 && mh->dsLo.front->ngayNhap.d == 1, "hsd/ngayNhap gan dung tham so");
    Phieu* pn = dspTimTheoMa(kho.phieu, 2);
    KIEMTRA(pn && pn->loai == PHIEU_NHAP && pn->soLuong == 20 && pn->donGia == 8500, "phieu nhap ghi gia nhap");
    KIEMTRA(nhapKho(kho, ma, 1, 1, D(1, 1, 2027), D(1, 11, 2026), nullptr) == KQ_OK, "hsd sau ngay nhap");
    xuatKho(kho, ma, 1, D(20, 11, 2026), nullptr);   // bo lai ve 60 ... (xuat 1 tu lo 1)

    // Xuat sai: khong thay doi gi
    int soPhieu = kho.phieu.count;
    KIEMTRA(xuatKho(kho, ma, 0, D(20, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "xuat 0");
    KIEMTRA(xuatKho(kho, ma, -1, D(20, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "xuat am");
    KIEMTRA(xuatKho(kho, ma, 1, D(32, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "ngay xuat sai");
    KIEMTRA(xuatKho(kho, "00000000", 1, D(20, 11, 2026), nullptr) == KQ_KHONG_TIM_THAY, "xuat ma khong co");
    KIEMTRA(xuatKho(kho, ma, 61, D(20, 11, 2026), nullptr) == KQ_KHONG_DU_HANG, "xuat vuot ton");
    KIEMTRA(mh->tonKho == 60 && lqTongSoLuong(mh->dsLo) == 60 && kho.phieu.count == soPhieu,
            "xuat that bai khong duoc thay doi ton/lo/phieu");

    // Lo hien tai: [9 (lo1), 20, 30, 1]. Xuat 15: het lo 1 (9), lay 6 tu lo 2
    KIEMTRA(xuatKho(kho, ma, 15, D(21, 11, 2026), &maPhieu) == KQ_OK, "xuat 15");
    KIEMTRA(mh->tonKho == 45 && mh->dsLo.count == 3, "ton 45, con 3 lo");
    KIEMTRA(mh->dsLo.front->maLo == 2 && mh->dsLo.front->soLuong == 14, "lo 2 con 14 (FIFO)");
    Phieu* px = dspTimTheoMa(kho.phieu, maPhieu);
    KIEMTRA(px && px->loai == PHIEU_XUAT && px->soLuong == 15 && px->donGia == 12000, "phieu xuat ghi don gia ban");

    // Xuat dung bang lo dau (14): lo 2 phai bi go ra
    KIEMTRA(xuatKho(kho, ma, 14, D(21, 11, 2026), nullptr) == KQ_OK && mh->dsLo.front->maLo == 3, "xuat vua het 1 lo");
    // Xuat het sach
    KIEMTRA(xuatKho(kho, ma, 31, D(22, 11, 2026), nullptr) == KQ_OK, "xuat het");
    KIEMTRA(mh->tonKho == 0 && lqRong(mh->dsLo) && mh->dsLo.rear == nullptr && mh->dsLo.count == 0, "kho rong");
    KIEMTRA(xuatKho(kho, ma, 1, D(22, 11, 2026), nullptr) == KQ_KHONG_DU_HANG, "xuat khi kho rong");
    // Nhap lai sau khi rong
    KIEMTRA(nhapKho(kho, ma, 7, 1, D(1, 3, 2027), D(22, 11, 2026), nullptr) == KQ_OK && mh->tonKho == 7, "nhap lai");

    // Chong tran so
    KIEMTRA(nhapKho(kho, ma, INT_MAX, 1, D(1, 3, 2027), D(22, 11, 2026), nullptr) == KQ_DU_LIEU_SAI, "tran int");
    KIEMTRA(mh->tonKho == 7, "khong doi ton khi tu choi tran so");

    std::string s = batOutput([&] { inChiTietMatHang(*mh); });
    KIEMTRA(s.find(ma) != std::string::npos && s.find("Nuoc ngot") != std::string::npos, "inChiTietMatHang");
    khoGiaiPhong(kho);
}

// ---------------------------------------------------------------------------
// Chay ngau nhien, so voi mo hinh FIFO tham chieu (mang tu cai)
struct MoHinh { bool coMat; int lo[64]; int dau, cuoi; };

static unsigned hat = 12345;
static unsigned ngauNhien() { hat ^= hat << 13; hat ^= hat >> 17; hat ^= hat << 5; return hat; }

static void testNgauNhien() {
    const int N = 2000, SO_THAO_TAC = 20000;
    std::printf("Inventory: %d thao tac ngau nhien tren %d ma (B-Tree m = 3)\n", SO_THAO_TAC, N);
    Kho kho;
    khoInit(kho, 3);
    MoHinh* mo = new MoHinh[N];
    char buf[16];
    for (int i = 0; i < N; ++i) mo[i].coMat = false;
    int phieuMongDoi = 0;

    for (int t = 0; t < SO_THAO_TAC; ++t) {
        int i = (int)(ngauNhien() % N);
        std::snprintf(buf, sizeof buf, "%08d", i * 37 + 10000000);
        std::string ma = buf;
        MoHinh& m = mo[i];
        int tong = 0;
        for (int k = m.dau; m.coMat && k < m.cuoi; ++k) tong += m.lo[k];
        int loai = (int)(ngauNhien() % 10);
        KetQua kq;

        if (!m.coMat || loai == 0) {   // them (hoac them trung)
            kq = themMatHang(kho, ma, "MH", "Nhom", 1000, 5);
            KIEMTRA(kq == (m.coMat ? KQ_TRUNG_MA : KQ_OK), "them %s", buf);
            if (!m.coMat) { m.coMat = true; m.dau = m.cuoi = 0; }
        } else if (loai <= 4) {          // nhap
            int sl = 1 + (int)(ngauNhien() % 50);
            if (m.cuoi == 64) continue;
            kq = nhapKho(kho, ma, sl, 500, D(1, 1, 2028), D(1, 11, 2026), nullptr);
            KIEMTRA(kq == KQ_OK, "nhap %s", buf);
            m.lo[m.cuoi++] = sl;
            ++phieuMongDoi;
        } else if (loai <= 8) {          // xuat
            int sl = 1 + (int)(ngauNhien() % 80);
            kq = xuatKho(kho, ma, sl, D(20, 11, 2026), nullptr);
            if (sl > tong) {
                KIEMTRA(kq == KQ_KHONG_DU_HANG, "xuat vuot ton %s", buf);
            } else {
                KIEMTRA(kq == KQ_OK, "xuat %s", buf);
                ++phieuMongDoi;
                int can = sl;
                while (can > 0) {
                    int lay = m.lo[m.dau] < can ? m.lo[m.dau] : can;
                    m.lo[m.dau] -= lay;
                    can -= lay;
                    if (m.lo[m.dau] == 0) ++m.dau;
                }
            }
        } else {                         // xoa
            bool keCaTon = ngauNhien() % 2;
            kq = xoaMatHang(kho, ma, keCaTon);
            if (tong > 0 && !keCaTon) KIEMTRA(kq == KQ_CON_TON_KHO, "xoa khi con ton %s", buf);
            else { KIEMTRA(kq == KQ_OK, "xoa %s", buf); m.coMat = false; }
        }

        // Doi chieu mat hang vua thao tac voi mo hinh
        MatHang* mh = timMatHang(kho, ma);
        if (!m.coMat) { KIEMTRA(mh == nullptr, "%s phai da bi xoa", buf); continue; }
        KIEMTRA(mh != nullptr, "%s phai con", buf);
        if (!mh) continue;
        int k = m.dau, tongMo = 0;
        bool khop = true;
        for (Lo* lo = mh->dsLo.front; lo; lo = lo->next, ++k)
            if (k >= m.cuoi || lo->soLuong != m.lo[k]) khop = false;
        for (int j = m.dau; j < m.cuoi; ++j) tongMo += m.lo[j];
        KIEMTRA(khop && k == m.cuoi && mh->dsLo.count == m.cuoi - m.dau, "danh sach lo %s lech mo hinh", buf);
        KIEMTRA(mh->tonKho == tongMo && lqTongSoLuong(mh->dsLo) == tongMo, "ton kho %s lech", buf);
    }
    KIEMTRA(kho.phieu.count == phieuMongDoi, "so phieu %d, mong doi %d", kho.phieu.count, phieuMongDoi);
    int conLai = 0;
    for (int i = 0; i < N; ++i) conLai += mo[i].coMat;
    KIEMTRA(kho.cay.size == conLai, "so mat hang trong cay %d, mong doi %d", kho.cay.size, conLai);
    std::printf("  con %d mat hang, %d phieu, cay cao %d\n", conLai, kho.phieu.count, btHeight(kho.cay));
    khoGiaiPhong(kho);   // ASan/LeakSanitizer se bao neu con ro ri
    delete[] mo;
}

int main() {
    testLotQueue();
    testPhieu();
    testMatHang();
    testNhapXuat();
    testNgauNhien();
    if (soLoi == 0) std::printf("\nTAT CA TEST KHO DEU QUA\n");
    else std::printf("\nCO %d LOI\n", soLoi);
    return soLoi == 0 ? 0 : 1;
}
