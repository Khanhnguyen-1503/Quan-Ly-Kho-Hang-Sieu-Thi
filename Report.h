#ifndef REPORT_H
#define REPORT_H
#include "Sort.h"
// Canh bao ton kho
void warnLowStock(vector<Product> ds);
// Canh bao het han trong 7 ngay
void warnExpiring(vector<Product> ds);
// Bao cao gia tri ton kho theo nhom
void reportInventory(vector<Product> ds);
// Top 10 mat hang ban chay
void reportTop10(vector<Product> ds,
                 vector<ExportSlip> slips,
                 int month, int year);

#endif