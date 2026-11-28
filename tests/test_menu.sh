#!/bin/sh
# tests/test_menu.sh - chay menu that bang kich ban nhap san, tren mot ban sao cua data/
# Cach dung: sh tests/test_menu.sh ./quanlykho     (chay tu thu muc goc cua repo)
APP=${1:-./quanlykho}
TMP=$(mktemp -d)
cp data/mathang.txt data/lo.txt data/phieu.txt "$TMP"/
LOI=0

kiemtra() {   # kiemtra <mo ta> <chuoi phai co trong out.txt>
    if ! grep -q -- "$2" "$TMP/out.txt"; then
        echo "  LOI: $1 (khong thay \"$2\")"
        LOI=$((LOI + 1))
    fi
}
demdong() { grep -c -- "$2" "$1"; }

echo "Menu: kich ban day du tren ban sao data/"
# Moi dong la mot lan nhap. Cac dong "sai" de kiem tra menu hoi lai.
printf '%s\n' \
    99 abc \
    1 12345678 "Nuoc suoi Test" "Do uong" 5000 50 \
    1 12345678 \
    7 12345678 0 30 3000 "" 01/01/1999 01/01/2099 \
    8 12345678 31 12 "" \
    2 12345678 "" "" 6000 "" \
    4 12345678 \
    5 suoi \
    6 "do uong" \
    10 11 12 \
    13 "" "" \
    9 k \
    14 \
    15 1 87654321 "Test tach" "Khac" 1 1 3 87654321 c 15 \
    3 12345678 c c \
    18 0 \
    | "$APP" "$TMP" > "$TMP/out.txt" 2>&1
MA=$?
[ $MA -eq 0 ] || { echo "  LOI: ma thoat $MA"; LOI=$((LOI + 1)); }

kiemtra "nap du lieu" "Da nap 35 mat hang, 65 lo, 99 phieu"
kiemtra "chon ngoai menu" "phai nam trong \[0, 18\]"
kiemtra "chon khong phai so" "phai la so nguyen"
kiemtra "them mat hang" "Thanh cong"
kiemtra "them trung ma" "Ma vach 12345678 da ton tai"
kiemtra "nhap so luong 0" "phai nam trong \[1, 1000000\]"
kiemtra "HSD truoc ngay nhap" "HSD khong duoc truoc ngay nhap"
kiemtra "tao phieu nhap" "Da tao phieu nhap PN00100"
kiemtra "xuat vuot ton" "phai nam trong \[1, 30\]"
kiemtra "tao phieu xuat" "Da tao phieu xuat PX00101"
kiemtra "sua gia, giu ten" "Don gia ban     : 6000.00"
kiemtra "ton sau xuat" "Tong ton kho    : 18"
kiemtra "dem nut B-Tree" "B-Tree: tim qua"
kiemtra "tim theo ten" "Nuoc suoi Test"
kiemtra "canh bao ton" "DUOI nguong canh bao"
kiemtra "canh bao HSD" "het han trong 7 ngay"
kiemtra "gia tri nhom" "Tong gia tri ton (theo gia nhap)"
kiemtra "top 10" "Top 10 ban chay thang"
kiemtra "danh sach phieu" "Tong so phieu trong kho: 101"
kiemtra "in cay" "Muc 0:"
kiemtra "bat in cay" "In cay sau moi lan tach/muon/gop nut: BAT"
kiemtra "tat in cay" "In cay sau moi lan tach/muon/gop nut: TAT"
kiemtra "xoa khi con ton" "Mat hang van con 18 don vi"
kiemtra "luu" "Da luu vao"

# Tep sau khi luu: 2 mat hang them roi xoa -> van 35; phieu 99 + 1 nhap + 1 xuat
[ "$(demdong "$TMP/mathang.txt" '|')" -eq 35 ] || { echo "  LOI: mathang.txt phai con 35 dong"; LOI=$((LOI + 1)); }
grep -q "^PN00100|NHAP|" "$TMP/phieu.txt" || { echo "  LOI: thieu PN00100 trong phieu.txt"; LOI=$((LOI + 1)); }
grep -q "^PX00101|XUAT|.*|12345678|12|5000$" "$TMP/phieu.txt" || {   # gia luc xuat, truoc khi sua
    echo "  LOI: thieu PX00101 dung noi dung trong phieu.txt"; LOI=$((LOI + 1)); }
grep -q "12345678" "$TMP/mathang.txt" && { echo "  LOI: 12345678 da xoa ma van con"; LOI=$((LOI + 1)); }
ls "$TMP"/*.tmp >/dev/null 2>&1 && { echo "  LOI: con sot tep .tmp"; LOI=$((LOI + 1)); }

echo "Menu: het du lieu nhap giua chung -> van luu va thoat"
printf '1\n11112222\nHang dang nhap\n' | "$APP" "$TMP" > "$TMP/out.txt" 2>&1
[ $? -eq 0 ] || { echo "  LOI: ma thoat khi het dau vao"; LOI=$((LOI + 1)); }
kiemtra "het dau vao" "(Het du lieu nhap) Luu va thoat."

echo "Menu: thu muc khong ton tai"
printf '0\nc\n' | "$APP" "$TMP/khong_co" > "$TMP/out.txt" 2>&1
kiemtra "kho rong" "Bat dau voi kho rong"
kiemtra "luu that bai" "LOI: khong ghi duoc du lieu"

echo "Menu: sinh du lieu mau 30 mat hang, bac B-Tree 3"
printf '17\nc\n30\n14\n0\n' | "$APP" "$TMP" 3 > "$TMP/out.txt" 2>&1
kiemtra "sinh lai va nap" "Da nap 30 mat hang"
kiemtra "bac 3" "B-Tree bac m = 3: 30 khoa"

rm -rf "$TMP"
if [ $LOI -eq 0 ]; then echo "TAT CA TEST MENU DEU QUA"; else echo "CO $LOI LOI"; fi
[ $LOI -eq 0 ]
