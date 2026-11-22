// tests/test_btree.cpp - kiem thu BTree (Khanh Nguyen)
// Bien dich: g++ -std=c++17 -Wall -Wextra -I. BTree.cpp tests/test_btree.cpp -o test_btree
// Chay:      ./test_btree         (them "demo" de xem in cay khi tach/gop voi m = 3)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include "BTree.h"

static int soLoi = 0;
#define KIEMTRA(dk, ...)                                     \
    do {                                                     \
        if (!(dk)) {                                         \
            ++soLoi;                                         \
            std::printf("  LOI (dong %d): ", __LINE__);      \
            std::printf(__VA_ARGS__);                        \
            std::printf("\n");                               \
        }                                                    \
    } while (0)

// ---- Kiem tra bat bien -----------------------------------------------------
// 1) moi nut khong phai goc co minK..m-1 khoa, goc co 1..m-1 khoa
// 2) khoa tang dan va nam dung khoang cua cha
// 3) moi la cung do sau
// 4) tong so khoa = t.size
static int doSauLa;
static int demKhoa;

static bool kiemTraNut(const BTreeNode* x, int m, bool laGoc, int sau,
                       const std::string* duoi, const std::string* tren) {
    int minK = (m + 1) / 2 - 1;
    if (x->n > m - 1) return false;
    if (!laGoc && x->n < minK) return false;
    if (laGoc && x->n < 1) return false;
    for (int i = 0; i < x->n; ++i) {
        const std::string& k = x->keys[i]->maVach;
        if (i > 0 && !(x->keys[i - 1]->maVach < k)) return false;
        if (duoi && !(*duoi < k)) return false;
        if (tren && !(k < *tren)) return false;
    }
    demKhoa += x->n;
    if (x->leaf) {
        if (doSauLa < 0) doSauLa = sau;
        return doSauLa == sau;
    }
    for (int i = 0; i <= x->n; ++i) {
        if (!x->child[i]) return false;
        const std::string* d = i > 0 ? &x->keys[i - 1]->maVach : duoi;
        const std::string* tr = i < x->n ? &x->keys[i]->maVach : tren;
        if (!kiemTraNut(x->child[i], m, false, sau + 1, d, tr)) return false;
    }
    return true;
}

static bool hopLe(const BTree& t) {
    if (!t.root) return t.size == 0;
    doSauLa = -1;
    demKhoa = 0;
    return kiemTraNut(t.root, t.m, true, 0, nullptr, nullptr) && demKhoa == t.size;
}

// ---- Tien ich --------------------------------------------------------------

static MatHang* taoMH(const std::string& ma) {
    MatHang* mh = new MatHang;
    mh->maVach = ma;
    mh->ten = "MH " + ma;
    mh->nhom = "Test";
    mh->donGia = 0;
    mh->tonKho = 0;
    mh->nguongCanhBao = 0;
    mh->dsLo.front = mh->dsLo.rear = nullptr;
    mh->dsLo.count = 0;
    mh->demTam = 0;
    return mh;
}

static std::string maVach13(int i) {
    char buf[16];
    std::snprintf(buf, sizeof buf, "893%010d", i);
    return buf;
}

static void tronMang(int* a, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    }
}

struct DuyetCtx { const std::string* truoc; int dem; bool tangDan; };
static void ghiNhan(MatHang* mh, void* p) {
    DuyetCtx* c = static_cast<DuyetCtx*>(p);
    if (c->truoc && !(*c->truoc < mh->maVach)) c->tangDan = false;
    c->truoc = &mh->maVach;
    c->dem++;
}

// ---- Cac test --------------------------------------------------------------

static void testNgauNhien(int m, int n, bool kiemTraMoiBuoc) {
    std::printf("m = %d, n = %d\n", m, n);
    BTree t;
    btInit(t, m);
    MatHang** ds = new MatHang*[n];
    int* thuTu = new int[n];
    for (int i = 0; i < n; ++i) { ds[i] = taoMH(maVach13(i * 7 + 1)); thuTu[i] = i; }

    // Chen theo thu tu ngau nhien
    tronMang(thuTu, n);
    for (int i = 0; i < n; ++i) {
        KIEMTRA(btInsert(t, ds[thuTu[i]]), "chen that bai %s", ds[thuTu[i]]->maVach.c_str());
        if (kiemTraMoiBuoc) KIEMTRA(hopLe(t), "cay sai sau khi chen %d", i);
    }
    KIEMTRA(hopLe(t), "cay sai sau khi chen het");
    KIEMTRA(t.size == n, "size = %d", t.size);

    // Chen trung ma phai bi tu choi
    MatHang* trung = taoMH(ds[0]->maVach);
    KIEMTRA(!btInsert(t, trung), "chen trung ma ma khong bao loi");
    delete trung;

    // Tim tat ca + tim ma khong co
    for (int i = 0; i < n; ++i)
        KIEMTRA(btSearch(t, ds[i]->maVach) == ds[i], "tim sai %s", ds[i]->maVach.c_str());
    KIEMTRA(btSearch(t, "0000000000000") == nullptr, "tim ma khong ton tai");

    // Duyet tang dan
    DuyetCtx ctx{nullptr, 0, true};
    btTraverse(t, ghiNhan, &ctx);
    KIEMTRA(ctx.dem == n && ctx.tangDan, "duyet sai: dem = %d", ctx.dem);

    std::printf("  chieu cao = %d, so nut = %d\n", btHeight(t), btSoNut(t));

    // Xoa mot nua theo thu tu ngau nhien
    tronMang(thuTu, n);
    int nua = n / 2;
    for (int i = 0; i < nua; ++i) {
        MatHang* mh = ds[thuTu[i]];
        KIEMTRA(btRemove(t, mh->maVach) == mh, "xoa sai %s", mh->maVach.c_str());
        if (kiemTraMoiBuoc) KIEMTRA(hopLe(t), "cay sai sau khi xoa %d", i);
    }
    KIEMTRA(hopLe(t), "cay sai sau khi xoa mot nua");
    KIEMTRA(btRemove(t, ds[thuTu[0]]->maVach) == nullptr, "xoa hai lan ma khong bao loi");
    for (int i = 0; i < n; ++i) {
        MatHang* kq = btSearch(t, ds[thuTu[i]]->maVach);
        if (i < nua) KIEMTRA(kq == nullptr, "van tim thay ma da xoa");
        else KIEMTRA(kq == ds[thuTu[i]], "mat ma chua xoa");
    }

    // Xoa het phan con lai
    for (int i = nua; i < n; ++i) {
        KIEMTRA(btRemove(t, ds[thuTu[i]]->maVach) == ds[thuTu[i]], "xoa sai");
        if (kiemTraMoiBuoc) KIEMTRA(hopLe(t), "cay sai sau khi xoa %d", i);
    }
    KIEMTRA(t.root == nullptr && t.size == 0, "cay chua rong sau khi xoa het");

    btDestroy(t);
    for (int i = 0; i < n; ++i) delete ds[i];
    delete[] ds;
    delete[] thuTu;
}

static void testTangDanGiamDan(int m) {
    std::printf("m = %d, chen tang dan roi xoa giam dan\n", m);
    const int n = 500;
    BTree t;
    btInit(t, m);
    MatHang** ds = new MatHang*[n];
    for (int i = 0; i < n; ++i) { ds[i] = taoMH(maVach13(i)); btInsert(t, ds[i]); }
    KIEMTRA(hopLe(t), "cay sai sau khi chen tang dan");
    for (int i = n - 1; i >= 0; --i) {
        KIEMTRA(btRemove(t, ds[i]->maVach) == ds[i], "xoa sai");
        KIEMTRA(hopLe(t), "cay sai khi xoa giam dan i = %d", i);
    }
    btDestroy(t);
    for (int i = 0; i < n; ++i) delete ds[i];
    delete[] ds;
}

static void testCayRong() {
    std::printf("cay rong\n");
    BTree t;
    btInit(t, 5);
    KIEMTRA(btSearch(t, "12345678") == nullptr, "tim tren cay rong");
    KIEMTRA(btRemove(t, "12345678") == nullptr, "xoa tren cay rong");
    KIEMTRA(btHeight(t) == 0 && btSoNut(t) == 0, "chieu cao/so nut cay rong");
    KIEMTRA(!btInsert(t, nullptr), "chen nullptr");
    btDestroy(t);
}

static void demo() {
    std::printf("=== Demo m = 3: in cay sau moi lan tach/gop ===\n");
    const int n = 10;
    int ma[n] = {50, 20, 80, 10, 30, 60, 90, 40, 70, 25};
    MatHang* ds[n];
    BTree t;
    btInit(t, 3);
    t.inKhiTachGop = true;
    for (int i = 0; i < n; ++i) {
        char buf[16];
        std::snprintf(buf, sizeof buf, "000000%02d", ma[i]);
        ds[i] = taoMH(buf);
        std::printf("Chen %s\n", buf);
        btInsert(t, ds[i]);
    }
    std::printf("\nCay sau khi chen:\n");
    btPrint(t);
    int xoa[] = {20, 30, 50, 10, 90};
    for (int k : xoa) {
        char buf[16];
        std::snprintf(buf, sizeof buf, "000000%02d", k);
        std::printf("\nXoa %s\n", buf);
        btRemove(t, buf);
    }
    std::printf("\nCay cuoi:\n");
    btPrint(t);
    btDestroy(t);
    for (int i = 0; i < n; ++i) delete ds[i];
}

int main(int argc, char** argv) {
    if (argc > 1 && std::strcmp(argv[1], "demo") == 0) {
        demo();
        return 0;
    }
    std::srand(20261122);
    testCayRong();
    const int bac[] = {3, 4, 5, 10};
    for (int m : bac) {
        testNgauNhien(m, 2000, true);      // kiem tra bat bien sau moi thao tac
        testTangDanGiamDan(m);
    }
    for (int m : bac) testNgauNhien(m, 100000, false);
    if (soLoi == 0) std::printf("\nTAT CA TEST DEU QUA\n");
    else std::printf("\nCO %d LOI\n", soLoi);
    return soLoi == 0 ? 0 : 1;
}
