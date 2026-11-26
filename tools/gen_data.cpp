// tools/gen_data.cpp - Huynh Tin
// Chuong trinh rieng sinh du lieu mau (35 mat hang) vao thu muc hien tai.
// Menu chinh (main.cpp) cung co muc sinh du lieu.
#include <iostream>
#include "DataGen.h"

int main() {
    std::cout << "Dang sinh du lieu mau (35 mat hang)...\n";
    
    sinhDuLieuMau(35, "mathang.txt", "lo.txt", "phieu.txt");
    
    std::cout << "Hoan tat! Da tao xong mathang.txt, lo.txt, phieu.txt.\n";
    return 0;
}