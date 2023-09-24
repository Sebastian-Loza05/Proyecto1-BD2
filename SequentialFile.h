#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>
#include <filesystem>
#include <cmath>
#include "Structures/Record.h"
#include "Structures/methods.h"

using namespace std;

struct Entry {
    Record record;
    long nextPF;
    bool isMain=false;
    Entry(){}
    
    Entry(Record record_){
      this->record = record_;
    }

    void setData(){
        cout<<"ID:";
        cin>>record.key;
        cout<<"Nombre:";
        cin>>record.nombre;
        cout<<"Producto:";
        cin>>record.producto;
        cout<<"Precio:";
        cin>>record.precio;
        cout<<"Cantidad:";
        cin>>record.cantidad;
    }

    // void showData(){
    //         cout<<id<<" - "<<nombre<<" - "<<producto<<" - "<<precio<<" - "<<cantidad<<endl;
    // }
};

template <typename T>
class SequentialFile: public MethodSelector {
private:
    string mainFilename;
    string auxFilename;

public:
    SequentialFile() {
        this->mainFilename = "sequential_datos.dat";
        this->auxFilename = "sequential_aux.dat";
        // ofstream main(this->mainFilename, ios::binary | ios::app);
        ofstream aux(this->auxFilename, ios::binary | ios::app);
        // main.close();
        aux.close();

        ifstream mainFile(this->mainFilename, ios::binary);
        if (!mainFile) {
            ofstream newMainFile(this->mainFilename, ios::binary);
            int nextPF = 0;
            bool isMain = true;
            newMainFile.write((char*)&nextPF, sizeof(int));
            newMainFile.write((char*)&isMain, sizeof(bool));
            newMainFile.close();
        }
    }

    SequentialFile(string mainFilename, string auxFilename) {
        this->mainFilename = mainFilename;
        this->auxFilename = auxFilename;

        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile) {
            ofstream newMainFile(mainFilename, ios::binary);
            int nextPF = 0;
            bool isMain = true;
            newMainFile.write((char*)&nextPF, sizeof(int));
            newMainFile.write((char*)&isMain, sizeof(bool));
            newMainFile.close();
        }
    }

    bool add(Record Nuevorecord) override {
        Entry NuevoEntry(Nuevorecord); 

        ifstream auxp(auxFilename, ios::binary | ios::app);

        ifstream mainp(mainFilename, ios::binary);
        if (!mainp.is_open()) {
          return false;
        }
        auxp.seekg(0,ios::end);
        int SizeAux=auxp.tellg()/sizeof(Entry);

        mainp.seekg(0,ios::end);
        int SizeMain=((int)mainp.tellg() - sizeof(int) - sizeof(bool))/sizeof(Entry);

        auxp.close();
        mainp.close();

        ifstream auxFile(auxFilename, ios::binary | ios::app);
        auxFile.seekg(0,ios::end);


        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) {
          return false;
        }

        int HnextPF;
        bool HisMain;
        mainFile.read((char*)&HnextPF, sizeof(int));
        mainFile.read((char*)&HisMain, sizeof(bool));

        Entry FirstEntry;
        mainFile.read((char*)&FirstEntry, sizeof(Entry));

        if ( menor_igual(NuevoEntry.record.key, FirstEntry.record.key) ) {
        // if(FirstEntry.record.key>=NuevoEntry.record.key){ // FirstEntry.record.key   NuevoEntry <=

            if(HisMain){
                NuevoEntry.nextPF=HnextPF;
                NuevoEntry.isMain=HisMain;
                HnextPF=SizeAux;
                HisMain=false;

                ofstream auxFile2(auxFilename, ios::binary | ios::app);
                auxFile2.write((char*)&NuevoEntry, sizeof(Entry));
                auxFile2.close();

                ofstream mainFile(mainFilename, ios::binary | ios::in | ios::out);
                mainFile.write((char*)&HnextPF, sizeof(int));
                mainFile.write((char*)&HisMain, sizeof(bool));
                mainFile.close();
            }

            else{
                Entry PrevioRecord;
                ifstream auxFile(auxFilename, ios::binary | ios::app);
                auxFile.seekg(HnextPF*sizeof(Entry));
                auxFile.read((char*)&PrevioRecord,sizeof(Entry));
                if ( menor(NuevoEntry.record.key, PrevioRecord.record.key) ) {
                // if(PrevioRecord.record.key>NuevoEntry.record.key){
                    NuevoEntry.nextPF=HnextPF;
                    NuevoEntry.isMain=false;
                    HnextPF=SizeAux;
                }
                else{
                    int posPrevio=HnextPF;
                    Entry SPrevioRecord;
                    auxFile.seekg(PrevioRecord.nextPF*sizeof(Entry));
                    auxFile.read((char*)&SPrevioRecord,sizeof(Entry));
                    while ( menor_igual(SPrevioRecord.record.key, NuevoEntry.record.key) and SPrevioRecord.isMain ) {
                    // while(SPrevioRecord.record.key<=NuevoEntry.record.key and SPrevioRecord.isMain){
                        posPrevio=PrevioRecord.nextPF;
                        PrevioRecord=SPrevioRecord;
                        auxFile.seekg(SPrevioRecord.nextPF*sizeof(Entry));
                        auxFile.read((char*)&SPrevioRecord,sizeof(Entry));
                    }
                    NuevoEntry.nextPF=PrevioRecord.nextPF;
                    NuevoEntry.isMain=PrevioRecord.isMain;
                    PrevioRecord.nextPF=SizeAux;
                    PrevioRecord.isMain=false;
                    ofstream auxFile3(auxFilename, ios::binary | ios::in | ios::out);
                    auxFile3.seekp(posPrevio*sizeof(Entry));
                    auxFile3.write((char*)&PrevioRecord, sizeof(Entry));
                    auxFile3.close();
                }

                ofstream mainFile(mainFilename, ios::binary | ios::in | ios::out);
                mainFile.write((char*)&HnextPF, sizeof(int));
                mainFile.write((char*)&HisMain, sizeof(bool));
                mainFile.close();

                ofstream auxFile3(auxFilename, ios::binary | ios::app);
                auxFile3.write((char*)&NuevoEntry, sizeof(Entry));
                auxFile3.close();
            }
            return true;
        }

        mainFile.seekg(0, ios::end);

        if ((int)mainFile.tellg() <= sizeof(int) + sizeof(bool)) {
            NuevoEntry.nextPF = -1;
            ofstream mainFile(mainFilename, ios::binary | ios::app);
            mainFile.write((char*)&NuevoEntry, sizeof(Entry));
            mainFile.close();
            return true;
        }

        else {

            int inicio = 0;
            mainFile.seekg(0, ios::end);
            int tamMain = mainFile.tellg() / sizeof(Entry);
            int fin=tamMain;
            mainFile.seekg(0, ios::beg);

            Entry record;
            Entry PrevRecord;
            int medio;
            int pos;
            bool lastMain=true;
            while (inicio <= fin) {
                medio = (inicio + fin) / 2;
                mainFile.seekg(medio * sizeof(Entry) + sizeof(int) + sizeof(bool));
                mainFile.read((char *) &record, sizeof(Entry));
                if ( menor(record.record.key, NuevoEntry.record.key) ) {
                // if(record.record.key<NuevoEntry.record.key){
                    inicio = medio + 1;
                    pos=medio;
                    PrevRecord=record;
                }
                else{
                    fin = medio - 1;
                    pos=medio-1;
                }
                if ( igual_igual(record.record.key, NuevoEntry.record.key) ) {
                // if(record.record.key==NuevoEntry.record.key){
                    PrevRecord=record;
                    pos=medio;
                    break;
                }
                if(pos>tamMain-1){
                    pos-=1;
                }
            }
            if(PrevRecord.nextPF==-2){
                pos-=1;
                mainFile.seekg(pos * sizeof(Entry) + sizeof(int) + sizeof(bool), ios::beg);
                mainFile.read((char *) &PrevRecord, sizeof(Entry));
            }

            while (!PrevRecord.isMain and menor(PrevRecord.record.key, NuevoEntry.record.key)) {
            // while(!PrevRecord.isMain and PrevRecord.record.key<NuevoEntry.record.key){
                lastMain=false;
                ifstream auxFile1(auxFilename, ios::binary | ios::app);
                auxFile1.seekg(PrevRecord.nextPF*sizeof(Entry));
                auxFile1.read((char*)&record, sizeof(Entry));
                auxFile1.close();
                if ( menor_igual(record.record.key, NuevoEntry.record.key) ) {
                // if(record.record.key<=NuevoEntry.record.key){
                    pos=PrevRecord.nextPF;
                    PrevRecord=record;
                }
                else{
                    break;
                }
            }
            NuevoEntry.nextPF=PrevRecord.nextPF;
            NuevoEntry.isMain=PrevRecord.isMain;
            PrevRecord.nextPF=SizeAux;
            PrevRecord.isMain=false;
            if(lastMain){
                ofstream mainFile2(mainFilename, ios::binary | ios::in | ios::out);
                mainFile2.seekp(pos * sizeof(Entry)+sizeof(int)+sizeof(bool),ios::beg);
                mainFile2.write((char*)&PrevRecord, sizeof(Entry));
                mainFile2.close();
            } else{
                ofstream auxFile2(auxFilename, ios::binary | ios::in | ios::out);
                auxFile2.seekp(pos * sizeof(Entry),ios::beg);
                auxFile2.write((char*)&PrevRecord, sizeof(Entry));
                auxFile2.close();
            }
            ofstream auxFile2(auxFilename, ios::binary | ios::app);
            auxFile2.write((char*)&NuevoEntry, sizeof(Entry));
            auxFile2.close();
        }

        auxFile.close();
        mainFile.close();

        if(SizeAux+1>int(log(SizeMain))){
            merge(mainFilename,auxFilename);
        }
        return true;
    }


    void merge(string &mainFilename, string &auxFilename) {
        ofstream outputFile("merge.dat", ios::binary);
        if (!outputFile.is_open()) {
          return;
        }


        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) {
          return;
        }

        ifstream auxFile(auxFilename, ios::binary);
        if (!auxFile.is_open()) {
            int HnextPF;
            bool HisMain;
            mainFile.read((char*)&HnextPF, sizeof(int));
            mainFile.read((char*)&HisMain, sizeof(bool));

            outputFile.write((char*)&HnextPF, sizeof(int));
            outputFile.write((char*)&HisMain, sizeof(bool));

            int nextPos = HnextPF;
            int contador = 0;

            while(nextPos != -1){
                Entry record;
                Entry temp;

                mainFile.seekg(nextPos * sizeof(Entry) + sizeof(int) + sizeof(bool));
                mainFile.read((char*) &record, sizeof(Entry));

                if (!record.isMain){
                        temp = record;
                        temp.isMain = true;
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        temp.nextPF = contador;
                        outputFile.write((char*) &temp, sizeof(Entry));
                }
                else{
                    contador++;
                    if (record.nextPF == -1){contador = -1;}
                    int tempnext = record.nextPF;
                    record.nextPF = contador;
                    outputFile.write((char*) &record, sizeof(Entry));
                    record.nextPF = tempnext;
                    }

                nextPos = record.nextPF;
            }
        }
        else{
            int HnextPF;
            bool HisMain;
            mainFile.read((char*)&HnextPF, sizeof(int));
            mainFile.read((char*)&HisMain, sizeof(bool));

            outputFile.write((char*)&HnextPF, sizeof(int));
            outputFile.write((char*)&HisMain, sizeof(bool));

            int nextPos = HnextPF;
            bool Whcfile = HisMain;
            int contador = 0;
            while(nextPos != -1){
                Entry record;
                Entry temp;
                if (Whcfile){
                    mainFile.seekg(nextPos * sizeof(Entry) + sizeof(int) + sizeof(bool));
                    mainFile.read((char*) &record, sizeof(Entry));

                    if (!record.isMain){
                        temp = record;
                        temp.isMain = true;
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        temp.nextPF = contador;
                        outputFile.write((char*) &temp, sizeof(Entry));
                    }
                    else{
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        int tempnext = record.nextPF;
                        record.nextPF = contador;
                        outputFile.write((char*) &record, sizeof(Entry));
                        record.nextPF = tempnext;
                    }
                }
                else {
                    auxFile.seekg(nextPos * sizeof(Entry));
                    auxFile.read((char*) &record, sizeof(Entry));

                    if (!record.isMain){
                        temp = record;
                        temp.isMain = true;
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        temp.nextPF = contador;
                        outputFile.write((char*) &temp, sizeof(Entry));
                    }
                    else {
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        int tempnext = record.nextPF;
                        record.nextPF = contador;
                        outputFile.write((char*) &record, sizeof(Entry));
                        record.nextPF = tempnext;
                    }
                }
                nextPos = record.nextPF;
                Whcfile = record.isMain;
            }
        }

        mainFile.close();
        auxFile.close();
        outputFile.close();

        std::remove(mainFilename.c_str());
        std::remove(auxFilename.c_str());
        std::rename("merge.dat", mainFilename.c_str());

        fstream main(mainFilename, ios::binary | ios::in | ios::out);
        if (!main.is_open()) {
          return;
        }

        int HnextPF = 0;
        int HisMain = 1;
        main.write((char*)&HnextPF, sizeof(int));
        main.write((char*)&HisMain, sizeof(bool));

        main.close();
    }

    bool remove(T key) override {
        merge(mainFilename,auxFilename);

        fstream mainFile(mainFilename, ios::binary| ios::in | ios::out);
        if (!mainFile.is_open()) {
          return false;
        }

        int inicio = 0;
        mainFile.seekg(0, ios::end);
        int fin = ((int)mainFile.tellg() - inicio) / sizeof(Entry) - 1;
        mainFile.seekg(0, ios::beg);

        Entry record;
        Entry PrevRecord;
        int pos;
        while (inicio <= fin) {
            int medio = (inicio + fin) / 2;
            mainFile.seekg(medio * sizeof(Entry) + sizeof(int) + sizeof(bool));
            mainFile.read((char*)&record, sizeof(Entry));

            if ( igual_igual(record.record.key, key) ) {
            // if (record.record.key == key) {
                pos = medio;
                break;
            }
            else if ( menor(record.record.key, key) ) {
            // else if (record.record.key < key) {
                inicio = medio + 1;
                if ( menor(PrevRecord.record.key, record.record.key) ) { // 
                // if (key- record.record.key < key - PrevRecord.record.key) { //  record.key  > prevredc
                    PrevRecord = record;
                }
            } else {
                fin = medio - 1;
            }

            pos = -4;
        }

        if (pos == -4){
            cout<<"No existe el ID"<<endl;
            return false;
        }

        if (pos == 0){
            int HnextPF;
            HnextPF = 1;
            mainFile.seekp(0, ios::beg);
            mainFile.write((char*)&HnextPF, sizeof(int));

            Entry rmrecord;
            mainFile.seekg(pos * sizeof(Entry) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.read((char*) &rmrecord, sizeof(Entry));

            rmrecord.nextPF = -2;

            mainFile.seekp(pos * sizeof(Entry) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.write((char*) &rmrecord, sizeof(Entry));
        }

        else{
            Entry prevrecord;
            mainFile.seekg((pos-1) * sizeof(Entry) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.read((char*) &prevrecord, sizeof(Entry));

            Entry rmrecord;
            mainFile.seekg(pos * sizeof(Entry) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.read((char*) &rmrecord, sizeof(Entry));

            prevrecord.nextPF = rmrecord.nextPF;
            rmrecord.nextPF = -2;

            mainFile.seekp((pos-1) * sizeof(Entry) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.write((char*) &prevrecord, sizeof(Entry));

            mainFile.seekp(pos * sizeof(Entry) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.write((char*) &rmrecord, sizeof(Entry));
        }
        mainFile.close();
        return true;
    }

    Record search(T key) override {
        vector<Entry> result;
        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) Record();

        int inicio = 0;
        mainFile.seekg(0, ios::end);
        int fin = ((int)mainFile.tellg() - inicio) / sizeof(Entry) - 1;
        mainFile.seekg(0, ios::beg);

        Entry record;
        Entry PrevRecord;

        while (inicio <= fin) {
            int medio = (inicio + fin) / 2;
            mainFile.seekg(medio * sizeof(Entry) + sizeof(int) + sizeof(bool));
            mainFile.read((char*)&record, sizeof(Entry));

            if ( igual_igual(record.record.key, key) ) {
            // if (record.record.key == key) {
                if (record.nextPF == -2){
                    cout << "El Entry se encuentra eliminado" <<endl;
                    return Record();
                }
                else{
                    result.push_back(record);
                    return result[0].record;
                }
            } 
            else if ( menor(record.record.key, key) ) {
            // else if (record.record.key < key) {
                inicio = medio + 1;
                if ( menor(PrevRecord.record.key, record.record.key) ) {
                // if (key - record.record.key < key - PrevRecord.record.key) { // -recr < -prev  rec > prev
                    PrevRecord = record;
                }
            } else {
                fin = medio - 1;
            }
        }

        ifstream auxFile(auxFilename, ios::binary);
        if (!auxFile.is_open()) throw runtime_error("No se pudo abrir el archivo auxiliar");

        while(auxFile.read((char*)&record, sizeof(Entry))){
            if ( igual_igual(PrevRecord.record.key, key) ) {
            // if (PrevRecord.record.key == key) {
                result.push_back(PrevRecord);
                return result[0].record;
            }
        }

        cout << "No se encontro el ID" << endl;

        mainFile.close();
        auxFile.close();
        return result[0].record;
    }

    vector<Entry> rangeSearch(T beginkey, T endkey) override {
        vector<Entry> result;
        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) throw runtime_error("No se pudo abrir el archivo main");

        ifstream auxFile(auxFilename, ios::binary);
        if (!auxFile.is_open()) throw runtime_error("No se pudo abrir el archivo auxiliar");

        Entry record;
        int inicio = 0;
        mainFile.seekg(0, ios::end);
        int fin = ((int)mainFile.tellg() - inicio) / sizeof(Entry) - 1;
        mainFile.seekg(0, ios::beg);

        while (inicio <= fin) {
            int medio = (inicio + fin) / 2;
            mainFile.seekg(medio * sizeof(Entry) + sizeof(int) + sizeof(bool));
            mainFile.read((char*)&record, sizeof(Entry));
            
            if ( menor_igual(beginkey, record.record.key) ) {
            // if (record.record.key >= beginkey) { // beginkey =< record
                fin = medio - 1;
            } else {
                inicio = medio + 1;
            }
        }

        bool found = false;

        while ( menor_igual(record.record.key, endkey) ) {
        // while (record.record.key <= endkey) {
            if ( found && menor(record.record.key, beginkey) ){
            // if (found && record.record.key < beginkey) {
                break;
            }
            if ( menor_igual(beginkey, record.record.key) ) {
            // if (record.record.key >= beginkey) {
                result.push_back(record);
                found = true;
            }
            if(record.nextPF==-1)break;
            if (record.isMain) {
                mainFile.seekg(record.nextPF * sizeof(Entry)  + sizeof(int) + sizeof(bool));
                mainFile.read((char*)&record, sizeof(Entry));
            } else {
                auxFile.seekg(record.nextPF * sizeof(Entry));
                auxFile.read((char*)&record, sizeof(Entry));

            }
        }

        mainFile.close();
        auxFile.close();
        return result;
    }

    vector<Record> load() override {
        merge(mainFilename,auxFilename);

        ifstream file(mainFilename, ios::binary);
        if(!file.is_open()) throw ("No se pudo abrir el archivo");
        vector<Record> result;
        Entry record;

        file.seekg(sizeof(int) + sizeof(bool), ios::beg);
        while(file.peek() != EOF){
            record = Entry();
            file.read((char*) &record, sizeof(Entry));
            if (record.nextPF != -2) {
                result.push_back(record.record);
            }
        }
        file.close();
        return result;
    }
};
