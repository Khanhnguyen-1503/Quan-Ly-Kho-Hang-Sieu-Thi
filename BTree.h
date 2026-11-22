// BTree.h - Khanh Nguyen
// B-Tree bac m, khoa = MatHang::maVach. Cay chi giu con tro MatHang*,
// KHONG delete MatHang (Inventory chiu trach nhiem giai phong).
#pragma once
#include <string>
#include "Types.h"

typedef void (*BTVisit)(MatHang* mh, void* ctx);   // ham goi lai khi duyet

void     btInit(BTree& t, int m);                              // m >= 3
MatHang* btSearch(BTree& t, const std::string& maVach);        // nullptr neu khong co; cong t.soNutTruyXuat
bool     btInsert(BTree& t, MatHang* mh);                      // false neu trung ma
MatHang* btRemove(BTree& t, const std::string& maVach);        // go khoi cay, tra con tro (nullptr neu khong co)
void     btTraverse(const BTree& t, BTVisit visit, void* ctx); // duyet tang dan theo maVach
int      btHeight(const BTree& t);                             // so muc; cay rong = 0
int      btSoNut(const BTree& t);
void     btPrint(const BTree& t);                              // in cay theo tung muc
void     btDestroy(BTree& t);                                  // giai phong nut, khong dung MatHang
