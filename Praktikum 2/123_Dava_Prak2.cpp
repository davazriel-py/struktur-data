#include <iostream>
#include <string>
#include <cstdint>
#include <ctime>
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


string getTanggal(){

    time_t now=time(0);
    tm *ltm=localtime(&now);

    int d=ltm->tm_mday;
    int m=1+ltm->tm_mon;
    int y=1900+ltm->tm_year;

    string bulan[12]={"Januari","Februari","Maret","April","Mei","Juni","Juli","Agustus","September","Oktober","November","Desember"};

    return to_string(d)+"-"+bulan[m-1]+"-"+to_string(y);
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

    ofstream file("pending_service.txt",ios::app);

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

    ofstream file("done_service.txt",ios::app);

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

    ofstream file("pending_service.txt");

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

    ifstream file("pending_service.txt");

    if(!file.is_open()) return;

    while(!file.eof()){

        Service* baru=new Service();

        getline(file,baru->modelMobil,'|');
        if(file.eof()) break;

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

    ifstream file("done_service.txt");

    if(!file.is_open()) return;

    while(!file.eof()){

        Service* baru=new Service();

        getline(file,baru->modelMobil,'|');
        if(file.eof()) break;

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


void tampilAntrian(){

    Service* scanner=frontQueue;

    cout<<"====== All Services ======\n";

    while(scanner!=NULL){

        cout<<"-----------------------\n";
        cout<<"Model Mobil: "<<scanner->modelMobil<<endl;
        cout<<"Merek Mobil: "<<scanner->merekMobil<<endl;
        cout<<"Kendala: "<<scanner->kendala<<endl;
        cout<<"Montir: "<<scanner->montir<<endl;
        cout<<"Nama Pelanggan: "<<scanner->dataCustomer.nama<<endl;
        cout<<"No Telp Pelanggan: "<<scanner->dataCustomer.telp<<endl;

        scanner=scanner->next;
    }

    cout<<"Press any key to go back ...";
    cin.get();
}


void servisBaru(){

    Service* baru=new Service();
    baru->next=NULL;

    cout<<"====== New Service ======\n";

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

    cout<<"No Telp Pelanggan: ";
    getline(cin,baru->dataCustomer.telp);

    baru->tanggalMasuk=getTanggal();

    enqueueService(baru);
    savePending(baru);

    cout<<"*Servis berhasil dicatat*\n";
    cout<<"Press any key to go back ...";
    cin.get();
}


void selesaiServis(){

    string montir;

    cout<<"====== Jobs Done======\n";
    cout<<"Nama Montir: ";
    getline(cin,montir);

    Service* scanner=frontQueue;
    Service* prev=NULL;

    while(scanner!=NULL){

        if(scanner->montir==montir){

            cout<<"====== Service ======\n";
            cout<<"Model Mobil: "<<scanner->modelMobil<<endl;
            cout<<"Merek Mobil: "<<scanner->merekMobil<<endl;
            cout<<"Kendala: "<<scanner->kendala<<endl;
            cout<<"Montir: "<<scanner->montir<<endl;

            string jawab;

            cout<<"Apakah servis ini sudah selesai?(yes/no): ";
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
}


void riwayatCustomer(){

    string nama;

    cout<<"====== Riwayat Servis Anda ======\n";
    cout<<"Masukkan Nama: ";
    getline(cin,nama);

    Service* scanner=doneHead;

    cout<<"====== Services =======\n";

    while(scanner!=NULL){

        if(scanner->dataCustomer.nama==nama){

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

    cout<<"Press any key to go back ...";
    cin.get();
}


void menuServis(){

    cout<<"====== Services ======\n";
    cout<<"1. Semua Servis Singkat\n";
    cout<<"2. Servis Baru\n";
    cout<<"3. Selesaikan Pekerjaan\n";
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
        cout<<"Pilih menu!\n";
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

            cout<<"====== Welcome to Lognuts ======\n";
            cout<<"1. Servis\n";
            cout<<"2. Pelanggan Baru\n";
            cout<<"3. Keluar\n";
            cout<<"Pilihan: ";

            int pilih;
            cin>>pilih;
            cin.ignore();

            if(pilih==1) menuServis();
            else if(pilih==2) servisBaru();
        }

        cout<<endl;
    }
}