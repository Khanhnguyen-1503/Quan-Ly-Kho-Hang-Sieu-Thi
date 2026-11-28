# Quản Lý Kho Hàng Siêu Thị

Đề tài 9: chương trình quản lý kho hàng siêu thị viết bằng **C++17**. Các cấu trúc dữ liệu đều do nhóm tự cài đặt, không dùng `std::vector`, `std::map` hay `std::sort`:

- **B-Tree bậc m** lưu mặt hàng, khóa là mã vạch, để tìm kiếm, thêm và xóa nhanh.
- **Hàng đợi FIFO** (danh sách liên kết đơn) quản lý các lô nhập của từng mặt hàng. Khi xuất kho, lô cũ nhất được lấy trước.
- **Danh sách liên kết đơn** lưu phiếu nhập và phiếu xuất.
- **Mảng động và Merge Sort** dùng để sắp xếp, tìm kiếm và lập báo cáo.

## Thành viên nhóm

| Thành viên | Phụ trách |
|---|---|
| Khanh Nguyen | B-Tree (`BTree`), thực nghiệm B-Tree (`Benchmark`) |
| Huynh Tin | Ngày tháng (`Date`), đọc/ghi tệp (`FileIO`), sinh dữ liệu (`DataGen`), kiểm tra nhập liệu (`Validate`), thực nghiệm (`Benchmark`) |
| Minh Tan | Hàng đợi lô (`LotQueue`), phiếu nhập/xuất (`Phieu`), nghiệp vụ kho (`Inventory`) |
| Long Vu | Sắp xếp (`Sort`), tìm kiếm, cảnh báo và báo cáo (`Report`) |

## Chức năng

- **Mặt hàng:** thêm, sửa, xóa, tìm theo mã vạch (8 hoặc 13 chữ số).
- **Nhập kho:** mỗi lần nhập tạo một lô mới với ngày nhập, hạn sử dụng, số lượng và giá nhập, đồng thời ghi phiếu nhập.
- **Xuất kho FIFO:** lấy hàng từ lô cũ nhất, lô hết thì chuyển sang lô kế tiếp, rồi ghi phiếu xuất. Nếu không đủ hàng thì không xuất gì.
- **Tìm kiếm và liệt kê:** tìm theo tên (chuỗi con, không phân biệt hoa thường), liệt kê theo nhóm hàng.
- **Cảnh báo:** mặt hàng sắp hết (tồn kho dưới ngưỡng), lô sắp hết hạn hoặc đã hết hạn.
- **Báo cáo:** giá trị tồn kho theo nhóm, top k mặt hàng bán chạy trong tháng.
- **Menu console** 18 chức năng: đọc dữ liệu khi khởi động, ghi lại khi thoát; mọi giá trị nhập đều được kiểm tra, nhập sai thì hỏi lại.
- **B-Tree (nâng cao):** xem cây, bật/tắt in cây sau mỗi lần tách/gộp nút, chạy thực nghiệm m = 3, 5, 10 ngay từ menu.

## Cấu trúc thư mục

```
├── Types.h               Các struct dùng chung (Date, Lo, MatHang, Phieu, BTree, ...)
├── BTree.h/.cpp          B-Tree bậc m (m >= 3)
├── LotQueue.h/.cpp       Hàng đợi FIFO các lô nhập
├── Phieu.h/.cpp          Danh sách phiếu nhập/xuất
├── Inventory.h/.cpp      Nghiệp vụ kho: thêm/sửa/xóa, nhập kho, xuất kho
├── Sort.h/.cpp           Mảng động DsMatHang và Merge Sort
├── Report.h/.cpp         Tìm kiếm, cảnh báo, báo cáo
├── Date.h/.cpp           Kiểu ngày dd/mm/yyyy
├── FileIO.h/.cpp         Đọc/ghi tệp dữ liệu (bỏ qua và báo dòng lỗi, ghi qua tệp tạm, sao lưu .bak)
├── DataGen.h/.cpp        Sinh dữ liệu mẫu và dữ liệu lớn
├── Validate.h/.cpp       Nhập dữ liệu từ bàn phím có kiểm tra
├── Benchmark.h/.cpp      Thực nghiệm hiệu năng B-Tree
├── main.cpp              Menu console cho người dùng
├── tools/gen_data.cpp    Chương trình riêng sinh dữ liệu mẫu
├── Makefile
├── data/                 Dữ liệu mẫu: mathang.txt, lo.txt, phieu.txt
├── ketqua/               Kết quả thực nghiệm (ketqua_btree.csv)
├── tests/                Các chương trình kiểm thử
└── baocao/               Báo cáo LaTeX (baocao.tex, chuong1-6.tex) và baocao.pdf
```

## Biên dịch và chạy

Yêu cầu: `g++` hỗ trợ C++17 và `make`. Trên Windows dùng MinGW, thay `make` bằng `mingw32-make`.

```bash
make            # build chương trình chính ./quanlykho
make run        # chạy menu với dữ liệu trong data/
make all        # build chương trình chính, test, bench và gen_data
make test       # build và chạy toàn bộ test
make bench      # chạy thực nghiệm B-Tree, ghi ra ketqua/ketqua_btree.csv
make gen_data   # build chương trình sinh dữ liệu mẫu
make baocao     # biên dịch báo cáo LaTeX -> baocao/baocao.pdf (cần XeLaTeX)
make clean      # xóa file đã build
```

Chạy chương trình: `./quanlykho [thư mục dữ liệu] [bậc B-Tree]`, mặc định `data 5`
(Windows: `quanlykho.exe data`). Không có `make`: `g++ -std=c++17 -O2 -I. *.cpp -o quanlykho`.

Kiểm tra rò rỉ và lỗi bộ nhớ (Linux/macOS):

```bash
make clean test CXXFLAGS="-std=c++17 -g -O1 -Wall -Wextra -fsanitize=address,undefined"
```

Mỗi file trong `tests/` có ghi sẵn lệnh `g++` ở đầu file nếu muốn biên dịch riêng từng phần.

## Kiểm thử

| Chương trình | Nội dung kiểm thử |
|---|---|
| `test_btree` | Chèn/xóa B-Tree với m = 3, 4, 5, 10, đến 100.000 khóa; chèn tăng dần rồi xóa giảm dần. Chạy `./test_btree demo` để xem cây được in ra khi tách/gộp nút. |
| `test_date` | Duyệt mọi ngày 1900–2100, năm nhuận, `parseDate`, cộng/trừ/so sánh ngày |
| `test_kho` | LotQueue, Phiếu, thêm/sửa/xóa/tìm, nhập kho, xuất kho FIFO, 20.000 thao tác ngẫu nhiên trên 2.000 mã |
| `test_report` | Merge Sort, tìm theo tên, liệt kê theo nhóm, cảnh báo, báo cáo, bộ dữ liệu 100.000 mặt hàng |
| `test_fileio` | Kiểm tra nhập liệu (`Validate`), đọc/ghi tệp: đọc `data/` rồi ghi lại giống hệt từng byte, tệp có dòng lỗi, thiếu tệp, 10.000 mặt hàng |
| `test_dulieu` | Kiểm tra 3 tệp trong `data/`: đúng định dạng, tổng số lượng các lô khớp với nhập − xuất |
| `tests/test_menu.sh` | Chạy menu thật bằng kịch bản nhập sẵn (có nhập sai xen kẽ) trên bản sao của `data/`, kiểm tra màn hình và tệp đã lưu |

Khi một bộ test qua hết, chương trình in `TAT CA TEST ... DEU QUA`. Khi có test sai, chương trình in `LOI (dong X): ...` kèm dòng bị lỗi trong file test.

Kết quả gần nhất: **7/7 bộ test đều qua**, không có cảnh báo khi biên dịch với `-Wall -Wextra -pedantic`, và không có lỗi khi chạy với AddressSanitizer/UBSan.

## Định dạng tệp dữ liệu

Các trường phân cách bằng `|`, ngày theo dạng `dd/mm/yyyy`.

| Tệp | Định dạng | Ví dụ |
|---|---|---|
| `mathang.txt` | `maVach\|ten\|nhom\|donGia\|nguongCanhBao` | `8930000000001\|Trung ga Loai 1\|Thuc pham tuoi\|85000\|10` |
| `lo.txt` | `maVach\|maLo\|ngayNhap\|hsd\|soLuong\|giaNhap` | `8930000000001\|LO00001\|02/11/2026\|01/06/2027\|16\|63750` |
| `phieu.txt` | `maPhieu\|loai\|ngay\|maVach\|soLuong\|donGia` | `PN00001\|NHAP\|02/11/2026\|8930000000001\|16\|63750` |

- Tồn kho **không** được ghi ra tệp. Chương trình tính lại bằng cách cộng số lượng các lô khi đọc.
- Dòng sai định dạng bị bỏ qua khi đọc (kèm thông báo tên tệp, số dòng, lý do) và **không** được ghi lại. Ở lần lưu đầu tiên mỗi lần chạy, ba tệp cũ được đổi tên thành `*.txt.bak` thay vì bị ghi đè, nên dòng lỗi và các lô liên quan vẫn còn để sửa tay. Lần chạy sau sẽ thay `.bak` bằng dữ liệu lúc mở chương trình, vì vậy hãy sửa từ `.bak` trước khi chạy lại.
- Mã lô có tiền tố `LO`. Mã phiếu có tiền tố `PN` (phiếu nhập) hoặc `PX` (phiếu xuất), phần số có ít nhất 5 chữ số. Phiếu nhập và phiếu xuất dùng chung một dãy số, nên `PN00006` và `PX00006` không được cùng tồn tại.

## Kết quả thực nghiệm B-Tree

Chèn rồi tìm lại n mã vạch theo thứ tự ngẫu nhiên (trích `ketqua/ketqua_btree.csv`):

| n | m | Thời gian chèn (ms) | Thời gian tìm (ms) | Số nút truy xuất TB/lần tìm | Chiều cao | Số nút |
|---:|---:|---:|---:|---:|---:|---:|
| 1.000 | 3 | 0,28 | 0,27 | 7,27 | 8 | 734 |
| 1.000 | 10 | 0,18 | 0,15 | 3,84 | 4 | 164 |
| 10.000 | 3 | 3,90 | 3,16 | 10,26 | 11 | 7.449 |
| 10.000 | 10 | 3,12 | 2,52 | 4,84 | 5 | 1.635 |
| 100.000 | 3 | 118,23 | 107,99 | 13,25 | 14 | 74.673 |
| 100.000 | 5 | 93,54 | 85,94 | 8,63 | 9 | 36.930 |
| 100.000 | 10 | 62,09 | 62,15 | 5,84 | 6 | 16.358 |

Nhận xét: khi tăng bậc m, cây thấp hơn và số nút phải truy xuất mỗi lần tìm giảm rõ rệt. Ví dụ với 100.000 khóa, chiều cao giảm từ 14 xuống 6 khi m tăng từ 3 lên 10. Chiều cao tăng theo log n, đúng với lý thuyết. Thời gian đo phụ thuộc vào máy chạy.

## Tiến độ

- [x] B-Tree, LotQueue, Phiếu, Inventory, Sort, Report, Date, DataGen, Benchmark
- [x] Bộ kiểm thử cho từng module
- [x] Hàm đọc/ghi tệp trong `FileIO.cpp` (`docDuLieu`, `ghiDuLieu`, ...)
- [x] `Validate.cpp` (nhập liệu có kiểm tra)
- [x] Menu console trong `main.cpp`
- [x] Báo cáo LaTeX (`baocao/`)
- [ ] Slide thuyết trình
