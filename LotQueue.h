// LotQueue.h - Hang doi FIFO cac lo nhap cua mot mat hang (DSLK don co front/rear)
// Phu trach: Minh Tan
#pragma once
#include "Types.h"

void lqInit(LotQueue& q);
bool lqRong(const LotQueue& q);
void lqEnqueue(LotQueue& q, Lo* lo);     // them vao cuoi. O(1)
Lo*  lqFront(const LotQueue& q);         // lo cu nhat. O(1)
Lo*  lqDequeue(LotQueue& q);             // go lo cu nhat ra (khong delete). O(1)
int  lqTongSoLuong(const LotQueue& q);   // tong so luong cac lo. O(k)
void lqClear(LotQueue& q);               // delete het cac lo

Lo* taoLo(int maLo, const Date& ngayNhap, const Date& hsd, int soLuong, double giaNhap);
