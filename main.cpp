// main.cpp - Huynh Tin
// Menu chuong trinh quan ly kho hang sieu thi.
// Chay: ./quanlykho [thuMucDuLieu] [bacBTree]      mac dinh: data 5
// Doc 3 tep khi khoi dong, ghi lai khi thoat (muc 0) hoac khi chon "Luu" (muc 18).
#include <iostream>
#include <string>
#include "BTree.h"
#include "Benchmark.h"
#include "DataGen.h"
#include "Date.h"
#include "FileIO.h"
#include "Inventory.h"
#include "Phieu.h"
#include "Report.h"
#include "Sort.h"
#include "Validate.h"

namespace {

Kho g_kho;
std::string g_thuMuc = "data";
int g_bac = 5;
bool g_chuaLuu = false;

const char* moTa(KetQua kq) {
    switch (kq) {
        case KQ_OK: return "Thanh cong";
        case KQ_TRUNG_MA: return "Ma vach da ton tai";
        case KQ_KHONG_TIM_THAY: return "Khong tim thay mat hang";
        case KQ_KHONG_DU_HANG: return "Khong du hang trong kho";
        case KQ_DU_LIEU_SAI: return "Du lieu khong hop le";
        case KQ_CON_TON_KHO: return "Mat hang van con ton kho";
        case KQ_LOI_TEP: return "Loi doc/ghi tep";
    }
    return "?";
}

void demLo(MatHang* mh, void* p) { *static_cast<int*>(p) += mh->dsLo.count; }

int soLo() {
    int n = 0;
    btTraverse(g_kho.cay, demLo, &n);
    return n;
}

std::string tep(const char* ten) { return g_thuMuc + "/" + ten; }

// ---- Nap / luu ----------------------------------------------------------------

void napDuLieu() {
    bool inCay = g_kho.cay.inKhiTachGop;
    khoInit(g_kho, g_bac);
    std::cout << "Dang doc du lieu tu " << g_thuMuc << "/ ...\n";
    KetQua kq = docDuLieu(g_kho, g_thuMuc);
    if (kq == KQ_LOI_TEP)
        std::cout << "Khong mo duoc " << tep("mathang.txt") << ". Bat dau voi kho rong.\n";
    else if (kq == KQ_DU_LIEU_SAI)
        std::cout << "Mot so dong loi da bi bo qua (xem thong bao o tren).\n";
    std::cout << "Da nap " << g_kho.cay.size << " mat hang, " << soLo() << " lo, " << g_kho.phieu.count
              << " phieu.\n";
    g_kho.cay.inKhiTachGop = inCay;   // bat sau khi nap de khong in hang tram lan tach
    g_chuaLuu = false;
}

bool luu() {
    KetQua kq = ghiDuLieu(g_kho, g_thuMuc);
    if (kq == KQ_OK) {
        std::cout << "Da luu vao " << g_thuMuc << "/ (mathang.txt, lo.txt, phieu.txt).\n";
        g_chuaLuu = false;
        return true;
    }
    std::cout << "LOI: khong ghi duoc du lieu vao " << g_thuMuc << "/ (thu muc co ton tai?).\n";
    return false;
}

// Tim mat hang theo ma vach nhap tu ban phim; in thong bao neu khong co
MatHang* hoiMatHang() {
    std::string ma = nhapMaVach("Ma vach: ");
    MatHang* mh = timMatHang(g_kho, ma);
    if (!mh) std::cout << "Khong co mat hang " << ma << " trong kho.\n";
    return mh;
}

// ---- 1-6: mat hang ------------------------------------------------------------

void themMH() {
    std::string ma = nhapMaVach("Ma vach (8 hoac 13 chu so): ");
    if (timMatHang(g_kho, ma)) {
        std::cout << "Ma vach " << ma << " da ton tai.\n";
        return;
    }
    std::string ten = nhapChuoi("Ten mat hang: ");
    std::string nhom = nhapChuoi("Nhom hang: ");
    double gia = nhapSoThuc("Don gia ban: ", 0, 1e12);
    int nguong = nhapSoNguyen("Nguong canh bao ton kho: ", 0, 1000000);
    KetQua kq = themMatHang(g_kho, ma, ten, nhom, gia, nguong);
    std::cout << moTa(kq) << ".\n";
    if (kq == KQ_OK) g_chuaLuu = true;
}

void suaMH() {
    MatHang* mh = hoiMatHang();
    if (!mh) return;
    inChiTietMatHang(*mh);
    std::cout << "(Bam Enter de giu nguyen gia tri cu)\n";
    std::string ten = nhapChuoi("Ten [" + mh->ten + "]: ", &mh->ten);
    std::string nhom = nhapChuoi("Nhom [" + mh->nhom + "]: ", &mh->nhom);
    double gia = nhapSoThuc("Don gia ban [" + std::to_string((long long)mh->donGia) + "]: ", 0, 1e12, &mh->donGia);
    int nguong = nhapSoNguyen("Nguong canh bao [" + std::to_string(mh->nguongCanhBao) + "]: ", 0, 1000000,
                              &mh->nguongCanhBao);
    KetQua kq = suaMatHang(g_kho, mh->maVach, ten, nhom, gia, nguong);
    std::cout << moTa(kq) << ".\n";
    if (kq == KQ_OK) g_chuaLuu = true;
}

void xoaMH() {
    MatHang* mh = hoiMatHang();
    if (!mh) return;
    inChiTietMatHang(*mh);
    if (!nhapCoKhong("Xac nhan xoa mat hang nay? (c/k): ")) return;
    std::string ma = mh->maVach;   // mh se bi delete khi xoa
    KetQua kq = xoaMatHang(g_kho, ma, false);
    if (kq == KQ_CON_TON_KHO) {
        std::cout << "Mat hang van con " << mh->tonKho << " don vi trong kho.\n";
        if (!nhapCoKhong("Xoa luon ca so ton nay? (c/k): ")) return;
        kq = xoaMatHang(g_kho, ma, true);
    }
    std::cout << moTa(kq) << ".\n";
    if (kq == KQ_OK) g_chuaLuu = true;
}

void xemChiTiet() {
    std::string ma = nhapMaVach("Ma vach: ");
    long long truoc = g_kho.cay.soNutTruyXuat;
    MatHang* mh = timMatHang(g_kho, ma);
    long long soNut = g_kho.cay.soNutTruyXuat - truoc;
    if (mh) inChiTietMatHang(*mh);
    else std::cout << "Khong co mat hang " << ma << " trong kho.\n";
    std::cout << "(B-Tree: tim qua " << soNut << " nut, cay cao " << btHeight(g_kho.cay) << ")\n";
}

void timTen() {
    std::string tk = docDong("Tu khoa trong ten (Enter = tat ca): ");
    DsMatHang ds;
    dsInit(ds);
    timTheoTen(g_kho, tk, ds);
    inBangMatHang(ds);
    dsClear(ds);
}

void lietKe() {
    std::string nhom = docDong("Nhom hang (Enter = tat ca, sap theo nhom): ");
    DsMatHang ds;
    dsInit(ds);
    lietKeTheoNhom(g_kho, nhom, ds);
    inBangMatHang(ds);
    dsClear(ds);
}

// ---- 7-9: nhap / xuat / phieu ----------------------------------------------------

void nhapKhoMenu() {
    MatHang* mh = hoiMatHang();
    if (!mh) {
        std::cout << "Hay them mat hang truoc (muc 1).\n";
        return;
    }
    std::cout << mh->ten << " - ton hien tai: " << mh->tonKho << '\n';
    int sl = nhapSoNguyen("So luong nhap: ", 1, 1000000);
    double gia = nhapSoThuc("Gia nhap: ", 0, 1e12);
    Date hn = homNay();
    Date ngay = nhapNgay("Ngay nhap dd/mm/yyyy (Enter = " + formatDate(hn) + "): ", &hn);
    Date hsd;
    while (true) {
        hsd = nhapNgay("Han su dung dd/mm/yyyy: ");
        if (soSanhDate(hsd, ngay) >= 0) break;
        std::cout << "  -> Loi: HSD khong duoc truoc ngay nhap " << formatDate(ngay) << ". Nhap lai.\n";
    }
    int maPhieu = 0;
    KetQua kq = nhapKho(g_kho, mh->maVach, sl, gia, hsd, ngay, &maPhieu);
    if (kq != KQ_OK) {
        std::cout << moTa(kq) << ".\n";
        return;
    }
    g_chuaLuu = true;
    std::cout << "Da tao phieu nhap " << taoMaPhieu(maPhieu, PHIEU_NHAP) << ", lo " << taoMaLo(g_kho.maLoKeTiep - 1)
              << ". Ton moi: " << mh->tonKho << '\n';
}

void xuatKhoMenu() {
    MatHang* mh = hoiMatHang();
    if (!mh) return;
    inChiTietMatHang(*mh);
    if (mh->tonKho == 0) {
        std::cout << "Mat hang da het hang.\n";
        return;
    }
    int sl = nhapSoNguyen("So luong xuat (1 - " + std::to_string(mh->tonKho) + "): ", 1, mh->tonKho);
    Date hn = homNay();
    Date ngay = nhapNgay("Ngay xuat dd/mm/yyyy (Enter = " + formatDate(hn) + "): ", &hn);
    int maPhieu = 0;
    KetQua kq = xuatKho(g_kho, mh->maVach, sl, ngay, &maPhieu);
    if (kq != KQ_OK) {
        std::cout << moTa(kq) << ".\n";
        return;
    }
    g_chuaLuu = true;
    std::cout << "Da tao phieu xuat " << taoMaPhieu(maPhieu, PHIEU_XUAT) << " (lay tu lo cu nhat truoc).";
    inChiTietMatHang(*mh);
}

void danhSachPhieu() {
    if (nhapCoKhong("Loc theo khoang ngay? (c/k): ")) {
        Date tu = nhapNgay("Tu ngay: ");
        Date den = nhapNgay("Den ngay: ");
        dspIn(g_kho.phieu, &tu, &den);
    } else {
        dspIn(g_kho.phieu, nullptr, nullptr);
    }
    std::cout << "Tong so phieu trong kho: " << g_kho.phieu.count << '\n';
}

// ---- 10-13: bao cao -----------------------------------------------------------

void canhBaoTon() {
    DsMatHang ds;
    dsInit(ds);
    canhBaoSapHetHang(g_kho, ds);
    std::cout << "Mat hang co ton kho DUOI nguong canh bao:\n";
    inBangMatHang(ds);
    dsClear(ds);
}

void canhBaoHSD() {
    Date hn = homNay();
    CanhBaoHSD* a = nullptr;
    int n = canhBaoSapHetHan(g_kho, hn, 7, a);
    std::cout << "Lo con hang da het han hoac het han trong 7 ngay (tinh tu " << formatDate(hn) << "):\n";
    inBangCanhBaoHSD(a, n);
    delete[] a;
}

void giaTriNhom() {
    GiaTriNhom* a = nullptr;
    int n = baoCaoGiaTriTheoNhom(g_kho, a);
    std::cout << "Gia tri ton kho theo nhom (tinh theo gia nhap tung lo):\n";
    inBangGiaTriNhom(a, n);
    delete[] a;
}

void top10() {
    Date hn = homNay();
    int thang = nhapSoNguyen("Thang (Enter = " + std::to_string(hn.m) + "): ", 1, 12, &hn.m);
    int nam = nhapSoNguyen("Nam (Enter = " + std::to_string(hn.y) + "): ", 1900, 2100, &hn.y);
    BanChay* a = nullptr;
    int n = topBanChay(g_kho, thang, nam, 10, a);
    std::cout << "Top 10 ban chay thang " << thang << "/" << nam << " (theo phieu xuat):\n";
    inBangBanChay(a, n);
    delete[] a;
}

// ---- 14-16: B-Tree ----------------------------------------------------------------

void xemCay() {
    std::cout << "B-Tree bac m = " << g_kho.cay.m << ": " << g_kho.cay.size << " khoa, " << btSoNut(g_kho.cay)
              << " nut, chieu cao " << btHeight(g_kho.cay) << '\n';
    if (g_kho.cay.size <= 40 || nhapCoKhong("Cay co " + std::to_string(g_kho.cay.size) +
                                            " khoa, in ra se rat dai. Van in? (c/k): "))
        btPrint(g_kho.cay);
}

void batTatInCay() {
    g_kho.cay.inKhiTachGop = !g_kho.cay.inKhiTachGop;
    std::cout << "In cay sau moi lan tach/muon/gop nut: " << (g_kho.cay.inKhiTachGop ? "BAT" : "TAT") << '\n';
    if (g_kho.cay.inKhiTachGop)
        std::cout << "(Thu them/xoa mat hang de xem. Nen dung voi kho nho, hoac chay: ./quanlykho <thu muc> 3)\n";
}

void thucNghiem() {
    if (!nhapCoKhong("Chay thuc nghiem n = 10^3..10^5, m = 3, 5, 10 (mat vai giay)? (c/k): ")) return;
    chayThucNghiem("ketqua/ketqua_btree.csv");
}

// ---- 17-18: du lieu --------------------------------------------------------------

void sinhDuLieu() {
    std::cout << "Se GHI DE mathang.txt, lo.txt, phieu.txt trong " << g_thuMuc
              << "/ roi nap lai kho. Thay doi chua luu se mat.\n";
    if (!nhapCoKhong("Tiep tuc? (c/k): ")) return;
    int n = nhapSoNguyen("So mat hang (30 - 100000): ", 30, 100000);
    sinhDuLieuMau(n, tep("mathang.txt"), tep("lo.txt"), tep("phieu.txt"));
    khoGiaiPhong(g_kho);
    napDuLieu();
}

void inMenu() {
    std::cout << "\n==================== QUAN LY KHO HANG SIEU THI ====================\n"
              << " Kho: " << g_kho.cay.size << " mat hang, " << soLo() << " lo, " << g_kho.phieu.count
              << " phieu | B-Tree m = " << g_kho.cay.m << ", cao " << btHeight(g_kho.cay) << " | " << g_thuMuc
              << "/" << (g_chuaLuu ? " (chua luu)" : "") << '\n'
              << " MAT HANG   1. Them   2. Sua   3. Xoa   4. Xem chi tiet theo ma vach\n"
              << "            5. Tim theo ten   6. Liet ke theo nhom\n"
              << " NHAP/XUAT  7. Nhap kho   8. Xuat kho (FIFO)   9. Danh sach phieu\n"
              << " BAO CAO    10. Ton duoi nguong   11. Sap het han (7 ngay)\n"
              << "            12. Gia tri ton theo nhom   13. Top 10 ban chay trong thang\n"
              << " B-TREE     14. Xem cay   15. Bat/tat in cay khi tach/gop   16. Thuc nghiem m = 3, 5, 10\n"
              << " DU LIEU    17. Sinh du lieu mau   18. Luu   0. Luu va thoat\n"
              << "===================================================================\n";
}

void xuLy(int chon) {
    switch (chon) {
        case 1: themMH(); break;
        case 2: suaMH(); break;
        case 3: xoaMH(); break;
        case 4: xemChiTiet(); break;
        case 5: timTen(); break;
        case 6: lietKe(); break;
        case 7: nhapKhoMenu(); break;
        case 8: xuatKhoMenu(); break;
        case 9: danhSachPhieu(); break;
        case 10: canhBaoTon(); break;
        case 11: canhBaoHSD(); break;
        case 12: giaTriNhom(); break;
        case 13: top10(); break;
        case 14: xemCay(); break;
        case 15: batTatInCay(); break;
        case 16: thucNghiem(); break;
        case 17: sinhDuLieu(); break;
        case 18: luu(); break;
    }
}

}  // namespace

int main(int argc, char** argv) {
    if (argc > 1) g_thuMuc = argv[1];
    if (argc > 2) {
        int m;
        if (laSoNguyen(argv[2], m) && m >= 3 && m <= 100) g_bac = m;
        else std::cout << "Bac B-Tree phai trong [3, 100], dung m = " << g_bac << '\n';
    }
    napDuLieu();
    try {
        while (true) {
            inMenu();
            int chon = nhapSoNguyen("Chon: ", 0, 18);
            if (chon == 0) {
                if (luu() || nhapCoKhong("Luu that bai. Van thoat (mat thay doi)? (c/k): ")) break;
                continue;
            }
            xuLy(chon);
        }
    } catch (const HetDauVao&) {
        std::cout << "\n(Het du lieu nhap) Luu va thoat.\n";
        luu();
    }
    khoGiaiPhong(g_kho);
    return 0;
}
