// LotQueue.h - Minh Tan
// Hang doi FIFO cac lo nhap cua mot mat hang (DSLK tu cai). front = lo cu nhat.
#pragma once
#include "Types.h"

void lqInit(LotQueue& q);
Lo*  taoLo(int maLo, const Date& ngayNhap, const Date& hsd, int soLuong, double giaNhap);  // new Lo, next = nullptr
void lqEnqueue(LotQueue& q, Lo* lo);     // them vao cuoi (lo moi nhat)
Lo*  lqFront(const LotQueue& q);         // lo cu nhat, nullptr neu rong
Lo*  lqDequeue(LotQueue& q);             // go lo cu nhat ra va tra ve; nguoi goi delete
int  lqTongSoLuong(const LotQueue& q);   // tong soLuong cac lo = tonKho
void lqClear(LotQueue& q);               // delete tat ca lo
