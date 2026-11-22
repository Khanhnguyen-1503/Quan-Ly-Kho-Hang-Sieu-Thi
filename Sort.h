#ifndef SORT_H
#define SORT_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;// Cau truc ngay thang
struct Date {
    int day, month, year;
};
// Thong tin mat hang
struct Product {
    string code;
    string name;
    string category;
    double price;
    int stock;
    int minStock;
    Date expiryDate;
};
// Thong tin mat hang trong phieu xuat
struct ExportItem {
    string productCode;
    int quantity;
};
// Thong tin phieu xuat
struct ExportSlip {
    string slipCode;
    Date exportDate;
    vector<ExportItem> items;
};
// Cac ham xu ly ngay
Date todayDate();
Date addDays(Date d, int days);
int daysBetween(Date a, Date b);
// Tim kiem va liet ke
void searchByName(vector<Product> ds, string ten);
void listByCategory(vector<Product> ds);
// Sap xep Merge Sort
void mergeSort(vector<Product>& ds, int l, int r);
#endif