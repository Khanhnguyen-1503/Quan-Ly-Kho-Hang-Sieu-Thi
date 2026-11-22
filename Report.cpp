// Report.cpp - Long Vu
#include "Report.h"
#include <iomanip>
#include <iostream>
#include "BTree.h"
#include "Date.h"
#include "Sort.h"

namespace {

std::string chuThuong(const std::string& s) {
    std::string r = s;
    for (char& c : r)
        if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
    return r;
}

// Gia tri ton cua mot mat hang theo gia nhap tung lo
double giaTriTon(const MatHang* mh) {
    double s = 0;
    for (const Lo* lo = mh->dsLo.front; lo; lo = lo->next) s += lo->soLuong * lo->giaNhap;
    return s;
}

// ---- Ham goi lai cho btTraverse ------------------------------------------

struct CtxTen { std::string tuKhoa; DsMatHang* kq; };
void gomTheoTen(MatHang* mh, void* p) {
    CtxTen* c = static_cast<CtxTen*>(p);
    if (chuThuong(mh->ten).find(c->tuKhoa) != std::string::npos) dsThem(*c->kq, mh);
}

struct CtxNhom { const std::string* nhom; DsMatHang* kq; };
void gomTheoNhom(MatHang* mh, void* p) {
    CtxNhom* c = static_cast<CtxNhom*>(p);
    if (c->nhom->empty() || soSanhKhongPhanBiet(mh->nhom, *c->nhom) == 0) dsThem(*c->kq, mh);
}

void gomSapHetHang(MatHang* mh, void* p) {
    if (mh->tonKho < mh->nguongCanhBao) dsThem(*static_cast<DsMatHang*>(p), mh);
}

void gomTatCa(MatHang* mh, void* p) { dsThem(*static_cast<DsMatHang*>(p), mh); }

// Lan 1 (a == nullptr): chi dem. Lan 2: ghi vao a.
struct CtxHSD { Date homNay; int soNgay; bool kemDaHetHan; CanhBaoHSD* a; int n; };
void gomHSD(MatHang* mh, void* p) {
    CtxHSD* c = static_cast<CtxHSD*>(p);
    for (Lo* lo = mh->dsLo.front; lo; lo = lo->next) {
        if (lo->soLuong <= 0) continue;
        int con = (int)khoangCachNgay(lo->hsd, c->homNay);
        if (con > c->soNgay || (con < 0 && !c->kemDaHetHan)) continue;
        if (c->a) c->a[c->n] = CanhBaoHSD{mh, lo, con};
        c->n++;
    }
}

void datDemTamVe0(MatHang* mh, void*) { mh->demTam = 0; }

struct CtxBan { BanChay* a; int n; };
void gomBanChay(MatHang* mh, void* p) {
    CtxBan* c = static_cast<CtxBan*>(p);
    if (mh->demTam <= 0) return;
    if (c->a) c->a[c->n] = BanChay{mh, mh->demTam};
    c->n++;
}

// Giu nguyen dinh dang cout sau khi in bang
struct GiuDinhDang {
    std::ios cu;
    GiuDinhDang() : cu(nullptr) { cu.copyfmt(std::cout); }
    ~GiuDinhDang() { std::cout.copyfmt(cu); }
};

void ke(int dai) { std::cout << std::string(dai, '-') << '\n'; }

}  // namespace

// ---------------------------------------------------------------------------

void timTheoTen(const Kho& kho, const std::string& tuKhoa, DsMatHang& kq) {
    CtxTen c{chuThuong(tuKhoa), &kq};
    btTraverse(kho.cay, gomTheoTen, &c);
    sapXepTheoTen(kq);
}

void lietKeTheoNhom(const Kho& kho, const std::string& nhom, DsMatHang& kq) {
    CtxNhom c{&nhom, &kq};
    btTraverse(kho.cay, gomTheoNhom, &c);
    if (nhom.empty()) sapXepTheoNhom(kq);
    else sapXepTheoTen(kq);
}

void canhBaoSapHetHang(const Kho& kho, DsMatHang& kq) {
    btTraverse(kho.cay, gomSapHetHang, &kq);
    sapXepTheoTonKho(kq);
}

int canhBaoSapHetHan(const Kho& kho, const Date& homNay, int soNgay, CanhBaoHSD*& kq,
                     bool kemDaHetHan) {
    kq = nullptr;
    CtxHSD c{homNay, soNgay, kemDaHetHan, nullptr, 0};
    btTraverse(kho.cay, gomHSD, &c);   // dem
    if (c.n == 0) return 0;
    kq = new CanhBaoHSD[c.n];
    c.a = kq;
    c.n = 0;
    btTraverse(kho.cay, gomHSD, &c);   // ghi
    sapXepTheoHSD(kq, c.n);
    return c.n;
}

int baoCaoGiaTriTheoNhom(const Kho& kho, GiaTriNhom*& kq) {
    kq = nullptr;
    DsMatHang ds;
    dsInit(ds);
    btTraverse(kho.cay, gomTatCa, &ds);
    sapXepTheoNhom(ds);   // cac mat hang cung nhom nam lien nhau

    int soNhom = 0;
    for (int i = 0; i < ds.n; ++i)
        if (i == 0 || soSanhKhongPhanBiet(ds.a[i]->nhom, ds.a[i - 1]->nhom) != 0) ++soNhom;

    if (soNhom > 0) {
        kq = new GiaTriNhom[soNhom];
        int g = -1;
        for (int i = 0; i < ds.n; ++i) {
            MatHang* mh = ds.a[i];
            if (i == 0 || soSanhKhongPhanBiet(mh->nhom, ds.a[i - 1]->nhom) != 0) {
                ++g;
                kq[g].nhom = mh->nhom;
                kq[g].soMatHang = 0;
                kq[g].tongTon = 0;
                kq[g].giaTriTon = 0;
            }
            kq[g].soMatHang++;
            kq[g].tongTon += mh->tonKho;
            kq[g].giaTriTon += giaTriTon(mh);
        }
        sapXepGiaTriNhom(kq, soNhom);
    }
    dsClear(ds);
    return soNhom;
}

int topBanChay(Kho& kho, int thang, int nam, int k, BanChay*& kq) {
    kq = nullptr;
    if (k <= 0) return 0;
    btTraverse(kho.cay, datDemTamVe0, nullptr);
    for (Phieu* p = kho.phieu.head; p; p = p->next) {
        if (p->loai != PHIEU_XUAT || p->ngay.m != thang || p->ngay.y != nam) continue;
        MatHang* mh = btSearch(kho.cay, p->maVach);
        if (mh) mh->demTam += p->soLuong;   // mat hang da xoa thi bo qua
    }
    CtxBan c{nullptr, 0};
    btTraverse(kho.cay, gomBanChay, &c);   // dem
    if (c.n == 0) return 0;
    kq = new BanChay[c.n];
    c.a = kq;
    c.n = 0;
    btTraverse(kho.cay, gomBanChay, &c);   // ghi
    sapXepBanChay(kq, c.n);
    return c.n < k ? c.n : k;
}

// ---- In bang ----------------------------------------------------------------

void inBangMatHang(const DsMatHang& ds) {
    GiuDinhDang g;
    if (ds.n == 0) {
        std::cout << "(Khong co mat hang nao)\n";
        return;
    }
    std::cout << std::left << std::setw(5) << "STT" << std::setw(15) << "Ma vach" << std::setw(26)
              << "Ten" << std::setw(16) << "Nhom" << std::right << std::setw(12) << "Don gia"
              << std::setw(8) << "Ton" << std::setw(8) << "Nguong" << '\n';
    ke(90);
    for (int i = 0; i < ds.n; ++i) {
        const MatHang* mh = ds.a[i];
        std::cout << std::left << std::setw(5) << i + 1 << std::setw(15) << mh->maVach << std::setw(26)
                  << mh->ten << std::setw(16) << mh->nhom << std::right << std::fixed
                  << std::setprecision(0) << std::setw(12) << mh->donGia << std::setw(8) << mh->tonKho
                  << std::setw(8) << mh->nguongCanhBao << '\n';
    }
    std::cout << "Tong: " << ds.n << " mat hang\n";
}

void inBangCanhBaoHSD(const CanhBaoHSD* a, int n) {
    GiuDinhDang g;
    if (n == 0) {
        std::cout << "(Khong co lo nao sap het han)\n";
        return;
    }
    std::cout << std::left << std::setw(5) << "STT" << std::setw(15) << "Ma vach" << std::setw(26)
              << "Ten" << std::setw(8) << "Ma lo" << std::setw(12) << "HSD" << std::setw(14)
              << "Con lai" << std::right << std::setw(8) << "SL" << '\n';
    ke(88);
    for (int i = 0; i < n; ++i) {
        std::string conLai = a[i].soNgayConLai < 0 ? "DA HET HAN" : std::to_string(a[i].soNgayConLai) + " ngay";
        std::cout << std::left << std::setw(5) << i + 1 << std::setw(15) << a[i].mh->maVach
                  << std::setw(26) << a[i].mh->ten << std::setw(8) << a[i].lo->maLo << std::setw(12)
                  << formatDate(a[i].lo->hsd) << std::setw(14) << conLai << std::right << std::setw(8)
                  << a[i].lo->soLuong << '\n';
    }
}

void inBangGiaTriNhom(const GiaTriNhom* a, int n) {
    GiuDinhDang g;
    if (n == 0) {
        std::cout << "(Kho chua co mat hang nao)\n";
        return;
    }
    std::cout << std::left << std::setw(18) << "Nhom" << std::right << std::setw(10) << "So MH"
              << std::setw(12) << "Tong ton" << std::setw(18) << "Gia tri ton" << '\n';
    ke(58);
    double tong = 0;
    for (int i = 0; i < n; ++i) {
        std::cout << std::left << std::setw(18) << a[i].nhom << std::right << std::setw(10)
                  << a[i].soMatHang << std::setw(12) << a[i].tongTon << std::fixed
                  << std::setprecision(0) << std::setw(18) << a[i].giaTriTon << '\n';
        tong += a[i].giaTriTon;
    }
    ke(58);
    std::cout << std::left << std::setw(40) << "Tong gia tri ton (theo gia nhap)" << std::right
              << std::fixed << std::setprecision(0) << std::setw(18) << tong << '\n';
}

void inBangBanChay(const BanChay* a, int n) {
    GiuDinhDang g;
    if (n == 0) {
        std::cout << "(Khong co phieu xuat nao trong thang)\n";
        return;
    }
    std::cout << std::left << std::setw(6) << "Hang" << std::setw(15) << "Ma vach" << std::setw(26)
              << "Ten" << std::setw(16) << "Nhom" << std::right << std::setw(10) << "SL ban" << '\n';
    ke(73);
    for (int i = 0; i < n; ++i)
        std::cout << std::left << std::setw(6) << i + 1 << std::setw(15) << a[i].mh->maVach
                  << std::setw(26) << a[i].mh->ten << std::setw(16) << a[i].mh->nhom << std::right
                  << std::setw(10) << a[i].soLuongBan << '\n';
}
