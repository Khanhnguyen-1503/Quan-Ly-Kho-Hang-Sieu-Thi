// tests/test_fileio.cpp - kiem thu Validate va FileIO (Huynh Tin)
// Bien dich: xem Makefile (make test_fileio). Chay tu thu muc goc cua repo (can data/).
// Tao thu muc tam tmp_test_fileio/ va xoa cac tep trong do khi xong.
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "BTree.h"
#include "DataGen.h"
#include "Date.h"
#include "FileIO.h"
#include "Inventory.h"
#include "LotQueue.h"
#include "Phieu.h"
#include "Validate.h"
#ifdef _WIN32
#include <direct.h>
static void taoThuMuc(const char* p) { _mkdir(p); }
#else
#include <sys/stat.h>
static void taoThuMuc(const char* p) { mkdir(p, 0755); }
#endif

static int soLoi = 0;
#define KIEMTRA(dk, ...)                                    \
    do {                                                    \
        if (!(dk)) {                                        \
            if (++soLoi <= 40) {                            \
                std::printf("  LOI (dong %d): ", __LINE__); \
                std::printf(__VA_ARGS__);                   \
                std::printf("\n");                          \
            }                                               \
        }                                                   \
    } while (0)

static const std::string TAM = "tmp_test_fileio";
static Date D(int d, int m, int y) { Date r = {d, m, y}; return r; }

static std::string docTep(const std::string& p) {
    std::ifstream f(p.c_str(), std::ios::binary);
    std::ostringstream os;
    os << f.rdbuf();
    return os.str();
}
static void ghiTep(const std::string& p, const std::string& noiDung) {
    std::ofstream f(p.c_str(), std::ios::binary);
    f << noiDung;
}
static void xoaTep3(const std::string& thuMuc) {
    const char* t[] = {"/mathang.txt", "/lo.txt", "/phieu.txt"};
    for (const char* x : t) {
        std::remove((thuMuc + x).c_str());
        std::remove((thuMuc + x + ".bak").c_str());
    }
}

// Chay f voi cin lay tu chuoi, cout/cerr bi nuot; tra ve noi dung cout
template <class F> static std::string voiDauVao(const std::string& vao, F f, std::string* cerrOut = nullptr) {
    std::istringstream is(vao);
    std::ostringstream os, es;
    std::streambuf* ci = std::cin.rdbuf(is.rdbuf());
    std::streambuf* co = std::cout.rdbuf(os.rdbuf());
    std::streambuf* ce = std::cerr.rdbuf(es.rdbuf());
    f();
    std::cin.rdbuf(ci);
    std::cout.rdbuf(co);
    std::cerr.rdbuf(ce);
    std::cin.clear();
    if (cerrOut) *cerrOut = es.str();
    return os.str();
}
static int dem(const std::string& s, const std::string& tu) {
    int n = 0;
    for (size_t p = s.find(tu); p != std::string::npos; p = s.find(tu, p + 1)) ++n;
    return n;
}

// ===========================================================================
static void testKiemTraChuoi() {
    std::printf("Validate: kiem tra chuoi\n");
    int i;
    double x;
    struct { const char* s; bool ok; int v; } nguyen[] = {
        {"0", true, 0}, {"42", true, 42}, {"  -7 ", true, -7}, {"+5", true, 5},
        {"2147483647", true, 2147483647}, {"-2147483648", true, -2147483647 - 1},
        {"2147483648", false, 0}, {"99999999999999", false, 0}, {"", false, 0}, {"  ", false, 0},
        {"-", false, 0}, {"12a", false, 0}, {"1.5", false, 0}, {"1 2", false, 0}, {"0x10", false, 0}};
    for (auto& t : nguyen) {
        bool ok = laSoNguyen(t.s, i);
        KIEMTRA(ok == t.ok && (!ok || i == t.v), "laSoNguyen(\"%s\")", t.s);
    }
    struct { const char* s; bool ok; double v; } thuc[] = {
        {"0", true, 0}, {"12000", true, 12000}, {"1.5", true, 1.5}, {" -2.25 ", true, -2.25},
        {"1e3", true, 1000}, {".5", true, 0.5}, {"", false, 0}, {"abc", false, 0}, {"1,5", false, 0},
        {"nan", false, 0}, {"inf", false, 0}, {"1e999", false, 0}, {"0x1p3", false, 0}, {"1.2.3", false, 0},
        {"1-2", false, 0}};
    for (auto& t : thuc) {
        bool ok = laSoThuc(t.s, x);
        KIEMTRA(ok == t.ok && (!ok || x == t.v), "laSoThuc(\"%s\")", t.s);
    }
    KIEMTRA(maVachHopLe("12345678") && maVachHopLe("8930000000001"), "ma vach hop le");
    const char* maSai[] = {"1234567", "123456789", "893000000000a", " 12345678", "", "89300000000011"};
    for (const char* m : maSai) KIEMTRA(!maVachHopLe(m), "ma vach \"%s\" phai sai", m);
    KIEMTRA(catKhoangTrang("  a b \t\r") == "a b" && catKhoangTrang("   ").empty(), "catKhoangTrang");
}

static void testNhapBanPhim() {
    std::printf("Validate: nhap tu ban phim (nhap sai thi hoi lai)\n");
    int v = 0;
    std::string ra = voiDauVao("abc\n500\n-1\n  42 \n", [&] { v = nhapSoNguyen("SL: ", 0, 100); });
    KIEMTRA(v == 42 && dem(ra, "-> Loi") == 3, "nhapSoNguyen hoi lai 3 lan (v = %d)", v);
    int cu = 7;
    voiDauVao("\n", [&] { v = nhapSoNguyen("SL: ", 0, 100, &cu); });
    KIEMTRA(v == 7, "Enter = gia tri mac dinh");

    double d = 0, dcu = 9.5;
    ra = voiDauVao("1,5\n-3\n12.5\n", [&] { d = nhapSoThuc("Gia: ", 0, 1e12); });
    KIEMTRA(d == 12.5 && dem(ra, "-> Loi") == 2, "nhapSoThuc");
    voiDauVao("\n", [&] { d = nhapSoThuc("Gia: ", 0, 1e12, &dcu); });
    KIEMTRA(d == 9.5, "nhapSoThuc mac dinh");

    std::string s, scu = "Cu";
    ra = voiDauVao("\n  \nA|B\n" + std::string(61, 'x') + "\n  Sua tuoi  \n", [&] { s = nhapChuoi("Ten: "); });
    KIEMTRA(s == "Sua tuoi" && dem(ra, "-> Loi") == 4, "nhapChuoi: rong, '|', qua dai (s = %s)", s.c_str());
    voiDauVao("\n", [&] { s = nhapChuoi("Ten: ", &scu); });
    KIEMTRA(s == "Cu", "nhapChuoi mac dinh");

    Date ng = D(1, 1, 2000), hn = D(26, 11, 2026);
    ra = voiDauVao("31/02/2026\n2026-01-01\n05/03/2027\n", [&] { ng = nhapNgay("Ngay: "); });
    KIEMTRA(ng.d == 5 && ng.m == 3 && ng.y == 2027 && dem(ra, "-> Loi") == 2, "nhapNgay");
    voiDauVao("\n", [&] { ng = nhapNgay("Ngay: ", &hn); });
    KIEMTRA(ng.d == 26 && ng.m == 11, "nhapNgay Enter = hom nay");

    ra = voiDauVao("123\nabcdefgh\n8930000000001\n", [&] { s = nhapMaVach("Ma: "); });
    KIEMTRA(s == "8930000000001" && dem(ra, "-> Loi") == 2, "nhapMaVach");

    bool b = false;
    ra = voiDauVao("x\nC\n", [&] { b = nhapCoKhong("? "); });
    KIEMTRA(b && dem(ra, "-> Loi") == 1, "nhapCoKhong c");
    voiDauVao("k\n", [&] { b = nhapCoKhong("? "); });
    KIEMTRA(!b, "nhapCoKhong k");

    // Het dau vao -> nem HetDauVao (khong lap vo han)
    bool nem = false;
    voiDauVao("abc\n", [&] {
        try { nhapSoNguyen("SL: ", 0, 10); } catch (const HetDauVao&) { nem = true; }
    });
    KIEMTRA(nem, "het dau vao phai nem HetDauVao");
}

// ===========================================================================
// So sanh hai kho: mat hang, lo, phieu, bo dem ke tiep
struct CtxSoSanh { Kho* khac; bool giong; };
static void soSanhMatHang(MatHang* a, void* p) {
    CtxSoSanh* c = static_cast<CtxSoSanh*>(p);
    MatHang* b = btSearch(c->khac->cay, a->maVach);
    if (!b || a->ten != b->ten || a->nhom != b->nhom || a->donGia != b->donGia || a->tonKho != b->tonKho ||
        a->nguongCanhBao != b->nguongCanhBao || a->dsLo.count != b->dsLo.count) {
        c->giong = false;
        return;
    }
    for (Lo *x = a->dsLo.front, *y = b->dsLo.front; x; x = x->next, y = y->next)
        if (!y || x->maLo != y->maLo || soSanhDate(x->ngayNhap, y->ngayNhap) != 0 ||
            soSanhDate(x->hsd, y->hsd) != 0 || x->soLuong != y->soLuong || x->giaNhap != y->giaNhap)
            c->giong = false;
}
static bool khoGiongNhau(Kho& a, Kho& b) {
    if (a.cay.size != b.cay.size || a.phieu.count != b.phieu.count || a.maLoKeTiep != b.maLoKeTiep ||
        a.phieu.maPhieuKeTiep != b.phieu.maPhieuKeTiep)
        return false;
    CtxSoSanh c{&b, true};
    btTraverse(a.cay, soSanhMatHang, &c);
    for (Phieu *x = a.phieu.head, *y = b.phieu.head; x; x = x->next, y = y->next)
        if (!y || x->maPhieu != y->maPhieu || x->loai != y->loai || soSanhDate(x->ngay, y->ngay) != 0 ||
            x->maVach != y->maVach || x->soLuong != y->soLuong || x->donGia != y->donGia)
            return false;
    return c.giong;
}

struct ThongKe { int soLo; int maLoMax; bool tonKhop; };
static void thongKe(MatHang* mh, void* p) {
    ThongKe* t = static_cast<ThongKe*>(p);
    t->soLo += mh->dsLo.count;
    if (mh->tonKho != lqTongSoLuong(mh->dsLo)) t->tonKhop = false;
    for (Lo* lo = mh->dsLo.front; lo; lo = lo->next)
        if (lo->maLo > t->maLoMax) t->maLoMax = lo->maLo;
}

static void testDocGhiDataMau() {
    std::printf("FileIO: doc data/, ghi lai, doc lai\n");
    Kho kho;
    khoInit(kho, 5);
    std::string loi;
    KetQua kq = KQ_OK;
    voiDauVao("", [&] { kq = docDuLieu(kho, "data"); }, &loi);
    KIEMTRA(kq == KQ_OK && loi.empty(), "doc data/ phai sach: %s", loi.c_str());
    ThongKe t{0, 0, true};
    btTraverse(kho.cay, thongKe, &t);
    int maPhieuMax = 0;
    for (Phieu* p = kho.phieu.head; p; p = p->next) maPhieuMax = p->maPhieu > maPhieuMax ? p->maPhieu : maPhieuMax;
    KIEMTRA(kho.cay.size == 35 && t.soLo == 65 && kho.phieu.count == 99, "so luong ban ghi (%d, %d, %d)",
            kho.cay.size, t.soLo, kho.phieu.count);
    KIEMTRA(t.tonKhop, "tonKho = tong cac lo");
    KIEMTRA(kho.maLoKeTiep == t.maLoMax + 1 && kho.phieu.maPhieuKeTiep == maPhieuMax + 1, "bo dem ke tiep");
    MatHang* mh = btSearch(kho.cay, "8930000000002");
    KIEMTRA(mh && mh->dsLo.front && mh->dsLo.front->maLo < mh->dsLo.rear->maLo, "lo dung thu tu FIFO");

    // Ghi ra thu muc tam: phai giong het tung byte
    KIEMTRA(ghiDuLieu(kho, TAM) == KQ_OK, "ghiDuLieu");
    const char* t3[] = {"/mathang.txt", "/lo.txt", "/phieu.txt"};
    for (const char* x : t3) KIEMTRA(docTep("data" + std::string(x)) == docTep(TAM + x), "%s khac ban goc", x);
    KIEMTRA(docTep(TAM + "/lo.txt.tmp").empty(), "khong con tep .tmp");

    // Thao tac roi luu, doc lai phai ra dung trang thai
    int mp;
    themMatHang(kho, "12345678", "Hang moi", "Do uong", 12345.5, 3);
    nhapKho(kho, "12345678", 10, 0.75, D(1, 1, 2027), D(26, 11, 2026), &mp);
    xuatKho(kho, "8930000000002", 30, D(26, 11, 2026), &mp);
    xoaMatHang(kho, "8930000000003", true);   // phieu cu cua ma nay van phai doc lai duoc
    KIEMTRA(ghiDuLieu(kho, TAM) == KQ_OK, "ghi sau thao tac");
    Kho kho2;
    khoInit(kho2, 3);   // bac khac, noi dung phai nhu nhau
    voiDauVao("", [&] { kq = docDuLieu(kho2, TAM); }, &loi);
    KIEMTRA(kq == KQ_OK && loi.empty(), "doc lai: %s", loi.c_str());
    KIEMTRA(khoGiongNhau(kho, kho2), "doc lai khac trang thai truoc khi ghi");
    MatHang* moi = btSearch(kho2.cay, "12345678");
    KIEMTRA(moi && moi->donGia == 12345.5 && moi->dsLo.front->giaNhap == 0.75, "so thap phan giu nguyen");
    // Ma lo / ma phieu moi sau khi doc lai khong duoc trung
    nhapKho(kho2, "12345678", 1, 1, D(1, 1, 2027), D(26, 11, 2026), &mp);
    KIEMTRA(moi->dsLo.rear->maLo == kho.maLoKeTiep && mp == kho.phieu.maPhieuKeTiep, "ma moi tiep noi sau khi doc");
    khoGiaiPhong(kho);
    khoGiaiPhong(kho2);
    xoaTep3(TAM);
}

static void testTepLoi() {
    std::printf("FileIO: tep co dong loi\n");
    ghiTep(TAM + "/mathang.txt",
           "11111111|Sua tuoi|Thuc pham tuoi|30000|10\r\n"   // CRLF
           "\n"                                              // dong trong
           "22222222|Thieu truong|Do uong|5000\n"            // 4 truong
           "1234567|Ma 7 so|Do uong|5000|1\n"                // ma sai
           "11111111|Trung ma|Do uong|5000|1\n"              // trung
           "33333333|Gia am|Do uong|-1|1\n"
           "44444444|Nguong chu|Do uong|5000|abc\n"
           "55555555||Do uong|5000|1\n"                      // ten rong
           "66666666|Thua|truong|1|1|x\n"                    // 6 truong
           "77777777|Keo|Banh keo|2500.5|0\n");
    ghiTep(TAM + "/lo.txt",
           "11111111|LO00001|01/11/2026|01/12/2026|5|20000\n"
           "99999999|LO00002|01/11/2026|01/12/2026|5|20000\n"   // ma khong co
           "11111111|L00003|01/11/2026|01/12/2026|5|20000\n"    // ma lo sai
           "11111111|LO00004|31/11/2026|01/12/2026|5|20000\n"   // ngay sai
           "11111111|LO00005|10/11/2026|01/11/2026|5|20000\n"   // hsd < ngay nhap
           "11111111|LO00006|10/11/2026|01/12/2026|0|20000\n"   // so luong 0
           "11111111|LO00007|05/11/2026|01/12/2026|7|21000\n"
           "11111111|LO00008|01/11/2026|01/12/2026|5|20000\n"   // nhap truoc lo truoc (sai FIFO)
           "77777777|LO00020|01/11/2026|01/01/2027|3|2000\n");
    ghiTep(TAM + "/phieu.txt",
           "PN00001|NHAP|01/11/2026|11111111|5|20000\n"
           "PX00002|NHAP|01/11/2026|11111111|5|20000\n"   // tien to khong khop
           "PN00003|NHAP|05/11/2026|11111111|7|21000\n"
           "PN00003|NHAP|05/11/2026|11111111|7|21000\n"   // trung ma
           "PX00004|BAN|05/11/2026|11111111|1|30000\n"    // loai sai
           "PX00005|XUAT|32/11/2026|11111111|1|30000\n"   // ngay sai
           "PX00006|XUAT|06/11/2026|88888888|2|9000\n"    // ma da xoa khoi kho: van nhan
           "PN00020|NHAP|01/11/2026|77777777|3|2000\n");
    Kho kho;
    khoInit(kho, 3);
    std::string loi;
    KetQua kq = KQ_OK;
    voiDauVao("", [&] { kq = docDuLieu(kho, TAM); }, &loi);
    KIEMTRA(kq == KQ_DU_LIEU_SAI, "co dong loi -> KQ_DU_LIEU_SAI");
    KIEMTRA(dem(loi, "mathang.txt dong") == 7, "7 dong mathang loi (bao %d)", dem(loi, "mathang.txt dong"));
    KIEMTRA(dem(loi, "lo.txt dong") == 6, "6 dong lo loi (bao %d)", dem(loi, "lo.txt dong"));
    KIEMTRA(dem(loi, "phieu.txt dong") == 4, "4 dong phieu loi (bao %d)", dem(loi, "phieu.txt dong"));
    KIEMTRA(loi.find("dong 3]") != std::string::npos, "bao dung so dong");
    KIEMTRA(kho.cay.size == 2, "con 2 mat hang hop le (%d)", kho.cay.size);
    MatHang* a = btSearch(kho.cay, "11111111");
    KIEMTRA(a && a->tonKho == 12 && a->dsLo.count == 2, "ma 1111: 2 lo hop le, ton 12");
    KIEMTRA(kho.maLoKeTiep == 21, "maLoKeTiep = 21 (%d)", kho.maLoKeTiep);
    KIEMTRA(kho.phieu.count == 4 && kho.phieu.maPhieuKeTiep == 21, "4 phieu hop le, ke tiep 21");
    KIEMTRA(dspTimTheoMa(kho.phieu, 6) != nullptr, "phieu cua mat hang da xoa van duoc doc");
    khoGiaiPhong(kho);

    // Qua nhieu dong loi: chi in 10 dong dau
    std::string nhieu;
    for (int i = 0; i < 25; ++i) nhieu += "sai\n";
    ghiTep(TAM + "/mathang.txt", nhieu);
    khoInit(kho, 3);
    voiDauVao("", [&] { kq = docDuLieu(kho, TAM); }, &loi);
    KIEMTRA(dem(loi, "mathang.txt dong") == 10 && loi.find("va 15 dong loi khac") != std::string::npos,
            "gioi han 10 thong bao");
    khoGiaiPhong(kho);
    xoaTep3(TAM);

    // Thieu tep
    khoInit(kho, 3);
    voiDauVao("", [&] { kq = docDuLieu(kho, "khong_co_thu_muc_nay"); }, &loi);
    KIEMTRA(kq == KQ_LOI_TEP && kho.cay.size == 0, "thieu mathang.txt -> KQ_LOI_TEP");
    ghiTep(TAM + "/mathang.txt", "11111111|A|B|1|1\n");
    voiDauVao("", [&] { kq = docDuLieu(kho, TAM); }, &loi);
    KIEMTRA(kq == KQ_DU_LIEU_SAI && kho.cay.size == 1 && loi.find("coi nhu rong") != std::string::npos,
            "thieu lo.txt/phieu.txt -> van doc mat hang");
    KIEMTRA(ghiDuLieu(kho, "khong_co_thu_muc_nay") == KQ_LOI_TEP, "ghi vao thu muc khong ton tai");
    khoGiaiPhong(kho);
    xoaTep3(TAM);
}

static void testSaoLuu() {
    std::printf("FileIO: sao luu .bak khi ghi de\n");
    // Dong 2 cua mathang hong; lo cua mat hang do cung bi bo qua khi doc
    const std::string mh = "11111111|Sua|Do uong|1000|1\n22222222|Hong|Do uong|1O00|1\n";
    const std::string lo = "11111111|LO00001|01/11/2026|01/12/2026|5|500\n"
                           "22222222|LO00002|01/11/2026|01/12/2026|7|700\n";
    const std::string ph = "PN00001|NHAP|01/11/2026|11111111|5|500\nPN00002|NHAP|01/11/2026|22222222|7|700\n";
    ghiTep(TAM + "/mathang.txt", mh);
    ghiTep(TAM + "/lo.txt", lo);
    ghiTep(TAM + "/phieu.txt", ph);
    Kho kho;
    khoInit(kho, 3);
    KetQua kq = KQ_OK;
    voiDauVao("", [&] { kq = docDuLieu(kho, TAM); });
    KIEMTRA(kq == KQ_DU_LIEU_SAI && kho.cay.size == 1, "doc: bo qua dong hong va lo cua no");

    // Lan luu dau (saoLuu): tep cu thanh .bak, nguyen ven ca dong hong va lo cua no
    KIEMTRA(ghiDuLieu(kho, TAM, true) == KQ_OK, "ghi co sao luu");
    KIEMTRA(docTep(TAM + "/mathang.txt.bak") == mh && docTep(TAM + "/lo.txt.bak") == lo &&
            docTep(TAM + "/phieu.txt.bak") == ph, ".bak phai giong het tep luc doc");
    KIEMTRA(docTep(TAM + "/mathang.txt").find("Hong") == std::string::npos &&
            docTep(TAM + "/lo.txt").find("LO00002") == std::string::npos, "tep moi khong con dong hong");

    // Lan luu sau trong cung phien (khong sao luu): .bak giu nguyen
    themMatHang(kho, "33333333", "Moi", "Do uong", 1, 1);
    KIEMTRA(ghiDuLieu(kho, TAM, false) == KQ_OK, "ghi lan 2");
    KIEMTRA(docTep(TAM + "/mathang.txt.bak") == mh, "lan luu sau khong duoc ghi de .bak");
    KIEMTRA(docTep(TAM + "/mathang.txt").find("33333333") != std::string::npos, "lan 2 van ghi du lieu moi");
    khoGiaiPhong(kho);

    // saoLuuDuLieu (truoc khi sinh du lieu moi): doi ten tep hien co thanh .bak
    std::string truoc = docTep(TAM + "/mathang.txt");
    KIEMTRA(saoLuuDuLieu(TAM) == KQ_OK, "saoLuuDuLieu");
    KIEMTRA(docTep(TAM + "/mathang.txt.bak") == truoc && docTep(TAM + "/mathang.txt").empty(),
            "saoLuuDuLieu doi ten tep thanh .bak");
    // Chua co tep nao: sao luu va ghi co sao luu deu khong loi
    xoaTep3(TAM);
    KIEMTRA(saoLuuDuLieu(TAM) == KQ_OK, "sao luu khi chua co tep");
    khoInit(kho, 3);
    KIEMTRA(ghiDuLieu(kho, TAM, true) == KQ_OK && docTep(TAM + "/mathang.txt.bak").empty(),
            "ghi co sao luu khi chua co tep cu");
    khoGiaiPhong(kho);
    xoaTep3(TAM);
}

static void testDuLieuLon() {
    const int N = 10000;
    std::printf("FileIO: %d mat hang sinh boi DataGen\n", N);
    sinhDuLieuMau(N, TAM + "/mathang.txt", TAM + "/lo.txt", TAM + "/phieu.txt");
    std::string goc[3] = {docTep(TAM + "/mathang.txt"), docTep(TAM + "/lo.txt"), docTep(TAM + "/phieu.txt")};
    Kho kho;
    khoInit(kho, 5);
    auto bd = std::chrono::steady_clock::now();
    std::string loi;
    KetQua kq = KQ_OK;
    voiDauVao("", [&] { kq = docDuLieu(kho, TAM); }, &loi);
    double msDoc = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - bd).count();
    KIEMTRA(kq == KQ_OK && loi.empty() && kho.cay.size == N, "doc %d mat hang", N);
    bd = std::chrono::steady_clock::now();
    KIEMTRA(ghiDuLieu(kho, TAM) == KQ_OK, "ghi lai");
    double msGhi = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - bd).count();
    KIEMTRA(docTep(TAM + "/mathang.txt") == goc[0] && docTep(TAM + "/lo.txt") == goc[1] &&
            docTep(TAM + "/phieu.txt") == goc[2], "doc roi ghi lai phai giong het");
    std::printf("  doc %.0f ms, ghi %.0f ms (%d phieu)\n", msDoc, msGhi, kho.phieu.count);
    khoGiaiPhong(kho);
    xoaTep3(TAM);
}

int main() {
    taoThuMuc(TAM.c_str());
    testKiemTraChuoi();
    testNhapBanPhim();
    testDocGhiDataMau();
    testTepLoi();
    testSaoLuu();
    testDuLieuLon();
    std::remove(TAM.c_str());
    if (soLoi == 0) std::printf("\nTAT CA TEST FILEIO DEU QUA\n");
    else std::printf("\nCO %d LOI\n", soLoi);
    return soLoi == 0 ? 0 : 1;
}
