#include <iostream>
#include "DataGen.h"

int main() {
    std::cout << "Dang sinh du lieu mau (35 mat hang)...\n";
    
    sinhDuLieuMau(35, "mathang.txt", "lo.txt", "phieu.txt");
    
    std::cout << "Hoan tat! Da tao xong mathang.txt, lo.txt, phieu.txt.\n";
    return 0;
}