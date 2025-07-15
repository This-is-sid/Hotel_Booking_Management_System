#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

void intro() {
    cout << "\n\n C++ Project On Hotel Management System";
}

void head() {
    cout << "\n=========================================";
    cout << "\n  Welcome to CozyStay Group of Hotels";
    cout << "\n=========================================";
}

class hotel {
private:
    int room_no;
    char name[30];
    char address[50];
    char phone[15];
    int days;
    float fare;
    char status[10]; // NEW: Confirmed or Canceled
public:
    void main_menu();
    void add();
    void display();
    void rooms();
    void edit();
    int check(int);
    void modify(int);
    void delete_rec(int);
    void summary(); // NEW: KPI-style summary
};

// Function to validate input safely
void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void hotel::main_menu() {
    int choice;
    while (true) {
        head();
        cout << "\n1. Book A Room\n2. Customer Record\n3. Rooms Allotted";
        cout << "\n4. Edit Record\n5. Booking Summary\n6. Exit";
        cout << "\nEnter Your Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: add(); break;
            case 2: display(); break;
            case 3: rooms(); break;
            case 4: edit(); break;
            case 5: summary(); break;
            case 6: return;
            default: cout << "\nInvalid choice. Try again!\n";
        }
    }
}

void hotel::add() {
    head();
    int r, flag;
    ofstream fout("Record.dat", ios::app | ios::binary);
    cout << "\nEnter Room no: ";
    cin >> r;
    flag = check(r);
    if (flag) {
        cout << "\nRoom is already booked!";
    } else {
        room_no = r;
        cin.ignore();
        cout << "\nName: ";
        cin.getline(name, 30);
        cout << "Address: ";
        cin.getline(address, 50);
        cout << "Phone No: ";
        cin.getline(phone, 15);
        cout << "Days to Checkout: ";
        cin >> days;

        if (days <= 0 || strlen(phone) < 10) {
            cout << "\nInvalid data. Booking failed.";
            fout.close();
            return;
        }

        fare = days * 900;
        strcpy(status, "Confirmed");
        fout.write(reinterpret_cast<char*>(this), sizeof(hotel));
        cout << "\nRoom booked successfully!";
    }
    fout.close();
}

void hotel::display() {
    head();
    ifstream fin("Record.dat", ios::in | ios::binary);
    int r, flag = 0;
    cout << "\nEnter room no: ";
    cin >> r;
    while (fin.read(reinterpret_cast<char*>(this), sizeof(hotel))) {
        if (room_no == r) {
            cout << "\nRoom no: " << room_no << "\nName: " << name;
            cout << "\nAddress: " << address << "\nPhone no: " << phone;
            cout << "\nDays: " << days << "\nTotal Fare: " << fare;
            cout << "\nStatus: " << status;
            flag = 1;
            break;
        }
    }
    if (!flag) cout << "\nRoom not found or vacant.";
    fin.close();
}

void hotel::rooms() {
    head();
    ifstream fin("Record.dat", ios::in | ios::binary);
    vector<hotel> records;

    while (fin.read(reinterpret_cast<char*>(this), sizeof(hotel))) {
        records.push_back(*this);
    }
    fin.close();

    sort(records.begin(), records.end(), [](const hotel &a, const hotel &b) {
        return a.room_no < b.room_no;
    });

    cout << "\nList Of Rooms Allotted\n";
    for (auto &h : records) {
        cout << "\nRoom no: " << h.room_no << "\nName: " << h.name;
        cout << "\nAddress: " << h.address << "\nPhone: " << h.phone;
        cout << "\nDays: " << h.days << "\nTotal: " << h.fare;
        cout << "\nStatus: " << h.status;
        cout << "\n------------------------------";
    }
}

void hotel::edit() {
    head();
    int choice, r;
    cout << "\n1. Modify Customer Record\n2. Cancel Booking\nEnter your choice: ";
    cin >> choice;
    cout << "\nEnter room no: ";
    cin >> r;
    if (choice == 1) modify(r);
    else if (choice == 2) delete_rec(r);
    else cout << "\nInvalid choice!";
}

int hotel::check(int r) {
    ifstream fin("Record.dat", ios::in | ios::binary);
    while (fin.read(reinterpret_cast<char*>(this), sizeof(hotel))) {
        if (room_no == r && strcmp(status, "Confirmed") == 0) {
            fin.close();
            return 1;
        }
    }
    fin.close();
    return 0;
}

void hotel::modify(int r) {
    fstream file("Record.dat", ios::in | ios::out | ios::binary);
    long pos;
    while (file.read(reinterpret_cast<char*>(this), sizeof(hotel))) {
        pos = file.tellg();
        if (room_no == r && strcmp(status, "Confirmed") == 0) {
            cin.ignore();
            cout << "\nNew Name: ";
            cin.getline(name, 30);
            cout << "New Address: ";
            cin.getline(address, 50);
            cout << "New Phone: ";
            cin.getline(phone, 15);
            cout << "New Days: ";
            cin >> days;
            fare = days * 900;
            file.seekp(pos - sizeof(hotel));
            file.write(reinterpret_cast<char*>(this), sizeof(hotel));
            cout << "\nRecord modified successfully!";
            break;
        }
    }
    file.close();
}

void hotel::delete_rec(int r) {
    fstream file("Record.dat", ios::in | ios::out | ios::binary);
    long pos;
    while (file.read(reinterpret_cast<char*>(this), sizeof(hotel))) {
        pos = file.tellg();
        if (room_no == r && strcmp(status, "Confirmed") == 0) {
            strcpy(status, "Canceled");
            file.seekp(pos - sizeof(hotel));
            file.write(reinterpret_cast<char*>(this), sizeof(hotel));
            cout << "\nBooking canceled successfully!";
            break;
        }
    }
    file.close();
}

void hotel::summary() {
    head();
    ifstream fin("Record.dat", ios::in | ios::binary);
    int count = 0;
    float total = 0;
    while (fin.read(reinterpret_cast<char*>(this), sizeof(hotel))) {
        if (strcmp(status, "Confirmed") == 0) {
            count++;
            total += fare;
        }
    }
    fin.close();
    cout << "\nTotal Active Bookings: " << count;
    cout << "\nTotal Revenue: Rs. " << total;
}

int main() {
    hotel h;
    intro();
    h.main_menu();
    return 0;
}