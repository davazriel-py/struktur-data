#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct Service {
    string model, merek, pelanggan, tanggal, montir, kendala;
    int urgency;

    Service* next;
};

Service* head = NULL;

void saveToFile() {
    ofstream file("service.txt");
    Service* temp = head;

    while (temp) {
        file << temp->model << "|"
             << temp->merek << "|"
             << temp->pelanggan << "|"
             << temp->tanggal << "|"
             << temp->urgency << "|"
             << temp->montir << "|"
             << temp->kendala << endl;

        temp = temp->next;
    }
    file.close();
}

void loadFromFile() {
    ifstream file("service.txt");
    if (!file.is_open()) return;

    while (!file.eof()) {
        Service* newNode = new Service();
        string line;

        getline(file, newNode->model, '|');
        if (newNode->model == "") break;

        getline(file, newNode->merek, '|');
        getline(file, newNode->pelanggan, '|');
        getline(file, newNode->tanggal, '|');

        string urg;
        getline(file, urg, '|');
        newNode->urgency = stoi(urg);

        getline(file, newNode->montir, '|');
        getline(file, newNode->kendala);

        newNode->next = NULL;

        if (!head) head = newNode;
        else {
            Service* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }
    file.close();
}

int dateToInt(string date) {
    int d, m, y;
    sscanf(date.c_str(), "%d-%d-%d", &d, &m, &y);

    return y * 10000 + m * 100 + d;
}

bool isDateExist(string date) {
    Service* temp = head;
    while (temp) {
        if (temp->tanggal == date) return true;
        temp = temp->next;
    }
    return false;
}

int daysInMonth(int m, int y) {
    if (m == 2) return 28;
    if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
    return 31;
}

bool isValidDate(string date) {
    if (date.length() != 8) return false;

    if (date[2] != '-' || date[5] != '-') return false;

    int d, m, y;

    if (sscanf(date.c_str(), "%d-%d-%d", &d, &m, &y) != 3)
        return false;

    if (y < 0 || y > 99) return false;

    if (m < 1 || m > 12) return false;

    if (d < 1 || d > daysInMonth(m, y)) return false;

    return true;
}


string nextDay(string date) {
    int d, m, y;
    sscanf(date.c_str(), "%d-%d-%d", &d, &m, &y);

    d++;
    if (d > daysInMonth(m, y)) {
        d = 1;
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
    }

    char newDate[20];
    sprintf(newDate, "%02d-%02d-%02d", d, m, y);
    return string(newDate);
}

string recursiveDate(string date, int count) {
    if (count == 5) {
        cout << "Sudah diundur 5x, masukkan tanggal baru: ";
        cin >> date;
        count = 0;
    }

    if (!isDateExist(date)) return date;

    cout << "Tanggal sudah dipakai, diundur " << count+1 << " hari\n";

    date = nextDay(date);

    return recursiveDate(nextDay(date), count + 1);
}

void addService() {
    Service* newNode = new Service();

    cout << "Model Mobil: "; cin >> newNode->model;
    cout << "Merek Mobil: "; cin >> newNode->merek;
    cout << "Kendala: "; cin.ignore(); getline(cin, newNode->kendala);
    cout << "Montir: "; cin >> newNode->montir;
    cout << "Nama Pelanggan: "; cin >> newNode->pelanggan;
    cout << "Urgency (1-4): "; cin >> newNode->urgency;
    string inputTanggal;
    while (true) {
        cout << "Tanggal (dd-mm-yy): ";
        cin >> inputTanggal;
        
        if (isValidDate(inputTanggal)) break;

        cout << "Tanggal tidak valid!\n";
    }

    newNode->tanggal = inputTanggal;

    newNode->next = NULL;

    if (!head) head = newNode;
    else {
        Service* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }

    saveToFile();
}

void display(Service* temp) {
    while (temp) {
        cout << "------------------\n";
        cout << "Model: " << temp->model << endl;
        cout << "Merek: " << temp->merek << endl;
        cout << "Pelanggan: " << temp->pelanggan << endl;
        cout << "Tanggal: " << temp->tanggal << endl;
        cout << "Urgency: " << temp->urgency << endl;
        cout << "Montir: " << temp->montir << endl;
        cout << "Kendala: " << temp->kendala << endl;
        temp = temp->next;
    }
}

Service* copyList() {
    if (!head) return NULL;

    Service* newHead = NULL;
    Service* tail = NULL;

    Service* temp = head;
    while (temp) {
        Service* node = new Service(*temp);
        node->next = NULL;

        if (!newHead) {
            newHead = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        temp = temp->next;
    }
    return newHead;
}

void paTience_sorTing(Service*& list, bool byUrgency) {
    if (!list) return;

    for (Service* i = list; i; i = i->next) {
        for (Service* j = i->next; j; j = j->next) {

            bool swapCond = false;

            if (byUrgency) {
                if (j->urgency > i->urgency) swapCond = true;
                else if (j->urgency == i->urgency &&
                         dateToInt(j->tanggal) < dateToInt(i->tanggal))
                    swapCond = true;
            } else {
                if (dateToInt(j->tanggal) < dateToInt(i->tanggal))
                    swapCond = true;
            }

            if (swapCond) {
                swap(*i, *j);
            }
        }
    }
}

void viewQueue() {
    Service* temp;

    char pilih;
    do {
        cout << "\n[D]ate [U]rgency [Q]ueue [E]xit : ";
        cin >> pilih;

        if (pilih == 'Q') {
            display(head);
        } else {
            temp = copyList();
            if (pilih == 'D') paTience_sorTing(temp, false);
            else if (pilih == 'U') paTience_sorTing(temp, true);
            display(temp);
        }

    } while (pilih != 'E');
}


void doneService() {
    string montirList[] = {"Suby", "Farhan", "Dimas", "Aldo", "Rafi"};
    int pilihan;

    cout << "====== Jobs Done ======\n";
    cout << "Pilih Montir!\n";
    for (int i = 0; i < 5; i++) {
        cout << i+1 << ". " << montirList[i] << endl;
    }

    cout << "Pilihan: ";
    cin >> pilihan;

    if (pilihan < 1 || pilihan > 5) {
        cout << "Pilihan tidak valid!\n";
        return;
    }

    string montir = montirList[pilihan - 1];

    Service *temp = head, *target = NULL, *prev = NULL, *prevTarget = NULL;

    while (temp) {
        if (temp->montir == montir) {
            if (!target ||
                temp->urgency > target->urgency ||
                (temp->urgency == target->urgency &&
                 dateToInt(temp->tanggal) < dateToInt(target->tanggal))) {

                target = temp;
                prevTarget = prev;
            }
        }
        prev = temp;
        temp = temp->next;
    }

    if (!target) {
        cout << "Tidak ada service untuk montir ini\n";
        return;
    }

    cout << "\n====== Service ======\n";
    cout << "Model: " << target->model << endl;
    cout << "Merek: " << target->merek << endl;
    cout << "Kendala: " << target->kendala << endl;
    cout << "Montir: " << target->montir << endl;
    cout << "Tanggal: " << target->tanggal << endl;
    cout << "Urgency: " << target->urgency << endl;
    cout << "Pelanggan: " << target->pelanggan << endl;

    string confirm;
    cout << "Apakah servis ini sudah selesai? (yes/no): ";
    cin >> confirm;

    if (confirm == "yes") {
        if (!prevTarget) head = target->next;
        else prevTarget->next = target->next;

        delete target;
        saveToFile();

        cout << "Service selesai!\n";
    }
}

void rescheduleService() {
    if (!head) {
        cout << "Tidak ada service\n";
        return;
    }

    cout << "====== Ganti Tanggal Ambil ======\n";

    Service* temp = head;
    int i = 1;

    // tampilkan list
    while (temp) {
        cout << i << ". Servis:\n";
        cout << "Model Mobil: " << temp->model << endl;
        cout << "Merek Mobil: " << temp->merek << endl;
        cout << "Kendala: " << temp->kendala << endl;
        cout << "Montir: " << temp->montir << endl;
        cout << "Tanggal Ambil: " << temp->tanggal << endl;
        cout << "----------------------\n";
        temp = temp->next;
        i++;
    }

    int pilih;
    cout << "Pilih servis yang ingin di reschedule: ";
    cin >> pilih;

    temp = head;
    for (int j = 1; j < pilih && temp; j++) {
        temp = temp->next;
    }

    if (!temp) {
        cout << "Pilihan tidak valid!\n";
        return;
    }

    cout << "====== Ganti Tanggal Ambil ======\n";
    cout << "Model Mobil: " << temp->model << endl;
    cout << "Merek Mobil: " << temp->merek << endl;
    cout << "Kendala: " << temp->kendala << endl;
    cout << "Montir: " << temp->montir << endl;
    cout << "Tanggal Ambil: " << temp->tanggal << endl;

    string tanggalBaru;
    
    while (true) {
        cout << "Masukkan Tanggal baru (dd-mm-yy): ";
        cin >> tanggalBaru;

        if (isValidDate(tanggalBaru)) break;
            cout << "Tanggal tidak valid!\n";
    }

    string hasil = recursiveDate(tanggalBaru, 0);
    cout << "Tanggal ambil yang baru adalah " << tanggalBaru << endl;
    temp->tanggal = tanggalBaru;
    cout << "Servis " << temp->model << " berhasil di undur!!\n";

    saveToFile();
}

int main() {
    loadFromFile();

    int pilih;
    do {
        cout << "\n1. Tambah Service\n";
        cout << "2. Lihat Antrian\n";
        cout << "3. Selesaikan Service\n";
        cout << "4. Reschedule Service\n";
        cout << "5. Exit\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: addService(); break;
            case 2: viewQueue(); break;
            case 3: doneService(); break;
            case 4: rescheduleService(); break;
        }

    } while (pilih != 5);

    return 0;
}