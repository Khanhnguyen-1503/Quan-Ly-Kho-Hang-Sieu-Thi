#include "DataGen.h"
#include "Date.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>

void sinhDuLieuMau(int soMatHang, const std::string& fileMatHang,
                   const std::string& fileLo, const std::string& filePhieu) {
    std::ofstream fMH(fileMatHang);
    std::ofstream fLo(fileLo);
    std::ofstream fPhieu(filePhieu);

    if (!fMH || !fLo || !fPhieu) return;

    // Moi loai san pham gan voi dung nhom hang cua no
    struct SanPham { const char* ten; const char* nhom; };
    const SanPham dsSanPham[] = {
        {"Sua tuoi", "Thuc pham tuoi"}, {"Trung ga", "Thuc pham tuoi"},
        {"Mi goi", "Thuc pham kho"},    {"Gao", "Thuc pham kho"},
        {"Nuoc ngot", "Do uong"},       {"Tra xanh", "Do uong"},      {"Ca phe", "Do uong"},
        {"Banh quy", "Banh keo"},       {"Keo deo", "Banh keo"},
        {"Nuoc mam", "Gia vi"},         {"Dau an", "Gia vi"},
        {"Dau goi", "Hoa my pham"},     {"Kem danh rang", "Hoa my pham"},
        {"Bot giat", "Do gia dung"},    {"Nuoc rua chen", "Do gia dung"}
    };
    const int soSanPham = sizeof(dsSanPham) / sizeof(dsSanPham[0]);

    Date today = homNay();
    int maLoCounter = 1;
    int maPhieuCounter = 1;

    std::srand(42); // Cố định seed để cả nhóm test trên cùng bộ dữ liệu

    for (int i = 1; i <= soMatHang; ++i) {
        char maVachBuf[16];
        std::snprintf(maVachBuf, sizeof(maVachBuf), "893%010d", i);
        std::string maVach(maVachBuf);

        const SanPham& sp = dsSanPham[i % soSanPham];
        std::string nhom = sp.nhom;
        std::string ten = std::string(sp.ten) + " Loai " + std::to_string(i);
        double donGia = ((std::rand() % 50) + 1) * 5000.0;
        int nguongCanhBao = 10 + (std::rand() % 15);

        // Ghi mathang.txt: maVach|ten|nhom|donGia|nguongCanhBao
        fMH << maVach << "|" << ten << "|" << nhom << "|"
            << static_cast<long long>(donGia) << "|" << nguongCanhBao << "\n";

        // Mỗi mặt hàng sinh 1 đến 3 lô nhập để test FIFO và cảnh báo HSD.
        // Lô được giữ trong mảng tạm, chưa ghi lo.txt vì còn phải trừ phần đã xuất.
        const int MAX_LO = 3;
        int soLo = 1 + (i % 3);
        char maLo[MAX_LO][16];
        Date ngayNhap[MAX_LO];
        Date hsd[MAX_LO];
        int conLai[MAX_LO];
        double giaNhap = donGia * 0.75;

        for (int j = 0; j < soLo; ++j) {
            std::snprintf(maLo[j], sizeof(maLo[j]), "LO%05d", maLoCounter++);

            // Lô đầu tiên nhập cách đây 20 ngày, các lô sau gần hơn
            ngayNhap[j] = congNgay(today, -20 + j * 7);
            // Cố tình tạo một số lô sắp hết hạn trong vòng 1-6 ngày tới (khi i % 5 == 0)
            int hanConLai = (i % 5 == 0 && j == 0) ? (1 + (i % 6)) : (30 + (std::rand() % 180));
            hsd[j] = congNgay(today, hanConLai);
            conLai[j] = 15 + (std::rand() % 40);

            // Sinh phiếu nhập tương ứng (số lượng nhập ban đầu): maPhieu|loai|ngay|maVach|soLuong|donGia
            char maPhieuBuf[16];
            std::snprintf(maPhieuBuf, sizeof(maPhieuBuf), "PN%05d", maPhieuCounter++);
            fPhieu << maPhieuBuf << "|NHAP|" << formatDate(ngayNhap[j]) << "|"
                   << maVach << "|" << conLai[j] << "|"
                   << static_cast<long long>(giaNhap) << "\n";
        }

        // Sinh 1 phiếu xuất trong tháng hiện tại cho khoảng 80% mặt hàng để test Top 10 bán chạy
        if (i % 5 != 1) {
            int soNgayLui = std::rand() % (today.d < 10 ? today.d : 10);   // không lùi sang tháng trước
            Date ngayXuat = congNgay(today, -soNgayLui);

            // Chỉ xuất được từ các lô đã nhập trước hoặc đúng ngày xuất
            int coSan = 0;
            for (int j = 0; j < soLo; ++j)
                if (soSanhDate(ngayNhap[j], ngayXuat) <= 0) coSan += conLai[j];

            if (coSan > 5) {
                int slXuat = 1 + (std::rand() % (coSan / 2));
                char maPhieuBuf[16];
                std::snprintf(maPhieuBuf, sizeof(maPhieuBuf), "PX%05d", maPhieuCounter++);
                fPhieu << maPhieuBuf << "|XUAT|" << formatDate(ngayXuat) << "|"
                       << maVach << "|" << slXuat << "|"
                       << static_cast<long long>(donGia) << "\n";

                // Trừ theo FIFO: lấy từ lô cũ nhất, không đủ thì lấy tiếp lô sau
                int canLay = slXuat;
                for (int j = 0; j < soLo && canLay > 0; ++j) {
                    int lay = conLai[j] < canLay ? conLai[j] : canLay;
                    conLai[j] -= lay;
                    canLay -= lay;
                }
            }
        }

        // Ghi lo.txt: maVach|maLo|ngayNhap|hsd|soLuong(còn lại)|giaNhap
        // Lô đã xuất hết không ghi nữa (đã bị lấy ra khỏi hàng đợi)
        for (int j = 0; j < soLo; ++j) {
            if (conLai[j] <= 0) continue;
            fLo << maVach << "|" << maLo[j] << "|" << formatDate(ngayNhap[j]) << "|"
                << formatDate(hsd[j]) << "|" << conLai[j] << "|"
                << static_cast<long long>(giaNhap) << "\n";
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