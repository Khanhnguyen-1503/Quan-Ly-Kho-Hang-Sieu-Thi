#include "../include/DataGen.h"
#include "../include/Date.h"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstdio>

void sinhDuLieuMau(int soMatHang, const std::string& fileMatHang,
                   const std::string& fileLo, const std::string& filePhieu) {
    std::ofstream fMH(fileMatHang);
    std::ofstream fLo(fileLo);
    std::ofstream fPhieu(filePhieu);

    if (!fMH || !fLo || !fPhieu) return;

    const std::vector<std::string> nhomHang = {
        "Thuc pham tuoi", "Do uong", "Banh keo", "Gia vi", "Hoa my pham", "Do gia dung"
    };
    const std::vector<std::string> tienToTen = {
        "Sua tuoi", "Mi goi", "Nuoc ngot", "Banh quy", "Dau an",
        "Nuoc mam", "Bot giat", "Dau goi", "Tra xanh", "Ca phe"
    };

    Date today = homNay();
    int maLoCounter = 1;
    int maPhieuCounter = 1;

    std::srand(42); // Cố định seed để cả nhóm test trên cùng bộ dữ liệu

    for (int i = 1; i <= soMatHang; ++i) {
        char maVachBuf[16];
        std::snprintf(maVachBuf, sizeof(maVachBuf), "893%010d", i);
        std::string maVach(maVachBuf);

        std::string nhom = nhomHang[i % nhomHang.size()];
        std::string ten = tienToTen[i % tienToTen.size()] + " Loai " + std::to_string(i);
        double donGia = ((std::rand() % 50) + 1) * 5000.0;
        int nguongCanhBao = 10 + (std::rand() % 15);

        // Ghi mathang.txt: maVach|ten|nhom|donGia|nguongCanhBao
        fMH << maVach << "|" << ten << "|" << nhom << "|"
            << static_cast<long long>(donGia) << "|" << nguongCanhBao << "\n";

        // Mỗi mặt hàng sinh 1 đến 3 lô nhập để test FIFO và cảnh báo HSD
        int soLo = 1 + (i % 3);
        int tongTon = 0;

        for (int j = 0; j < soLo; ++j) {
            char maLoBuf[16];
            std::snprintf(maLoBuf, sizeof(maLoBuf), "LO%05d", maLoCounter++);
            
            // Lô đầu tiên nhập cách đây 20 ngày, các lô sau gần hơn
            Date ngayNhap = congNgay(today, -20 + j * 7);
            // Cố tình tạo một số lô sắp hết hạn trong vòng 1-6 ngày tới (khi i % 5 == 0)
            int hanConLai = (i % 5 == 0 && j == 0) ? (1 + (i % 6)) : (30 + (std::rand() % 180));
            Date hsd = congNgay(today, hanConLai);

            int soLuongNhap = 15 + (std::rand() % 40);
            double giaNhap = donGia * 0.75;
            tongTon += soLuongNhap;

            // Ghi lo.txt: maVach|maLo|ngayNhap|hsd|soLuong|giaNhap
            fLo << maVach << "|" << maLoBuf << "|" << formatDate(ngayNhap) << "|"
                << formatDate(hsd) << "|" << soLuongNhap << "|"
                << static_cast<long long>(giaNhap) << "\n";

            // Sinh phiếu nhập tương ứng: maPhieu|loai|ngay|maVach|soLuong|donGia
            char maPhieuBuf[16];
            std::snprintf(maPhieuBuf, sizeof(maPhieuBuf), "PN%05d", maPhieuCounter++);
            fPhieu << maPhieuBuf << "|NHAP|" << formatDate(ngayNhap) << "|"
                   << maVach << "|" << soLuongNhap << "|"
                   << static_cast<long long>(giaNhap) << "\n";
        }

        // Sinh 1 phiếu xuất trong tháng hiện tại cho khoảng 60% mặt hàng để test Top 10 bán chạy
        if (i % 5 != 1 && tongTon > 5) {
            int slXuat = 1 + (std::rand() % (tongTon / 2));
            Date ngayXuat = congNgay(today, -(std::rand() % 10));
            char maPhieuBuf[16];
            std::snprintf(maPhieuBuf, sizeof(maPhieuBuf), "PX%05d", maPhieuCounter++);

            fPhieu << maPhieuBuf << "|XUAT|" << formatDate(ngayXuat) << "|"
                   << maVach << "|" << slXuat << "|"
                   << static_cast<long long>(donGia) << "\n";
        }
    }
}

void sinhDuLieuLon(int soMatHang, const std::string& fileMatHang) {
    std::ofstream fMH(fileMatHang);
    if (!fMH) return;

    std::srand(100);
    for (int i = 1; i <= soMatHang; ++i) {
        // Tạo mã vạch ngẫu nhiên hóa thứ tự bằng phép nhân modulo nguyên tố để tránh chèn tăng dần đơn điệu
        long long code = (1LL * i * 7919) % 1000000000LL;
        char maVachBuf[16];
        std::snprintf(maVachBuf, sizeof(maVachBuf), "893%010lld", code);

        fMH << maVachBuf << "|San pham " << i << "|Nhom " << (i % 10)
            << "|" << ((i % 100 + 1) * 1000) << "|10\n";
    }
}