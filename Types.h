// Types.h - Cac struct dung chung cho ca nhom (De tai 9 - Quan ly kho hang sieu thi)
// Da chot ngay 1. Muon sua struct phai bao ca nhom truoc.
#pragma once
#include <string>

struct Date { int d; int m; int y; };

struct Lo {
    int maLo;
    Date ngayNhap;
    Date hsd;          // HSD gan voi tung lo
    int soLuong;       // con lai trong lo
    double giaNhap;
    Lo* next;
};

struct LotQueue { Lo* front; Lo* rear; int count; };   // front = lo cu nhat

struct MatHang {
    std::string maVach;   // 8 hoac 13 chu so, khoa B-Tree
    std::string ten;
    std::string nhom;
    double donGia;
    int tonKho;           // = tong soLuong cac lo
    int nguongCanhBao;
    LotQueue dsLo;
    int demTam;           // bien tam cho bao cao
};

enum LoaiPhieu { PHIEU_NHAP = 0, PHIEU_XUAT = 1 };

struct Phieu {
    int maPhieu; LoaiPhieu loai; Date ngay;
    std::string maVach; int soLuong; double donGia;
    Phieu* next;
};

struct DanhSachPhieu { Phieu* head; Phieu* tail; int count; int maPhieuKeTiep; };

struct BTreeNode {
    int n; bool leaf;
    MatHang** keys;       // toi da m-1 khoa (+1 o de tach)
    BTreeNode** child;    // toi da m con (+1)
};

struct BTree {
    BTreeNode* root; int m; int size;
    long long soNutTruyXuat;   // phan nang cao
    bool inKhiTachGop;         // phan nang cao
};

struct DsMatHang { MatHang** a; int n; int cap; };   // thay vector

struct CanhBaoHSD { MatHang* mh; Lo* lo; int soNgayConLai; };
struct BanChay    { MatHang* mh; int soLuongBan; };
struct GiaTriNhom { std::string nhom; int soMatHang; int tongTon; double giaTriTon; };

enum KetQua { KQ_OK = 0, KQ_TRUNG_MA, KQ_KHONG_TIM_THAY, KQ_KHONG_DU_HANG,
              KQ_DU_LIEU_SAI, KQ_CON_TON_KHO, KQ_LOI_TEP };
