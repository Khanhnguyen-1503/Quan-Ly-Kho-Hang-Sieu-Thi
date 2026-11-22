#include "Sort.h"
#include <algorithm>
#include <cctype>
#include <ctime>
// Chuyen chuoi thanh chu thuong
string lowerText(string s) {
    for (char& c : s)
        c = tolower((unsigned char)c);
    return s;
}
// Lay ngay hien tai
Date todayDate() {
    time_t now = time(0);
    tm* t = localtime(&now);
    return {t->tm_mday, t->tm_mon + 1,
            t->tm_year + 1900};
}
// Cong them so ngay
Date addDays(Date d, int days) {
    tm t = {};
    t.tm_mday = d.day + days;
    t.tm_mon = d.month - 1;
    t.tm_year = d.year - 1900;
    mktime(&t);
    return {t.tm_mday, t.tm_mon + 1,
            t.tm_year + 1900};
}
// Tinh khoang cach giua hai ngay
int daysBetween(Date a, Date b) {
    tm x = {}, y = {};
    x.tm_mday = a.day;
    x.tm_mon = a.month - 1;
    x.tm_year = a.year - 1900;
    y.tm_mday = b.day;
    y.tm_mon = b.month - 1;
    y.tm_year = b.year - 1900;
    time_t t1 = mktime(&x);
    time_t t2 = mktime(&y);
    return (int)(difftime(t2, t1) / 86400);
}
// 1. Tim kiem theo ten (chuoi con)
void searchByName(vector<Product> ds, string ten) {
    bool found = false;
    for (auto p : ds) {
        if (lowerText(p.name).find(lowerText(ten))
            != string::npos) {

            cout << p.code << " | "
                 << p.name << " | "
                 << p.category << " | Ton: "
                 << p.stock << endl;

            found = true;
        }
    }
    if (!found)
        cout << "Khong tim thay mat hang!\n";
}
// 2. Liet ke theo nhom hang
void listByCategory(vector<Product> ds) {
    sort(ds.begin(), ds.end(),
         [](Product a, Product b) {
             return a.category < b.category;
         });
    for (auto p : ds) {
        cout << p.category << " | "
             << p.name << " | Ton: "
             << p.stock << endl;
    }
}
// Ham tron hai nua da sap xep
void merge(vector<Product>& ds, int l, int m, int r) {
    vector<Product> temp;
    int i = l;
    int j = m + 1;
    while (i <= m && j <= r) {
        if (ds[i].name <= ds[j].name)
            temp.push_back(ds[i++]);
        else
            temp.push_back(ds[j++]);
    }
    while (i <= m)
        temp.push_back(ds[i++]);
    while (j <= r)
        temp.push_back(ds[j++]);
    for (int k = 0; k < temp.size(); k++)
        ds[l + k] = temp[k];
}
// 3. Thuat toan Merge Sort
void mergeSort(vector<Product>& ds, int l, int r) {
    if (l >= r)
        return;
    int m = (l + r) / 2;
    mergeSort(ds, l, m);
    mergeSort(ds, m + 1, r);
    merge(ds, l, m, r);
}