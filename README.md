# Quản lý kho hàng siêu thị (Đề tài 9)

C++17, không dùng `vector`, `map`, `sort` của STL. Mặt hàng lưu trong **B-Tree bậc m** (khóa là mã vạch),
mỗi mặt hàng có **hàng đợi FIFO các lô** (DSLK), hạn sử dụng gắn với từng lô.

## Build và chạy

```bash
make            # build chương trình chính ./quanlykho
make run        # chạy menu với dữ liệu trong data/
make test       # build và chạy toàn bộ test
make bench      # thực nghiệm B-Tree m = 3, 5, 10 -> ketqua/ketqua_btree.csv
```

Windows (MinGW): dùng `mingw32-make`, chạy `quanlykho.exe data`.
Không có make: `g++ -std=c++17 -O2 -I. *.cpp -o quanlykho`

Tham số: `./quanlykho [thư mục dữ liệu] [bậc B-Tree]`, mặc định `data 5`.
Dữ liệu đọc khi khởi động, ghi lại khi thoát (mục 0) hoặc khi chọn Lưu (mục 18).

## Mô-đun

| Tệp | Người phụ trách | Nội dung |
|---|---|---|
| `Types.h` | Cả nhóm | Các struct dùng chung |
| `BTree` | Khánh Nguyên | Tìm, chèn (tách), xóa (mượn, gộp), duyệt, in cây, đếm nút truy xuất |
| `Benchmark` | Khánh Nguyên, Huỳnh Tín | Đo B-Tree n = 10³–10⁵, m = 3, 5, 10 |
| `LotQueue`, `Phieu`, `Inventory` | Minh Tân | Hàng đợi lô, danh sách phiếu, nhập kho / xuất kho FIFO |
| `Sort`, `Report` | Long Vũ | Merge Sort, tìm theo tên, theo nhóm, cảnh báo, báo cáo, top 10 |
| `Date`, `Validate`, `FileIO`, `DataGen`, `main` | Huỳnh Tín | Ngày, kiểm tra nhập, đọc/ghi tệp, sinh dữ liệu, menu |

## Định dạng tệp (`data/`)

Phân cách `|`, ngày `dd/mm/yyyy`. `tonKho` không ghi ra tệp mà tính lại từ các lô.

```
mathang.txt  maVach|ten|nhom|donGia|nguongCanhBao
lo.txt       maVach|maLo|ngayNhap|hsd|soLuong(còn lại)|giaNhap     (theo thứ tự FIFO)
phieu.txt    maPhieu|loai|ngay|maVach|soLuong|donGia
```

Mã lô / mã phiếu trong tệp có tiền tố, trong chương trình là số nguyên:
`LO00001` ↔ 1, `PN00006` (nhập) / `PX00006` (xuất) ↔ 6. Phiếu nhập và xuất dùng chung một dãy số.

## Test

| Test | Kiểm tra |
|---|---|
| `test_btree` | Bất biến B-Tree sau mỗi thao tác, m = 3, 4, 5, 10; `./test_btree demo` in từng bước tách/gộp |
| `test_date` | Mọi ngày 1900–2100, năm nhuận, chuỗi sai |
| `test_kho` | LotQueue, Phieu, nhập/xuất FIFO; 20 000 thao tác ngẫu nhiên so với mô hình |
| `test_report` | Tìm kiếm, cảnh báo, báo cáo, top 10 trên dữ liệu tính tay và 10⁵ mặt hàng |
| `test_fileio` | Kiểm tra nhập, đọc/ghi tệp, dòng lỗi, đọc rồi ghi lại giống hệt |
| `test_dulieu` | Tính nhất quán của `data/` (lô khớp phiếu, FIFO, mã) |
| `tests/test_menu.sh` | Chạy menu thật bằng kịch bản nhập sẵn |
