#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Customer{
    string nama;
    string telp;
};

struct Service{
    string modelMobil;
    string merekMobil;
    string kendala;
    string montir;
    string tanggalMasuk;
    Customer dataCustomer;
    Service* next;
};

Service* frontQueue=NULL;
Service* rearQueue=NULL;
Service* doneHead=NULL;

bool customerExists(string nama){

    ifstream file("cust.txt");

    if(!file.is_open()) return false;

    string namaFile,telp;

    while(getline(file,namaFile,'|')){
        getline(file,telp);

        if(namaFile==nama){
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

string getCustomerPhone(string nama){

    ifstream file("cust.txt");

    if(!file.is_open()) return "";

    string namaFile,telp;

    while(getline(file,namaFile,'|')){
        getline(file,telp);

        if(namaFile==nama){
            file.close();
            return telp;
        }
    }

    file.close();
    return "";
}

void saveCustomer(Customer data){

    ofstream file("cust.txt",ios::app);

    file<<data.nama<<"|"<<data.telp<<endl;

    file.close();
}

void enqueueService(Service* baru){

    if(frontQueue==NULL){
        frontQueue=rearQueue=baru;
    }
    else{
        rearQueue->next=baru;
        rearQueue=baru;
    }
}

void addDone(Service* node){

    node->next=doneHead;
    doneHead=node;
}

void savePending(Service* data){

    ofstream file("pending.txt",ios::app);

    file<<data->modelMobil<<"|"
        <<data->merekMobil<<"|"
        <<data->kendala<<"|"
        <<data->montir<<"|"
        <<data->tanggalMasuk<<"|"
        <<data->dataCustomer.nama<<"|"
        <<data->dataCustomer.telp
        <<endl;

    file.close();
}

void saveDone(Service* data){

    ofstream file("done.txt",ios::app);

    file<<data->modelMobil<<"|"
        <<data->merekMobil<<"|"
        <<data->kendala<<"|"
        <<data->montir<<"|"
        <<data->tanggalMasuk<<"|"
        <<data->dataCustomer.nama<<"|"
        <<data->dataCustomer.telp
        <<endl;

    file.close();
}

void rewritePending(){

    ofstream file("pending.txt");

    Service* scanner=frontQueue;

    while(scanner!=NULL){

        file<<scanner->modelMobil<<"|"
            <<scanner->merekMobil<<"|"
            <<scanner->kendala<<"|"
            <<scanner->montir<<"|"
            <<scanner->tanggalMasuk<<"|"
            <<scanner->dataCustomer.nama<<"|"
            <<scanner->dataCustomer.telp
            <<endl;

        scanner=scanner->next;
    }

    file.close();
}

void loadPending(){

    ifstream file("pending.txt");

    if(!file.is_open()) return;

    while(true){

        Service* baru=new Service();

        if(!getline(file,baru->modelMobil,'|')) break;

        getline(file,baru->merekMobil,'|');
        getline(file,baru->kendala,'|');
        getline(file,baru->montir,'|');
        getline(file,baru->tanggalMasuk,'|');
        getline(file,baru->dataCustomer.nama,'|');
        getline(file,baru->dataCustomer.telp);

        baru->next=NULL;

        enqueueService(baru);
    }

    file.close();
}

void loadDone(){

    ifstream file("done.txt");

    if(!file.is_open()) return;

    while(true){

        Service* baru=new Service();

        if(!getline(file,baru->modelMobil,'|')) break;

        getline(file,baru->merekMobil,'|');
        getline(file,baru->kendala,'|');
        getline(file,baru->montir,'|');
        getline(file,baru->tanggalMasuk,'|');
        getline(file,baru->dataCustomer.nama,'|');
        getline(file,baru->dataCustomer.telp);

        baru->next=doneHead;
        doneHead=baru;
    }

    file.close();
}

void pelangganBaru(){

    Customer data;

    cout<<"====== Pelanggan Baru ======\n";

    cout<<"Nama Pelanggan: ";
    getline(cin,data.nama);

    if(customerExists(data.nama)){
        cout<<"Pelanggan sudah ada!\n";
        cout<<"Press enter...";
        cin.get();
        return;
    }

    cout<<"No Telp: ";
    getline(cin,data.telp);

    saveCustomer(data);

    cout<<"Pelanggan berhasil ditambahkan!\n";
    cout<<"Press enter...";
    cin.get();
}

void servisBaru(){

    Service* baru=new Service();
    baru->next=NULL;

    cout<<"====== Servis Baru ======\n";

    cout<<"Model Mobil: ";
    getline(cin,baru->modelMobil);

    cout<<"Merek Mobil: ";
    getline(cin,baru->merekMobil);

    cout<<"Kendala: ";
    getline(cin,baru->kendala);

    cout<<"Montir: ";
    getline(cin,baru->montir);

    cout<<"Nama Pelanggan: ";
    getline(cin,baru->dataCustomer.nama);

    if(customerExists(baru->dataCustomer.nama)){

        // jika pelanggan sudah ada
        baru->dataCustomer.telp=getCustomerPhone(baru->dataCustomer.nama);

        cout<<"Pelanggan sudah terdaftar.\n";
        cout<<"No Telp: "<<baru->dataCustomer.telp<<endl;
    }
    else{

        // pelanggan baru
        cout<<"Pelanggan belum terdaftar.\n";
        cout<<"Masukkan No Telp: ";
        getline(cin,baru->dataCustomer.telp);

        Customer data;
        data.nama=baru->dataCustomer.nama;
        data.telp=baru->dataCustomer.telp;

        saveCustomer(data);

        cout<<"Pelanggan baru berhasil disimpan.\n";
    }

    cout<<"Tanggal Masuk: ";
    getline(cin,baru->tanggalMasuk);

    enqueueService(baru);
    savePending(baru);

    cout<<"Servis berhasil dicatat!\n";
    cout<<"Press enter...";
    cin.get();
}

void tampilAntrian(){

    Service* scanner=frontQueue;

    cout<<"====== All Services ======\n";

    if(scanner==NULL)
        cout<<"Tidak ada antrian servis.\n";

    while(scanner!=NULL){

        cout<<"-----------------------\n";
        cout<<"Model Mobil: "<<scanner->modelMobil<<endl;
        cout<<"Merek Mobil: "<<scanner->merekMobil<<endl;
        cout<<"Kendala: "<<scanner->kendala<<endl;
        cout<<"Montir: "<<scanner->montir<<endl;
        cout<<"Tanggal Masuk: "<<scanner->tanggalMasuk<<endl;
        cout<<"Nama Pelanggan: "<<scanner->dataCustomer.nama<<endl;
        cout<<"No Telp: "<<scanner->dataCustomer.telp<<endl;

        scanner=scanner->next;
    }

    cout<<"Press enter...";
    cin.get();
}

void selesaiServis(){

    string montir;

    cout<<"====== Jobs Done ======\n";
    cout<<"Nama Montir: ";
    getline(cin,montir);

    Service* scanner=frontQueue;
    Service* prev=NULL;

    while(scanner!=NULL){

        if(scanner->montir==montir){

            cout<<"Model Mobil: "<<scanner->modelMobil<<endl;

            string jawab;

            cout<<"Apakah servis selesai? (yes/no): ";
            getline(cin,jawab);

            if(jawab=="yes"){

                if(prev==NULL)
                    frontQueue=scanner->next;
                else
                    prev->next=scanner->next;

                if(scanner==rearQueue)
                    rearQueue=prev;

                addDone(scanner);
                saveDone(scanner);
                rewritePending();
            }

            break;
        }

        prev=scanner;
        scanner=scanner->next;
    }

    cout<<"Press enter...";
    cin.get();
}

void riwayatCustomer(){

    string nama;

    cout<<"====== Riwayat Servis Anda ======\n";
    cout<<"Masukkan Nama: ";
    getline(cin,nama);

    cout<<"====== Services =======\n";

    bool ditemukan=false;

    Service* scanner=frontQueue;

    // cek servis yang masih pending
    while(scanner!=NULL){

        if(scanner->dataCustomer.nama==nama){

            ditemukan=true;

            cout<<"Model Mobil: "<<scanner->modelMobil<<endl;
            cout<<"Merek Mobil: "<<scanner->merekMobil<<endl;
            cout<<"Kendala: "<<scanner->kendala<<endl;
            cout<<"Montir: "<<scanner->montir<<endl;
            cout<<"Tanggal Masuk: "<<scanner->tanggalMasuk<<endl;
            cout<<"Nama Pelanggan: "<<scanner->dataCustomer.nama<<endl;
            cout<<"No Telp Pelanggan: "<<scanner->dataCustomer.telp<<endl;
            cout<<"=========================\n";
        }

        scanner=scanner->next;
    }

    // cek servis yang sudah selesai
    scanner=doneHead;

    while(scanner!=NULL){

        if(scanner->dataCustomer.nama==nama){

            ditemukan=true;

            cout<<"Model Mobil: "<<scanner->modelMobil<<endl;
            cout<<"Merek Mobil: "<<scanner->merekMobil<<endl;
            cout<<"Kendala: "<<scanner->kendala<<endl;
            cout<<"Montir: "<<scanner->montir<<endl;
            cout<<"Tanggal Masuk: "<<scanner->tanggalMasuk<<endl;
            cout<<"Nama Pelanggan: "<<scanner->dataCustomer.nama<<endl;
            cout<<"No Telp Pelanggan: "<<scanner->dataCustomer.telp<<endl;
            cout<<"=========================\n";
        }

        scanner=scanner->next;
    }

    if(!ditemukan){
        cout<<"Tidak ada riwayat servis.\n";
    }

    cout<<"Press any key to go back ...";
    cin.get();
}

void menuServis(){

    cout<<"====== Services ======\n";
    cout<<"1. Semua Servis\n";
    cout<<"2. Servis Baru\n";
    cout<<"3. Selesaikan Servis\n";
    cout<<"Pilihan: ";

    int pilih;
    cin>>pilih;
    cin.ignore();

    if(pilih==1) tampilAntrian();
    else if(pilih==2) servisBaru();
    else if(pilih==3) selesaiServis();
}

int main(){

    loadPending();
    loadDone();

    while(true){

        cout<<"====== Welcome to Lognuts ======\n";
        cout<<"1. Antrian Servis\n";
        cout<<"2. Riwayat Servis Anda\n";
        cout<<"3. Keluar\n";
        cout<<"Pilihan: ";

        string input;
        getline(cin,input);

        if(input=="1") tampilAntrian();
        else if(input=="2") riwayatCustomer();
        else if(input=="3") break;

        else if(input=="adminacces8008"){

            cout<<"====== Admin Menu ======\n";
            cout<<"1. Servis\n";
            cout<<"2. Pelanggan Baru\n";
            cout<<"3. Keluar\n";
            cout<<"Pilihan: ";

            int pilih;
            cin>>pilih;
            cin.ignore();

            if(pilih==1) menuServis();
            else if(pilih==2) pelangganBaru();
        }

        cout<<endl;
    }
}