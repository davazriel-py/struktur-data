#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Service;

struct Customer {
    string nama;
    int umur;
    string gender;
    string noTelp;
    string alamat;

    Service* serviceHead;
    Customer* next;
};

struct Service {
    string modelMobil;
    string merekMobil;
    string kendala;
    string montir;

    Customer* customer;
    Service* nextGlobal;
    Service* nextCustomer;
};

Customer* headCustomer = NULL;
Service* headService = NULL;

// ====================== UTIL ======================

Customer* findCustomer(string nama) {
    Customer* temp = headCustomer;
    while (temp != NULL) {
        if (temp->nama == nama)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void addCustomer(Customer* newCust) {
    newCust->next = headCustomer;
    headCustomer = newCust;
}

void addService(Service* newServ) {
    newServ->nextGlobal = headService;
    headService = newServ;

    newServ->nextCustomer = newServ->customer->serviceHead;
    newServ->customer->serviceHead = newServ;
}

void pressEnter() {
    cout << "\nPress any key to go back ...";
    cin.ignore();
    cin.get();
}

// ====================== FILE SAVE ======================

void saveCustomers() {
    ofstream file("customers.txt");
    Customer* temp = headCustomer;

    while (temp != NULL) {
        file << temp->nama << "|"
             << temp->umur << "|"
             << temp->gender << "|"
             << temp->noTelp << "|"
             << temp->alamat << "\n";
        temp = temp->next;
    }
    file.close();
}

void saveServices() {
    ofstream file("services.txt");
    Service* temp = headService;

    while (temp != NULL) {
        file << temp->modelMobil << "|"
             << temp->merekMobil << "|"
             << temp->kendala << "|"
             << temp->montir << "|"
             << temp->customer->nama << "\n";
        temp = temp->nextGlobal;
    }
    file.close();
}

// ====================== FILE LOAD ======================

void loadCustomers() {
    ifstream file("customers.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string nama, umurStr, gender, telp, alamat;

        getline(ss, nama, '|');
        getline(ss, umurStr, '|');
        getline(ss, gender, '|');
        getline(ss, telp, '|');
        getline(ss, alamat, '|');

        Customer* newCust = new Customer;
        newCust->nama = nama;
        newCust->umur = stoi(umurStr);
        newCust->gender = gender;
        newCust->noTelp = telp;
        newCust->alamat = alamat;
        newCust->serviceHead = NULL;
        newCust->next = NULL;

        addCustomer(newCust);
    }
    file.close();
}

void loadServices() {
    ifstream file("services.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string model, merek, kendala, montir, namaCust;

        getline(ss, model, '|');
        getline(ss, merek, '|');
        getline(ss, kendala, '|');
        getline(ss, montir, '|');
        getline(ss, namaCust, '|');

        Customer* cust = findCustomer(namaCust);
        if (cust != NULL) {
            Service* newServ = new Service;
            newServ->modelMobil = model;
            newServ->merekMobil = merek;
            newServ->kendala = kendala;
            newServ->montir = montir;
            newServ->customer = cust;
            newServ->nextGlobal = NULL;
            newServ->nextCustomer = NULL;

            addService(newServ);
        }
    }
    file.close();
}

// ====================== MENU ======================

void semuaServisSingkat() {
    cout << "\n====== All Services ======\n";

    Service* temp = headService;

    if (temp == NULL) {
        cout << "Belum ada servis.\n";
        pressEnter();
        return;
    }

    while (temp != NULL) {
        cout << "-----------------------\n";
        cout << "Model Mobil: " << temp->modelMobil << endl;
        cout << "Merek Mobil: " << temp->merekMobil << endl;
        cout << "Kendala: " << temp->kendala << endl;
        cout << "Montir: " << temp->montir << endl;
        cout << "Nama Pelanggan: " << temp->customer->nama << endl;
        cout << "No Telp Pelanggan: " << temp->customer->noTelp << endl;
        cout << "-----------------------\n";
        temp = temp->nextGlobal;
    }

    pressEnter();
}

void riwayatMontir() {
    cout << "\n====== Mechanic's job history ======\n";
    cout << "1. Suby\n2. Farhan\n3. Dimas\n4. Aldo\n";
    cout << "Pilihan: ";

    int pilih;
    cin >> pilih;
    cin.ignore();

    string montir;
    if (pilih == 1) montir = "Suby";
    else if (pilih == 2) montir = "Farhan";
    else if (pilih == 3) montir = "Dimas";
    else if (pilih == 4) montir = "Aldo";
    else return;

    cout << "\n====== " << montir << "'s Jobs ======\n";

    Service* temp = headService;

    while (temp != NULL) {
        if (temp->montir == montir) {
            cout << "-----------------------\n";
            cout << "Model Mobil: " << temp->modelMobil << endl;
            cout << "Merek Mobil: " << temp->merekMobil << endl;
            cout << "Kendala: " << temp->kendala << endl;
            cout << "Montir: " << temp->montir << endl;
            cout << "Nama Pelanggan: " << temp->customer->nama << endl;
            cout << "No Telp Pelanggan: " << temp->customer->noTelp << endl;
            cout << "-----------------------\n";
        }
        temp = temp->nextGlobal;
    }

    pressEnter();
}

void semuaDataPelanggan() {
    cout << "\n====== All Customers ======\n";

    Customer* temp = headCustomer;

    if (temp == NULL) {
        cout << "Belum ada pelanggan.\n";
        pressEnter();
        return;
    }

    while (temp != NULL) {
        cout << "-----------------------\n";
        cout << "Nama: " << temp->nama << endl;
        cout << "Nomor Telepon: " << temp->noTelp << endl;
        cout << "Alamat: " << temp->alamat << endl;

        if (temp->serviceHead != NULL) {
            cout << "___Servis Terakhir___\n";
            cout << "Mobil: "
                 << temp->serviceHead->merekMobil
                 << " "
                 << temp->serviceHead->modelMobil << endl;
            cout << "Kendala: "
                 << temp->serviceHead->kendala << endl;
            cout << "Montir: "
                 << temp->serviceHead->montir << endl;
        }

        temp = temp->next;
    }

    pressEnter();
}

Customer* getPrevious(Customer* current) {
    Customer* temp = headCustomer;
    Customer* prev = NULL;

    while (temp != NULL && temp != current) {
        prev = temp;
        temp = temp->next;
    }
    return prev;
}

void dataPelanggan() {
    if (headCustomer == NULL) {
        cout << "\nBelum ada pelanggan.\n";
        pressEnter();
        return;
    }

    Customer* temp = headCustomer;
    char pilih;

    while (true) {
        cout << "\n====== Customer Data ======\n";
        cout << "Nama: " << temp->nama << endl;
        cout << "Nomor Telepon: " << temp->noTelp << endl;
        cout << "Umur: " << temp->umur << endl;
        cout << "Gender: " << temp->gender << endl;
        cout << "Alamat: " << temp->alamat << endl;

        cout << "\n3 Servis Terakhir\n";

        Service* serv = temp->serviceHead;
        int count = 0;

        while (serv != NULL && count < 3) {
            cout << "-----------------------\n";
            cout << "Mobil: "
                 << serv->merekMobil
                 << " "
                 << serv->modelMobil << endl;
            cout << "Kendala: "
                 << serv->kendala << endl;
            cout << "Montir: "
                 << serv->montir << endl;

            serv = serv->nextCustomer;
            count++;
        }

        cout << "\n[N]ext, [P]revious, [E]xit\n";
        cout << "Pilihan: ";
        cin >> pilih;

        if ((pilih == 'N' || pilih == 'n') && temp->next != NULL)
            temp = temp->next;
        else if ((pilih == 'P' || pilih == 'p')) {
            Customer* prev = getPrevious(temp);
            if (prev != NULL)
                temp = prev;
        }
        else if (pilih == 'E' || pilih == 'e')
            break;
    }
}

void servisBaru() {
    cin.ignore();

    string model, merek, kendala, montir, nama;

    cout << "\n====== New Service ======\n";
    cout << "Model Mobil: "; getline(cin, model);
    cout << "Merek Mobil: "; getline(cin, merek);
    cout << "Kendala: "; getline(cin, kendala);
    cout << "Montir: "; getline(cin, montir);
    cout << "Nama Pelanggan: "; getline(cin, nama);

    Customer* cust = findCustomer(nama);

    if (cust == NULL) {
        string telp, alamat, gender;
        int umur;

        cout << "No Telp Pelanggan: "; getline(cin, telp);
        cout << "Alamat: "; getline(cin, alamat);
        cout << "Umur: "; cin >> umur; cin.ignore();
        cout << "Gender: "; getline(cin, gender);

        Customer* newCust = new Customer;
        newCust->nama = nama;
        newCust->umur = umur;
        newCust->gender = gender;
        newCust->noTelp = telp;
        newCust->alamat = alamat;
        newCust->serviceHead = NULL;
        newCust->next = NULL;

        addCustomer(newCust);
        cust = newCust;

        cout << "*Pelanggan baru telah terdaftar dan servis berhasil dicatat*\n";
    } else {
        cout << "*Pelanggan sudah terdaftar dan servis berhasil dicatat*\n";
    }

    Service* newServ = new Service;
    newServ->modelMobil = model;
    newServ->merekMobil = merek;
    newServ->kendala = kendala;
    newServ->montir = montir;
    newServ->customer = cust;
    newServ->nextGlobal = NULL;
    newServ->nextCustomer = NULL;

    addService(newServ);

    saveCustomers();
    saveServices();

    pressEnter();
}

// ====================== MAIN ======================

int main() {
    loadCustomers();
    loadServices();

    int pilih;

    do {
        cout << "\n====== Welcome To Lognuts ======\n";
        cout << "1. Servis\n";
        cout << "2. Semua Data Pelanggan\n";
        cout << "3. Data Pelanggan\n";
        cout << "4. Keluar\n";
        cout << "Pilihan: ";

        cin >> pilih;

        switch (pilih) {
            case 1: {
                int sub;
                cout << "\n====== Services ======\n";
                cout << "1. Semua Servis Singkat\n";
                cout << "2. Servis Baru\n";
                cout << "3. Riwayat Kerja Montir\n";
                cout << "Pilihan: ";
                cin >> sub;

                if (sub == 1) semuaServisSingkat();
                else if (sub == 2) servisBaru();
                else if (sub == 3) riwayatMontir();
                break;
            }
            case 2:
                semuaDataPelanggan();
                break;
            case 3:
                dataPelanggan();
                break;
        }

    } while (pilih != 4);

    return 0;
}