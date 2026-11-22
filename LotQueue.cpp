// LotQueue.cpp - Hang doi FIFO cac lo nhap cua mot mat hang (DSLK don)
// Nguoi thuc hien: Minh Tan
#include "LotQueue.h"

void lqInit(LotQueue& q) {
    q.front = nullptr;
    q.rear = nullptr;
    q.count = 0;
}

bool lqRong(const LotQueue& q) {
    return q.front == nullptr;
}

// Them lo vao cuoi hang doi (rear) - O(1)
void lqEnqueue(LotQueue& q, Lo* lo) {
    if (!lo) return;
    lo->next = nullptr;
    if (lqRong(q)) {
        q.front = q.rear = lo;
    } else {
        q.rear->next = lo;
        q.rear = lo;
    }
    q.count++;
}

// Xem lo cu nhat o dau hang doi - O(1)
Lo* lqFront(const LotQueue& q) {
    return q.front;
}

// Go lo cu nhat ra khoi hang doi (khong delete bo nho) - O(1)
Lo* lqDequeue(LotQueue& q) {
    if (lqRong(q)) return nullptr;
    Lo* lo = q.front;
    q.front = q.front->next;
    if (q.front == nullptr) {
        q.rear = nullptr;
    }
    lo->next = nullptr;
    q.count--;
    return lo;
}

// Tinh tong so luong ton cua tat ca cac lo - O(k)
int lqTongSoLuong(const LotQueue& q) {
    int tong = 0;
    for (Lo* cur = q.front; cur != nullptr; cur = cur->next) {
        tong += cur->soLuong;
    }
    return tong;
}

// Giai phong toan bo cac lo trong hang doi
void lqClear(LotQueue& q) {
    Lo* cur = q.front;
    while (cur != nullptr) {
        Lo* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    q.front = q.rear = nullptr;
    q.count = 0;
}

// Ham tao mot nut Lo moi
Lo* taoLo(int maLo, const Date& ngayNhap, const Date& hsd, int soLuong, double giaNhap) {
    Lo* lo = new Lo;
    lo->maLo = maLo;
    lo->ngayNhap = ngayNhap;
    lo->hsd = hsd;
    lo->soLuong = soLuong;
    lo->giaNhap = giaNhap;
    lo->next = nullptr;
    return lo;
}