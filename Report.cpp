#include "Report.h"
#include <map>
#include <algorithm>
#include <iomanip>
// 4. Canh bao ton kho duoi nguong
void warnLowStock(vector<Product> ds) {
    cout << "\n--- CANH BAO TON KHO ---\n";
    bool found = false;
    for (auto p : ds) {
        if (p.stock < p.minStock) {
            cout << p.name
                 << " | Ton: " << p.stock
                 << " | Nguong: " << p.minStock
                 << endl;

            found = true;
        }
    }
    if (!found)
        cout << "Khong co hang ton kho thap!\n";
}
// 5. Canh bao hang het han trong 7 ngay
void warnExpiring(vector<Product> ds) {
    Date today = todayDate();
    vector<Product> list;
    for (auto p : ds) {
        int days = daysBetween(today, p.expiryDate);
        if (days >= 0 && days <= 7)
            list.push_back(p);
    }
    // Sap xep theo han su dung tang dan
    sort(list.begin(), list.end(),
         [](Product a, Product b) {
             return daysBetween(a.expiryDate,
                                b.expiryDate) > 0;
         });
    cout << "\n--- HANG SAP HET HAN ---\n";
    if (list.empty())
        cout << "Khong co hang sap het han!\n";
    for (auto p : list) {
        cout << p.name << " | HSD: "
             << p.expiryDate.day << "/"
             << p.expiryDate.month << "/"
             << p.expiryDate.year << endl;
    }
}
// 6. Bao cao gia tri ton kho theo nhom
void reportInventory(vector<Product> ds) {
    map<string, pair<int, double>> report;
    for (auto p : ds) {
        report[p.category].first += p.stock;
        report[p.category].second +=
            p.stock * p.price;
    }
    cout << "\n--- BAO CAO TON KHO ---\n";
    for (auto x : report) {
        cout << "Nhom: " << x.first
             << " | So luong: " << x.second.first
             << " | Gia tri: "
             << fixed << setprecision(0)
             << x.second.second << endl;
    }
}
// 7. Top 10 mat hang ban chay trong thang
void reportTop10(vector<Product> ds,
                 vector<ExportSlip> slips,
                 int month, int year) {
    map<string, int> sold;
    // Tinh so luong ban tu phieu xuat
    for (auto slip : slips) {
        if (slip.exportDate.month == month &&
            slip.exportDate.year == year) {
            for (auto item : slip.items) {
                sold[item.productCode] += item.quantity;
            }
        }
    }
    vector<pair<string, int>> ranking(
        sold.begin(), sold.end()
    );
    // Sap xep so luong ban giam dan
    sort(ranking.begin(), ranking.end(),
         [](auto a, auto b) {
             return a.second > b.second;
         });
    cout << "\n--- TOP 10 BAN CHAY ---\n";
    if (ranking.empty()) {
        cout << "Chua co du lieu phieu xuat!\n";
        return;
    }
    int limit = min(10, (int)ranking.size());
    for (int i = 0; i < limit; i++) {
        string name = ranking[i].first;
        for (auto p : ds) {
            if (p.code == ranking[i].first) {
                name = p.name;
                break;
            }
        }
        cout << i + 1 << ". "
             << name
             << " | So luong ban: "
             << ranking[i].second << endl;
    }
}
