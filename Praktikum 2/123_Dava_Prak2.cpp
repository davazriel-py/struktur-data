#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Customer{
    string nama;
    string telp;
    string alamat;
    string umur;
    string gender;
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

string montirList[4]={"Suby","Farhan","Dimas","Aldo"};

bool cekCust(string nama){

    ifstream file("cust.txt");
    if(!file.is_open()) return false;

    string namaFile,telp,alamat,umur,gender;

    while(getline(file,namaFile,'|')){
        getline(file,telp,'|');
        getline(file,alamat,'|');
        getline(file,umur,'|');
        getline(file,gender);

        if(namaFile==nama){
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

string getData(string nama){

    ifstream file("cust.txt");
    if(!file.is_open()) return "";

    string namaFile,telp,alamat,umur,gender;

    while(getline(file,namaFile,'|')){
        getline(file,telp,'|');
        getline(file,alamat,'|');
        getline(file,umur,'|');
        getline(file,gender);

        if(namaFile==nama){
            file.close();
            return telp;
        }
    }

    file.close();
    return "";
}

void saveCust(Customer data){

    ofstream file("cust.txt",ios::app);
    file<<data.nama<<"|"
        <<data.telp<<"|"
        <<data.alamat<<"|"
        <<data.umur<<"|"
        <<data.gender<<endl;
    file.close();
}

void addQueue(Service* baru){

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
        addQueue(baru);
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

bool montirPending(string nama){

    Service* scanner=frontQueue;

    while(scanner!=NULL){
        if(scanner->montir==nama)
            return true;

        scanner=scanner->next;
    }

    return false;
}

void riwayatCust(){

    string nama;

    cout<<"====== Riwayat Servis Anda ======\n";
    cout<<"Masukkan Nama: ";
    getline(cin,nama);

    cout<<"====== Services =======\n";

    bool ditemukan=false;

    Service* scanner=frontQueue;

    while(scanner!=NULL){

        if(scanner->dataCustomer.nama==nama){

            ditemukan=true;

            cout<<"Model Mobil: "<<scanner->modelMobil<<endl;
            cout<<"Merek Mobil: "<<scanner->merekMobil<<endl;
            cout<<"Kendala: "<<scanner->kendala<<endl;
            cout<<"Montir: "<<scanner->montir<<endl;
            cout<<"Tanggal Masuk: "<<scanner->tanggalMasuk<<endl;
            cout<<"Nama Pelanggan: "<<scanner->dataCustomer.nama<<endl;
            cout<<"No Telp: "<<scanner->dataCustomer.telp<<endl;
            cout<<"=========================\n";
        }

        scanner=scanner->next;
    }

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
            cout<<"No Telp: "<<scanner->dataCustomer.telp<<endl;
            cout<<"=========================\n";
        }

        scanner=scanner->next;
    }

    if(!ditemukan){
        cout<<"Tidak ada riwayat servis.\n";
    }

    cout<<"Press enter...";
    cin.get();
}

void semuaServisSingkat(){

    cout<<"====== Semua Servis Singkat ======\n";

    Service* scanner=frontQueue;

    if(scanner==NULL)
        cout<<"Tidak ada servis pending.\n";

    while(scanner!=NULL){

        cout<<"-----------------------\n";
        cout<<"Model: "<<scanner->modelMobil<<endl;
        cout<<"Merek: "<<scanner->merekMobil<<endl;
        cout<<"Montir: "<<scanner->montir<<endl;
        cout<<"Customer: "<<scanner->dataCustomer.nama<<endl;

        scanner=scanner->next;
    }

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

    if(cekCust(baru->dataCustomer.nama)){

        baru->dataCustomer.telp=getData(baru->dataCustomer.nama);
        cout<<"No Telp: "<<baru->dataCustomer.telp<<endl;
    }
    else{

        cout<<"Masukkan No Telp: ";
        getline(cin,baru->dataCustomer.telp);

        cout<<"Masukkan Alamat: ";
        getline(cin,baru->dataCustomer.alamat);

        cout<<"Masukkan Umur: ";
        getline(cin,baru->dataCustomer.umur);

        cout<<"Masukkan Gender: ";
        getline(cin,baru->dataCustomer.gender);

        Customer data;
        data.nama=baru->dataCustomer.nama;
        data.telp=baru->dataCustomer.telp;
        data.alamat=baru->dataCustomer.alamat;
        data.umur=baru->dataCustomer.umur;
        data.gender=baru->dataCustomer.gender;

        saveCust(data);
    }

    cout<<"Tanggal Masuk: ";
    getline(cin,baru->tanggalMasuk);

    addQueue(baru);
    savePending(baru);

    cout<<"Servis berhasil dicatat!\n";
    cout<<"Press enter...";
    cin.get();
}

void selesaiServis(){

    cout<<"====== Selesaikan Pekerjaan ======\n";
    cout<<"Pilih Montir!\n";

    int nomor=1;

    for(int i=0;i<4;i++){
        if(montirPending(montirList[i])){
            cout<<nomor<<". "<<montirList[i]<<endl;
            nomor++;
        }
    }

    cout<<"Pilihan: ";

    int pilih;
    cin>>pilih;
    cin.ignore();

    string montir="";
    nomor=1;

    for(int i=0;i<4;i++){
        if(montirPending(montirList[i])){
            if(pilih==nomor)
                montir=montirList[i];
            nomor++;
        }
    }

    if(montir==""){
        cout<<"Pilihan tidak valid\n";
        return;
    }

    Service* scanner=frontQueue;
    Service* prev=NULL;

    while(scanner!=NULL){

        if(scanner->montir==montir){

            cout<<"-----------------------\n";
            cout<<"Model Mobil: "<<scanner->modelMobil<<endl;
            cout<<"Merek Mobil: "<<scanner->merekMobil<<endl;
            cout<<"Kendala: "<<scanner->kendala<<endl;
            cout<<"Montir: "<<scanner->montir<<endl;
            cout<<"Tanggal Masuk: "<<scanner->tanggalMasuk<<endl;
            cout<<"Nama Pelanggan: "<<scanner->dataCustomer.nama<<endl;
            cout<<"No Telp: "<<scanner->dataCustomer.telp<<endl;
            cout<<"-----------------------\n";

            string jawab;

            cout<<"Apakah servis selesai? (yes/no): ";
            getline(cin,jawab);

            if(jawab=="yes"){

                Service* selesaiNode=scanner;

                if(prev==NULL)
                    frontQueue=scanner->next;
                else
                    prev->next=scanner->next;

                if(scanner==rearQueue)
                    rearQueue=prev;

                scanner=scanner->next;

                addDone(selesaiNode);
                saveDone(selesaiNode);
                rewritePending();

                continue;
            }
        }

        prev=scanner;
        scanner=scanner->next;
    }

    cout<<"Press enter...";
    cin.get();
}

void tampilRiwayatMontir(string nama){

    cout<<"====== Riwayat Kerja "<<nama<<" ======\n";

    Service* scanner=frontQueue;

    while(scanner!=NULL){

        if(scanner->montir==nama){

            cout<<"Model Mobil: "<<scanner->modelMobil<<endl;
            cout<<"Customer: "<<scanner->dataCustomer.nama<<endl;
            cout<<"Status: Pending\n";
            cout<<"-----------------\n";
        }

        scanner=scanner->next;
    }

    scanner=doneHead;

    while(scanner!=NULL){

        if(scanner->montir==nama){

            cout<<"Model Mobil: "<<scanner->modelMobil<<endl;
            cout<<"Customer: "<<scanner->dataCustomer.nama<<endl;
            cout<<"Status: Done\n";
            cout<<"-----------------\n";
        }

        scanner=scanner->next;
    }

    cout<<"Press enter...";
    cin.get();
}

void riwayatKerjaMontir(){

    cout<<"====== Riwayat Kerja Montir ======\n";
    cout<<"Pilih Montir!\n";

    int nomor=1;

    for(int i=0;i<4;i++){

        if(montirPending(montirList[i])){
            cout<<nomor<<". "<<montirList[i]<<endl;
            nomor++;
        }
    }

    cout<<"Pilihan: ";

    int pilih;
    cin>>pilih;
    cin.ignore();

    nomor=1;

    for(int i=0;i<4;i++){

        if(montirPending(montirList[i])){

            if(pilih==nomor){
                tampilRiwayatMontir(montirList[i]);
                return;
            }

            nomor++;
        }
    }
}

void pelangganBaru(){

    Customer data;

    cout<<"====== Pelanggan Baru ======\n";

    cout<<"Nama Pelanggan: ";
    getline(cin,data.nama);

    if(cekCust(data.nama)){
        cout<<"Pelanggan sudah ada!\n";
        cin.get();
        return;
    }

    cout<<"No Telp: ";
    getline(cin,data.telp);

    saveCust(data);

    cout<<"Pelanggan berhasil ditambahkan!\n";
    cin.get();
}

void menuServis(){

    cout<<"====== Services ======\n";
    cout<<"1. Semua Servis Singkat\n";
    cout<<"2. Servis Baru\n";
    cout<<"3. Selesaikan Pekerjaan\n";
    cout<<"4. Riwayat Kerja Montir\n";
    cout<<"Pilihan: ";

    int pilih;
    cin>>pilih;
    cin.ignore();

    if(pilih==1) semuaServisSingkat();
    else if(pilih==2) servisBaru();
    else if(pilih==3) selesaiServis();
    else if(pilih==4) riwayatKerjaMontir();
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

        if(input=="1") semuaServisSingkat();
        else if(input=="2") riwayatCust();
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