// BTree.cpp - Khanh Nguyen
// B-Tree bac m (moi nut toi da m-1 khoa, m con).
// Nut khong phai goc co it nhat ceil(m/2)-1 khoa.
//
// Chen: chen vao la, neu nut day (m khoa, dung o "+1") thi tach o giua,
//       day khoa giua len cha; tach lan nguoc len goc.
// Xoa:  khoa o nut trong -> thay bang khoa lon nhat cay con trai (predecessor).
//       Sau khi xoa, nut con nao thieu khoa thi muon anh em trai/phai,
//       khong muon duoc thi gop voi anh em.
#include "BTree.h"
#include <iostream>

// ---------------------------------------------------------------------------
// Ham noi bo
// ---------------------------------------------------------------------------
namespace {

int soSuKien = 0;   // so lan tach/muon/gop trong thao tac hien tai (de in cay)

int soKhoaToiThieu(int m) { return (m + 1) / 2 - 1; }   // ceil(m/2) - 1

BTreeNode* taoNut(int m, bool leaf) {
    BTreeNode* x = new BTreeNode;
    x->n = 0;
    x->leaf = leaf;
    x->keys = new MatHang*[m];          // m-1 khoa + 1 o de tach
    x->child = new BTreeNode*[m + 1];   // m con + 1 o de tach
    for (int i = 0; i <= m; ++i) x->child[i] = nullptr;
    return x;
}

void giaiPhongNut(BTreeNode* x) {
    delete[] x->keys;
    delete[] x->child;
    delete x;
}

void giaiPhongCay(BTreeNode* x) {
    if (!x) return;
    if (!x->leaf)
        for (int i = 0; i <= x->n; ++i) giaiPhongCay(x->child[i]);
    giaiPhongNut(x);
}

// Vi tri dau tien i co keys[i]->maVach >= key (tim nhi phan). Tra ve n neu moi khoa < key.
int timViTri(const BTreeNode* x, const std::string& key) {
    int lo = 0, hi = x->n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (x->keys[mid]->maVach < key) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

void inNut(const BTreeNode* x) {
    std::cout << '[';
    for (int i = 0; i < x->n; ++i) {
        if (i) std::cout << ' ';
        std::cout << x->keys[i]->maVach;
    }
    std::cout << ']';
}

// ---- Chen ------------------------------------------------------------------

// Tach con y = x->child[i] dang co m khoa. Khoa giua len x.
void tachCon(BTree& t, BTreeNode* x, int i) {
    int m = t.m;
    BTreeNode* y = x->child[i];
    int giua = m / 2;
    BTreeNode* z = taoNut(m, y->leaf);

    z->n = m - 1 - giua;
    for (int j = 0; j < z->n; ++j) z->keys[j] = y->keys[giua + 1 + j];
    if (!y->leaf)
        for (int j = 0; j <= z->n; ++j) {
            z->child[j] = y->child[giua + 1 + j];
            y->child[giua + 1 + j] = nullptr;
        }
    y->n = giua;

    for (int j = x->n; j > i; --j) x->keys[j] = x->keys[j - 1];
    for (int j = x->n + 1; j > i + 1; --j) x->child[j] = x->child[j - 1];
    x->keys[i] = y->keys[giua];
    x->child[i + 1] = z;
    x->n++;

    ++soSuKien;
    if (t.inKhiTachGop) {
        std::cout << "  Tach nut: day " << x->keys[i]->maVach << " len, ";
        inNut(y);
        std::cout << " | ";
        inNut(z);
        std::cout << '\n';
    }
}

// Chen mh vao cay con goc x. Sau khi tra ve, x co the dang tran (m khoa): cha se tach.
bool chenDeQuy(BTree& t, BTreeNode* x, MatHang* mh) {
    int i = timViTri(x, mh->maVach);
    if (i < x->n && x->keys[i]->maVach == mh->maVach) return false;   // trung ma

    if (x->leaf) {
        for (int j = x->n; j > i; --j) x->keys[j] = x->keys[j - 1];
        x->keys[i] = mh;
        x->n++;
        return true;
    }
    if (!chenDeQuy(t, x->child[i], mh)) return false;
    if (x->child[i]->n == t.m) tachCon(t, x, i);
    return true;
}

// ---- Xoa -------------------------------------------------------------------

// Gop child[j], keys[j], child[j+1] cua x thanh mot nut (child[j]).
void gopCon(BTree& t, BTreeNode* x, int j) {
    BTreeNode* trai = x->child[j];
    BTreeNode* phai = x->child[j + 1];
    MatHang* khoaGiua = x->keys[j];

    trai->keys[trai->n] = khoaGiua;
    for (int k = 0; k < phai->n; ++k) trai->keys[trai->n + 1 + k] = phai->keys[k];
    if (!trai->leaf)
        for (int k = 0; k <= phai->n; ++k) trai->child[trai->n + 1 + k] = phai->child[k];
    trai->n += 1 + phai->n;

    for (int k = j; k < x->n - 1; ++k) x->keys[k] = x->keys[k + 1];
    for (int k = j + 1; k < x->n; ++k) x->child[k] = x->child[k + 1];
    x->child[x->n] = nullptr;
    x->n--;

    giaiPhongNut(phai);

    ++soSuKien;
    if (t.inKhiTachGop) {
        std::cout << "  Gop nut voi khoa " << khoaGiua->maVach << " -> ";
        inNut(trai);
        std::cout << '\n';
    }
}

// x->child[i] dang thieu khoa: muon trai, muon phai, hoac gop.
void suaThieu(BTree& t, BTreeNode* x, int i) {
    int minK = soKhoaToiThieu(t.m);
    BTreeNode* c = x->child[i];

    if (i > 0 && x->child[i - 1]->n > minK) {            // muon anh trai
        BTreeNode* trai = x->child[i - 1];
        for (int k = c->n; k > 0; --k) c->keys[k] = c->keys[k - 1];
        if (!c->leaf)
            for (int k = c->n + 1; k > 0; --k) c->child[k] = c->child[k - 1];
        c->keys[0] = x->keys[i - 1];
        if (!c->leaf) {
            c->child[0] = trai->child[trai->n];
            trai->child[trai->n] = nullptr;
        }
        x->keys[i - 1] = trai->keys[trai->n - 1];
        trai->n--;
        c->n++;
        ++soSuKien;
        if (t.inKhiTachGop)
            std::cout << "  Muon anh trai: " << x->keys[i - 1]->maVach << " len cha, "
                      << c->keys[0]->maVach << " xuong nut thieu\n";
        return;
    }
    if (i < x->n && x->child[i + 1]->n > minK) {         // muon anh phai
        BTreeNode* phai = x->child[i + 1];
        c->keys[c->n] = x->keys[i];
        if (!c->leaf) c->child[c->n + 1] = phai->child[0];
        x->keys[i] = phai->keys[0];
        for (int k = 0; k < phai->n - 1; ++k) phai->keys[k] = phai->keys[k + 1];
        if (!phai->leaf) {
            for (int k = 0; k < phai->n; ++k) phai->child[k] = phai->child[k + 1];
            phai->child[phai->n] = nullptr;
        }
        phai->n--;
        c->n++;
        ++soSuKien;
        if (t.inKhiTachGop)
            std::cout << "  Muon anh phai: " << x->keys[i]->maVach << " len cha, "
                      << c->keys[c->n - 1]->maVach << " xuong nut thieu\n";
        return;
    }
    if (i > 0) gopCon(t, x, i - 1);
    else gopCon(t, x, i);
}

// Go khoa lon nhat khoi cay con goc x.
MatHang* xoaMax(BTree& t, BTreeNode* x) {
    if (x->leaf) return x->keys[--x->n];
    int i = x->n;
    MatHang* kq = xoaMax(t, x->child[i]);
    if (x->child[i]->n < soKhoaToiThieu(t.m)) suaThieu(t, x, i);
    return kq;
}

MatHang* xoaDeQuy(BTree& t, BTreeNode* x, const std::string& key) {
    int i = timViTri(x, key);
    if (i < x->n && x->keys[i]->maVach == key) {
        MatHang* kq = x->keys[i];
        if (x->leaf) {
            for (int k = i; k < x->n - 1; ++k) x->keys[k] = x->keys[k + 1];
            x->n--;
            return kq;
        }
        x->keys[i] = xoaMax(t, x->child[i]);   // thay bang predecessor
        if (x->child[i]->n < soKhoaToiThieu(t.m)) suaThieu(t, x, i);
        return kq;
    }
    if (x->leaf) return nullptr;
    MatHang* kq = xoaDeQuy(t, x->child[i], key);
    if (kq && x->child[i]->n < soKhoaToiThieu(t.m)) suaThieu(t, x, i);
    return kq;
}

// ---- Duyet / dem / in ------------------------------------------------------

void duyetDeQuy(const BTreeNode* x, BTVisit visit, void* ctx) {
    for (int i = 0; i < x->n; ++i) {
        if (!x->leaf) duyetDeQuy(x->child[i], visit, ctx);
        visit(x->keys[i], ctx);
    }
    if (!x->leaf) duyetDeQuy(x->child[x->n], visit, ctx);
}

int demNut(const BTreeNode* x) {
    if (!x) return 0;
    int s = 1;
    if (!x->leaf)
        for (int i = 0; i <= x->n; ++i) s += demNut(x->child[i]);
    return s;
}

void inMuc(const BTreeNode* x, int muc) {
    if (muc == 0) {
        inNut(x);
        std::cout << ' ';
        return;
    }
    if (!x->leaf)
        for (int i = 0; i <= x->n; ++i) inMuc(x->child[i], muc - 1);
}

void inCaySauSuKien(const BTree& t) {
    if (t.inKhiTachGop && soSuKien > 0) {
        btPrint(t);
        std::cout << '\n';
    }
}

}  // namespace

// ---------------------------------------------------------------------------
// Ham cong khai
// ---------------------------------------------------------------------------

void btInit(BTree& t, int m) {
    t.root = nullptr;
    t.m = m < 3 ? 3 : m;
    t.size = 0;
    t.soNutTruyXuat = 0;
    t.inKhiTachGop = false;
}

MatHang* btSearch(BTree& t, const std::string& maVach) {
    BTreeNode* x = t.root;
    while (x) {
        t.soNutTruyXuat++;
        int i = timViTri(x, maVach);
        if (i < x->n && x->keys[i]->maVach == maVach) return x->keys[i];
        if (x->leaf) return nullptr;
        x = x->child[i];
    }
    return nullptr;
}

bool btInsert(BTree& t, MatHang* mh) {
    if (!mh) return false;
    soSuKien = 0;
    if (!t.root) {
        t.root = taoNut(t.m, true);
        t.root->keys[0] = mh;
        t.root->n = 1;
        t.size = 1;
        return true;
    }
    if (!chenDeQuy(t, t.root, mh)) return false;
    if (t.root->n == t.m) {   // goc tran -> cay cao them 1 muc
        BTreeNode* s = taoNut(t.m, false);
        s->child[0] = t.root;
        t.root = s;
        tachCon(t, s, 0);
    }
    t.size++;
    inCaySauSuKien(t);
    return true;
}

MatHang* btRemove(BTree& t, const std::string& maVach) {
    if (!t.root) return nullptr;
    soSuKien = 0;
    MatHang* kq = xoaDeQuy(t, t.root, maVach);
    if (!kq) return nullptr;
    if (t.root->n == 0) {   // goc rong -> cay thap di 1 muc
        BTreeNode* cu = t.root;
        t.root = cu->leaf ? nullptr : cu->child[0];
        giaiPhongNut(cu);
    }
    t.size--;
    inCaySauSuKien(t);
    return kq;
}

void btTraverse(const BTree& t, BTVisit visit, void* ctx) {
    if (t.root && visit) duyetDeQuy(t.root, visit, ctx);
}

int btHeight(const BTree& t) {
    int h = 0;
    for (const BTreeNode* x = t.root; x; x = x->leaf ? nullptr : x->child[0]) ++h;
    return h;
}

int btSoNut(const BTree& t) { return demNut(t.root); }

void btPrint(const BTree& t) {
    if (!t.root) {
        std::cout << "(cay rong)\n";
        return;
    }
    int h = btHeight(t);
    for (int muc = 0; muc < h; ++muc) {
        std::cout << "Muc " << muc << ": ";
        inMuc(t.root, muc);
        std::cout << '\n';
    }
}

void btDestroy(BTree& t) {
    giaiPhongCay(t.root);
    t.root = nullptr;
    t.size = 0;
}
