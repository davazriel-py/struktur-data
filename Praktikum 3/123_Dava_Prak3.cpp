#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

const string fileCustomer = "cust.txt";
const string filePending = "pending.txt";
const string fileDone = "done.txt";
const string fileMontir = "montir.txt";

struct Customer {
    string nama;
    string telp;
};

struct Service {
    string modelMobil;
    string merekMobil;
    string kendala;
    string montir;
    string tanggalMasuk;
    Customer dataCustomer;
    Service* next;
};

struct CustomerNode {
    Customer data;
    CustomerNode* next;
};

struct CancelNode {
    Service data;
    CancelNode* next;
};

struct MechanicNode {
    string nama;
    MechanicNode* prev;
    MechanicNode* next;
};

Service* frontQueue = NULL;
Service* rearQueue = NULL;
Service* doneHead = NULL;
CustomerNode* customerHead = NULL;
CancelNode* cancelTop = NULL;
MechanicNode* montirRoot = NULL;


void pressEnter();
void rewriteCustFile();
void addCustNode(const Customer& data, bool saveFile);
CustomerNode* findCustName(const string& nama);
bool checkCust(const string& nama, const string& telp);
void loadCustomerFile();
MechanicNode* createMechanicNode(const string& nama);
void insertMontirTree(MechanicNode*& root, const string& nama, bool& inserted);
void inorderMontir(MechanicNode* root, vector<string>& hasil);
void rewriteMontirFile();
void loadMontirFile();
vector<string> getMontirList();
void enqueueService(Service* baru);
void pushDoneService(Service* node);
void addDoneTailLoad(Service* node);
void rewritePendingFile();
void rewriteDoneFile();
void loadPendingFile();
void loadDoneFile();
void printServiceDetail(Service* data);
int getQueueCount();
string pilihMontirMenu();
void menuAntrianAnda(const string& nama, const string& telp);
void menuBookingCustomer(const string& nama, const string& telp);
void pushCancelStack(Service* data);
void menuCancelService(const string& nama, const string& telp);
CancelNode* popCancel(const string& nama, const string& telp);
void menuUndoCancel(const string& nama, const string& telp);
void menuRiwayatCustomer(const string& nama, const string& telp);
void menuServisBaruAdmin();
void menuSemuaServiceAdmin();
void menuSelesaikanServiceAdmin();
void menuPelangganBaruAdmin();
void menuMontirBaruAdmin();
void menuServisAdmin();
void menuAdmin();


void pressEnter() {
    cout << "\nPress enter to go back...";
    string dummy;
    getline(cin, dummy);
}


void rewriteCustFile() {
    ofstream file(fileCustomer);
    CustomerNode* scanner = customerHead;

    while (scanner != NULL) {
        file << scanner->data.nama << "|" << scanner->data.telp << "\n";
        scanner = scanner->next;
    }
}


void addCustNode(const Customer& data, bool saveFile) {
    CustomerNode* baru = new CustomerNode;
    baru->data = data;
    baru->next = NULL;

    if (customerHead == NULL) {
        customerHead = baru;
    } else {
        CustomerNode* scanner = customerHead;
        while (scanner->next != NULL) {
            scanner = scanner->next;
        }
        scanner->next = baru;
    }

    if (saveFile) {
        rewriteCustFile();
    }
}


CustomerNode* findCustName(const string& nama) {
    CustomerNode* scanner = customerHead;
    while (scanner != NULL) {
        if (scanner->data.nama == nama) {
            return scanner;
        }
        scanner = scanner->next;
    }
    return NULL;
}


bool checkCust(const string& nama, const string& telp) {
    CustomerNode* scanner = customerHead;
    while (scanner != NULL) {
        if (scanner->data.nama == nama && scanner->data.telp == telp) {
            return true;
        }
        scanner = scanner->next;
    }
    return false;
}


void loadCustomerFile() {
    ifstream file(fileCustomer);
    if (!file.is_open()) {
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        Customer data;
        size_t posSep = line.find('|');
        if (posSep == string::npos) {
            data.nama = line;
            data.telp = "";
        } else {
            data.nama = line.substr(0, posSep);
            size_t posSep2 = line.find('|', posSep + 1);
            if (posSep2 == string::npos) {
                data.telp = line.substr(posSep + 1);
            } else {
                data.telp = line.substr(posSep + 1, posSep2 - (posSep + 1));
            }
        }
        addCustNode(data, false);
    }
}


MechanicNode* createMechanicNode(const string& nama) {
    MechanicNode* baru = new MechanicNode;
    baru->nama = nama;
    baru->prev = NULL;
    baru->next = NULL;
    return baru;
}


void insertMontirTree(MechanicNode*& root, const string& nama, bool& inserted) {
    if (root == NULL) {
        root = createMechanicNode(nama);
        inserted = true;
        return;
    }

    if (nama == root->nama) {
        inserted = false;
        return;
    }

    if (nama < root->nama) {
        insertMontirTree(root->prev, nama, inserted);
    } else {
        insertMontirTree(root->next, nama, inserted);
    }
}


void inorderMontir(MechanicNode* root, vector<string>& hasil) {
    if (root == NULL) {
        return;
    }

    inorderMontir(root->prev, hasil);
    hasil.push_back(root->nama);
    inorderMontir(root->next, hasil);
}


void rewriteMontirFile() {
    vector<string> daftar;
    inorderMontir(montirRoot, daftar);

    ofstream file(fileMontir);
    for (const string& nama : daftar) {
        file << nama << "\n";
    }
}


void loadMontirFile() {
    ifstream file(fileMontir);
    if (!file.is_open()) {
        vector<string> defaultMontir = {"Suby", "Farhan", "Aldo", "Dimas"};
        for (const string& nama : defaultMontir) {
            bool inserted = false;
            insertMontirTree(montirRoot, nama, inserted);
        }
        rewriteMontirFile();
        return;
    }

    string nama;
    while (getline(file, nama)) {
        if (nama.empty()) {
            continue;
        }
        bool inserted = false;
        insertMontirTree(montirRoot, nama, inserted);
    }
}


vector<string> getMontirList() {
    vector<string> daftar;
    inorderMontir(montirRoot, daftar);
    return daftar;
}


void enqueueService(Service* baru) {
    baru->next = NULL;
    if (frontQueue == NULL) {
        frontQueue = baru;
        rearQueue = baru;
    } else {
        rearQueue->next = baru;
        rearQueue = baru;
    }
}


void pushDoneService(Service* node) {
    node->next = doneHead;
    doneHead = node;
}


void addDoneTailLoad(Service* node) {
    node->next = NULL;
    if (doneHead == NULL) {
        doneHead = node;
        return;
    }
    Service* scanner = doneHead;
    while (scanner->next != NULL) {
        scanner = scanner->next;
    }
    scanner->next = node;
}


void rewritePendingFile() {
    ofstream file(filePending);
    Service* scanner = frontQueue;
    while (scanner != NULL) {
        file << scanner->modelMobil << "|" << scanner->merekMobil << "|" << scanner->kendala << "|" << scanner->montir
             << "|" << scanner->tanggalMasuk << "|" << scanner->dataCustomer.nama << "\n";
        scanner = scanner->next;
    }
}


void rewriteDoneFile() {
    ofstream file(fileDone);
    Service* scanner = doneHead;
    while (scanner != NULL) {
        file << scanner->modelMobil << "|" << scanner->merekMobil << "|" << scanner->kendala << "|" << scanner->montir
             << "|" << scanner->tanggalMasuk << "|" << scanner->dataCustomer.nama << "\n";
        scanner = scanner->next;
    }
}


void loadPendingFile() {
    ifstream file(filePending);
    if (!file.is_open()) {
        return;
    }

    while (true) {
        Service* baru = new Service;
        if (!getline(file, baru->modelMobil, '|')) {
            delete baru;
            break;
        }
        getline(file, baru->merekMobil, '|');
        getline(file, baru->kendala, '|');
        getline(file, baru->montir, '|');
        getline(file, baru->tanggalMasuk, '|');

        string sNama;
        if (!getline(file, sNama)) {
            delete baru;
            break;
        }

        size_t pisah = sNama.find('|');
        if (pisah != string::npos) {
            baru->dataCustomer.nama = sNama.substr(0, pisah);
        } else {
            baru->dataCustomer.nama = sNama;
        }

        CustomerNode* c = findCustName(baru->dataCustomer.nama);
        baru->dataCustomer.telp = (c != NULL ? c->data.telp : "");

        baru->next = NULL;
        enqueueService(baru);
    }
}


void loadDoneFile() {
    ifstream file(fileDone);
    if (!file.is_open()) {
        return;
    }

    while (true) {
        Service* baru = new Service;
        if (!getline(file, baru->modelMobil, '|')) {
            delete baru;
            break;
        }
        getline(file, baru->merekMobil, '|');
        getline(file, baru->kendala, '|');
        getline(file, baru->montir, '|');
        getline(file, baru->tanggalMasuk, '|');

        string sNama;
        if (!getline(file, sNama)) {
            delete baru;
            break;
        }
        size_t pisah = sNama.find('|');
        if (pisah != string::npos) {
            baru->dataCustomer.nama = sNama.substr(0, pisah);
        } else {
            baru->dataCustomer.nama = sNama;
        }

        CustomerNode* c = findCustName(baru->dataCustomer.nama);
        baru->dataCustomer.telp = (c != NULL ? c->data.telp : "");

        baru->next = NULL;
        addDoneTailLoad(baru);
    }
}


void printServiceDetail(Service* data) {
    cout << "-----------------------\n";
    cout << "Model Mobil: " << data->modelMobil << "\n";
    cout << "Merek Mobil: " << data->merekMobil << "\n";
    cout << "Kendala: " << data->kendala << "\n";
    cout << "Montir: " << data->montir << "\n";
    cout << "Nama Pelanggan: " << data->dataCustomer.nama << "\n";
    cout << "No Telp Pelanggan: " << data->dataCustomer.telp << "\n";
    cout << "Tanggal Masuk Bengkel: " << data->tanggalMasuk << "\n";
    cout << "-----------------------\n\n";
}


int getQueueCount() {
    int jumlah = 0;
    Service* scanner = frontQueue;
    while (scanner != NULL) {
        jumlah++;
        scanner = scanner->next;
    }
    return jumlah;
}


string pilihMontirMenu() {
    vector<string> daftar = getMontirList();
    if (daftar.empty()) {
        cout << "Belum ada montir terdaftar.\n";
        return "";
    }

    cout << "Pilih Montir:\n";
    for (int indeks = 0; indeks < static_cast<int>(daftar.size()); indeks++) {
        cout << indeks + 1 << ". " << daftar[static_cast<size_t>(indeks)] << "\n";
    }

    cout << "Pilihan: ";
    int pil = 0;
    cin >> pil;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (pil < 1 || pil > static_cast<int>(daftar.size())) {
        cout << "Pilihan montir tidak valid.\n";
        return "";
    }
    return daftar[static_cast<size_t>(pil - 1)];
}


void menuAntrianAnda(const string& nama, const string& telp) {
    cout << "====== All Services ======\n\n";
    Service* scanner = frontQueue;
    int aNtrian = 1;
    bool aDa_pending = false;
    vector<pair<string, int> > posisi;

    while (scanner != NULL) {
        aDa_pending = true;
        printServiceDetail(scanner);
        if (scanner->dataCustomer.nama == nama && scanner->dataCustomer.telp == telp) {
            posisi.push_back(make_pair(scanner->modelMobil, aNtrian));
        }
        aNtrian++;
        scanner = scanner->next;
    }

    if (!aDa_pending) {
        cout << "Belum ada servis pending.\n";
    } else if (posisi.empty()) {
        cout << "Anda belum punya servis dalam antrian.\n";
    } else {
        for (size_t indeks = 0; indeks < posisi.size(); indeks++) {
            cout << "Servis " << posisi[indeks].first << " Anda berada di antrian ke-" << posisi[indeks].second
                 << "\n";
        }
    }

    pressEnter();
}


void menuBookingCustomer(const string& nama, const string& telp) {
    cout << "====== New Service ======\n";
    string model;
    string merek;
    string kendala;

    cout << "Model Mobil: ";
    getline(cin, model);
    cout << "Merek Mobil: ";
    getline(cin, merek);
    cout << "Kendala: ";
    getline(cin, kendala);

    Service* scanner = frontQueue;
    int posisi = 1;
    while (scanner != NULL) {
        if (scanner->dataCustomer.nama == nama && scanner->dataCustomer.telp == telp &&
            scanner->modelMobil == model &&
            scanner->merekMobil == merek) {
            cout << "\nMobil ini sudah dibengkel anda ingin menambahkan kendala ini kepada servis tersebut (yes/no): ";
            string jawab;
            getline(cin, jawab);
            if (jawab == "yes") {
                if (!scanner->kendala.empty()) {
                    scanner->kendala += " + ";
                }
                scanner->kendala += kendala;
                rewritePendingFile();
                cout << "\n*Kendala sudah diupdate, nomor antrian anda adalah: " << posisi << "*\n";
            } else {
                cout << "\n*Perubahan dibatalkan*\n";
            }
            pressEnter();
            return;
        }
        scanner = scanner->next;
        posisi++;
    }

    string tanggal;
    cout << "Tanggal Masuk Bengkel: ";
    getline(cin, tanggal);

    string mOntir = pilihMontirMenu();
    if (mOntir.empty()) {
        pressEnter();
        return;
    }

    Service* baru = new Service;
    baru->modelMobil = model;
    baru->merekMobil = merek;
    baru->kendala = kendala;
    baru->montir = mOntir;
    baru->tanggalMasuk = tanggal;
    baru->dataCustomer.nama = nama;
    baru->dataCustomer.telp = telp;
    baru->next = NULL;
    enqueueService(baru);
    rewritePendingFile();

    cout << "\n*Servis sudah tercatat, nomor antrian anda adalah: " << getQueueCount() << "*\n";
    pressEnter();
}


void pushCancelStack(Service* data) {
    CancelNode* baru = new CancelNode;
    baru->data = *data;
    baru->data.next = NULL;
    baru->next = cancelTop;
    cancelTop = baru;
}


void menuCancelService(const string& nama, const string& telp) {
    vector<Service*> daftar_service;
    vector<Service*> dAftar_prev;

    Service* scanner = frontQueue;
    Service* pRev = NULL;

    while (scanner != NULL) {
        if (scanner->dataCustomer.nama == nama && scanner->dataCustomer.telp == telp) {
            daftar_service.push_back(scanner);
            dAftar_prev.push_back(pRev);
        }
        pRev = scanner;
        scanner = scanner->next;
    }

    if (daftar_service.empty()) {
        cout << "Tidak ada servis yang bisa dibatalkan.\n";
        pressEnter();
        return;
    }

    cout << "====== Cancel Service ======\n";
    for (size_t indeks = 0; indeks < daftar_service.size(); indeks++) {
        cout << indeks + 1 << ". Servis Ke-" << indeks + 1 << ":\n";
        cout << "   Model Mobil: " << daftar_service[indeks]->modelMobil << "\n";
        cout << "   Merek Mobil: " << daftar_service[indeks]->merekMobil << "\n";
        cout << "   Kendala: " << daftar_service[indeks]->kendala << "\n";
        cout << "   Montir: " << daftar_service[indeks]->montir << "\n\n";
    }

    cout << "Pilih Mobil yang ingin dibatalkan: ";
    int pil = 0;
    cin >> pil;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (pil < 1 || pil > static_cast<int>(daftar_service.size())) {
        cout << "Pilihan tidak valid.\n";
        pressEnter();
        return;
    }

    Service* hapus = daftar_service[static_cast<size_t>(pil - 1)];
    Service* prevHapus = dAftar_prev[static_cast<size_t>(pil - 1)];

    if (prevHapus == NULL) {
        frontQueue = hapus->next;
    } else {
        prevHapus->next = hapus->next;
    }

    if (hapus == rearQueue) {
        rearQueue = prevHapus;
    }

    pushCancelStack(hapus);
    cout << "\n*Servis " << hapus->modelMobil << " telah dibatalkan*\n";
    delete hapus;

    rewritePendingFile();
    pressEnter();
}


CancelNode* popCancel(const string& nama, const string& telp) {
    CancelNode* temp = NULL;

    while (cancelTop != NULL &&
           !(cancelTop->data.dataCustomer.nama == nama && cancelTop->data.dataCustomer.telp == telp)) {
        CancelNode* pindah = cancelTop;
        cancelTop = cancelTop->next;
        pindah->next = temp;
        temp = pindah;
    }

    CancelNode* hasil = NULL;
    if (cancelTop != NULL) {
        hasil = cancelTop;
        cancelTop = cancelTop->next;
        hasil->next = NULL;
    }

    while (temp != NULL) {
        CancelNode* balik = temp;
        temp = temp->next;
        balik->next = cancelTop;
        cancelTop = balik;
    }

    return hasil;
}


void menuUndoCancel(const string& nama, const string& telp) {
    CancelNode* dataBatal = popCancel(nama, telp);
    if (dataBatal == NULL) {
        cout << "Tidak ada data pembatalan untuk di-undo.\n";
        pressEnter();
        return;
    }

    cout << "====== Booking Kembali Service ======\n\n";
    cout << "Model Mobil: " << dataBatal->data.modelMobil << "\n";
    cout << "Merek Mobil: " << dataBatal->data.merekMobil << "\n";
    cout << "Kendala: " << dataBatal->data.kendala << "\n";
    cout << "Montir: " << dataBatal->data.montir << "\n\n";

    cout << "Apakah anda ingin membooking kembali servis ini? (yes/no): ";
    string jawab;
    getline(cin, jawab);
    if (jawab != "yes") {
        cout << "\n*Undo dibatalkan*\n";
        delete dataBatal;
        pressEnter();
        return;
    }

    cout << "Apakah ingin di reschedule? input (-) jika tidak\n";
    cout << "Tanggal Lama: " << dataBatal->data.tanggalMasuk << "\n";
    cout << "Tanggal Baru: ";
    string tanggalBaru;
    getline(cin, tanggalBaru);

    if (tanggalBaru != "-" && !tanggalBaru.empty()) {
        dataBatal->data.tanggalMasuk = tanggalBaru;
    }

    Service* baru = new Service;
    *baru = dataBatal->data;
    baru->next = NULL;
    enqueueService(baru);
    rewritePendingFile();

    cout << "\n*Servis " << baru->modelMobil << " telah dibooking kembali*\n";
    delete dataBatal;
    pressEnter();
}


void menuRiwayatCustomer(const string& nama, const string& telp) {
    cout << "====== Riwayat Servis ======\n";
    bool ditemukan = false;
    Service* scanner = doneHead;
    while (scanner != NULL) {
        if (scanner->dataCustomer.nama == nama && scanner->dataCustomer.telp == telp) {
            ditemukan = true;
            printServiceDetail(scanner);
        }
        scanner = scanner->next;
    }

    if (!ditemukan) {
        cout << "Belum ada riwayat servis selesai.\n";
    }

    pressEnter();
}


void menuServisBaruAdmin() {
    cout << "====== New Service ======\n";
    Service* baru = new Service;
    baru->next = NULL;

    cout << "Model Mobil: ";
    getline(cin, baru->modelMobil);
    cout << "Merek Mobil: ";
    getline(cin, baru->merekMobil);
    cout << "Kendala: ";
    getline(cin, baru->kendala);
    cout << "Tanggal Masuk Bengkel: ";
    getline(cin, baru->tanggalMasuk);

    baru->montir = pilihMontirMenu();
    if (baru->montir.empty()) {
        delete baru;
        pressEnter();
        return;
    }

    cout << "Nama Pelanggan: ";
    getline(cin, baru->dataCustomer.nama);

    CustomerNode* cust = findCustName(baru->dataCustomer.nama);
    if (cust != NULL) {
        baru->dataCustomer = cust->data;
    } else {
        cout << "No Telp Pelanggan: ";
        getline(cin, baru->dataCustomer.telp);
        addCustNode(baru->dataCustomer, true);
    }

    enqueueService(baru);
    rewritePendingFile();
    cout << "\n*Servis sudah tercatat, nomor antrian " << baru->dataCustomer.nama
         << " adalah: " << getQueueCount() << "*\n";
    pressEnter();
}


void menuSemuaServiceAdmin() {
    cout << "====== All Services ======\n\n";
    if (frontQueue == NULL) {
        cout << "Tidak ada servis pending.\n";
        pressEnter();
        return;
    }

    Service* scanner = frontQueue;
    int no = 1;
    while (scanner != NULL) {
        cout << "No. " << no << "\n";
        printServiceDetail(scanner);
        scanner = scanner->next;
        no++;
    }
    pressEnter();
}


void menuSelesaikanServiceAdmin() {
    if (frontQueue == NULL) {
        cout << "Tidak ada servis yang bisa diselesaikan.\n";
        pressEnter();
        return;
    }

    vector<Service*> daftar;
    vector<Service*> daftar_prev;
    Service* scanner = frontQueue;
    Service* prev = NULL;
    int nO = 1;

    cout << "====== Selesaikan Servis ======\n";
    while (scanner != NULL) {
        cout << nO << ". " << scanner->modelMobil << " - " << scanner->dataCustomer.nama << " (" << scanner->montir
             << ")\n";
        daftar.push_back(scanner);
        daftar_prev.push_back(prev);
        prev = scanner;
        scanner = scanner->next;
        nO++;
    }

    cout << "Pilih nomor servis yang selesai: ";
    int pil = 0;
    cin >> pil;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (pil < 1 || pil > static_cast<int>(daftar.size())) {
        cout << "Pilihan tidak valid.\n";
        pressEnter();
        return;
    }

    Service* selesai = daftar[static_cast<size_t>(pil - 1)];
    Service* prevSelesai = daftar_prev[static_cast<size_t>(pil - 1)];

    cout << "\nDetail servis yang dipilih:\n";
    printServiceDetail(selesai);
    cout << "Apakah Anda yakin ingin menyelesaikan servis ini? (yes/no): ";
    string konfirmasi;
    getline(cin, konfirmasi);
    if (konfirmasi != "yes") {
        cout << "\n*Operasi selesai servis dibatalkan*\n";
        pressEnter();
        return;
    }

    if (prevSelesai == NULL) {
        frontQueue = selesai->next;
    } else {
        prevSelesai->next = selesai->next;
    }

    if (selesai == rearQueue) {
        rearQueue = prevSelesai;
    }

    pushDoneService(selesai);
    rewritePendingFile();
    rewriteDoneFile();

    cout << "\n*Servis " << selesai->modelMobil << " telah dipindah ke riwayat selesai*\n";
    pressEnter();
}


void menuPelangganBaruAdmin() {
    cout << "====== Pelanggan Baru ======\n";
    Customer data;
    cout << "Nama Pelanggan: ";
    getline(cin, data.nama);

    if (findCustName(data.nama) != NULL) {
        cout << "Pelanggan sudah terdaftar.\n";
        pressEnter();
        return;
    }

    cout << "No Telp: ";
    getline(cin, data.telp);

    addCustNode(data, true);
    cout << "\n*Pelanggan baru berhasil ditambahkan*\n";
    pressEnter();
}


void menuMontirBaruAdmin() {
    cout << "====== New Mechanic ======\n";
    cout << "Masukkan Nama montir baru\n";
    cout << "Nama: ";
    string nama;
    getline(cin, nama);

    bool inserted = false;
    insertMontirTree(montirRoot, nama, inserted);
    if (!inserted) {
        cout << "\n*" << nama << " sudah terdaftar sebagai montir*\n";
    } else {
        rewriteMontirFile();
        cout << "\n*" << nama << " telah ditambahkan ke daftar montir*\n";
    }
    pressEnter();
}


void menuServisAdmin() {
    while (true) {
        cout << "====== Services ======\n";
        cout << "1. Semua Servis\n";
        cout << "2. Servis Baru\n";
        cout << "3. Selesaikan Servis\n";
        cout << "4. Kembali\n";
        cout << "Pilihan: ";

        int pil = 0;
        cin >> pil;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (pil == 1) {
            menuSemuaServiceAdmin();
        } else if (pil == 2) {
            menuServisBaruAdmin();
        } else if (pil == 3) {
            menuSelesaikanServiceAdmin();
        } else if (pil == 4) {
            break;
        } else {
            cout << "Pilihan tidak valid.\n";
            pressEnter();
        }
        cout << "\n";
    }
}


void menuAdmin() {
    while (true) {
        cout << "====== Welcome to Lognuts ======\n";
        cout << "Pilih menu!\n\n";
        cout << "1. Servis\n";
        cout << "2. Pelanggan Baru\n";
        cout << "3. Montir Baru\n";
        cout << "4. Keluar\n\n";
        cout << "Pilihan: ";

        int pil = 0;
        cin >> pil;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (pil == 1) {
            menuServisAdmin();
        } else if (pil == 2) {
            menuPelangganBaruAdmin();
        } else if (pil == 3) {
            menuMontirBaruAdmin();
        } else if (pil == 4) {
            break;
        } else {
            cout << "Pilihan tidak valid.\n";
            pressEnter();
        }
        cout << "\n";
    }
}


void menuCustomer(const string& nama, const string& telp) {
    while (true) {
        cout << "====== Welcome " << nama << " ======\n";
        cout << "Pilih menu:\n\n";
        cout << "1. Antrian Anda\n";
        cout << "2. Booking Servis\n";
        cout << "3. Batalkan Servis\n";
        cout << "4. Undo Pembatalan\n";
        cout << "5. Riwayat Servis\n";
        cout << "6. Keluar\n\n";
        cout << "Pilihan: ";

        int pil = 0;
        cin >> pil;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (pil == 1) {
            menuAntrianAnda(nama, telp);
        } else if (pil == 2) {
            menuBookingCustomer(nama, telp);
        } else if (pil == 3) {
            menuCancelService(nama, telp);
        } else if (pil == 4) {
            menuUndoCancel(nama, telp);
        } else if (pil == 5) {
            menuRiwayatCustomer(nama, telp);
        } else if (pil == 6) {
            break;
        } else {
            cout << "Pilihan tidak valid.\n";
            pressEnter();
        }
        cout << "\n";
    }
}


bool menuLogin() {
    cout << "====== Welcome To Garasi Suby ======\n";
    cout << "Masukkan nama & Nomor Telepon\n";
    cout << "(ketik \"exit\" pada Nama untuk menutup program)\n\n";

    string nama;
    cout << "Nama: ";
    getline(cin, nama);

    if (nama == "exit") {
        return false;
    }

    if (nama == "adminacces8008") {
        cout << "\n";
        menuAdmin();
        return true;
    }

    string telp;
    cout << "No Telp: ";
    getline(cin, telp);

    if (!checkCust(nama, telp)) {
        CustomerNode* samaNama = findCustName(nama);
        if (samaNama == NULL) {
            cout << "\nNama customer tidak ditemukan.\n";
            pressEnter();
            return true;
        }
        cout << "\nNama ditemukan, tetapi nomor telepon tidak cocok.\n";
        pressEnter();
        return true;
    }

    cout << "\n";
    menuCustomer(nama, telp);
    return true;
}


int main() {
    loadCustomerFile();
    loadMontirFile();
    loadPendingFile();
    loadDoneFile();

    while (true) {
        if (!menuLogin()) {
            break;
        }
        cout << "\n";
    }

    return 0;
}





