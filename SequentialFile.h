#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>
#include <filesystem>
#include <cmath>

using namespace std;

struct Registro {
    int id;
    char nombre[10];
    char producto[10];
    float precio;
    int cantidad;
    long nextPF;
    bool isMain=false;
public:
    void setData(){
        cout<<"ID:";
        cin>>id;
        cout<<"Nombre:";
        cin>>nombre;
        cout<<"Producto:";
        cin>>producto;
        cout<<"Precio:";
        cin>>precio;
        cout<<"Cantidad:";
        cin>>cantidad;
    }
};

template <typename T>
class SequentialFile {
private:
    string mainFilename;
    string auxFilename;

public:
    SequentialFile() {
        this->mainFilename = "sequential_datos.dat";
        this->auxFilename = "sequential_aux.dat";

        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile) {
            ofstream newMainFile(mainFilename, ios::binary);
            if (!newMainFile.is_open()) {
                throw runtime_error("No se pudo crear el archivo main");
            }
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
            if (!newMainFile.is_open()) {
                throw runtime_error("No se pudo crear el archivo main");
            }
            int nextPF = 0;
            bool isMain = true;
            newMainFile.write((char*)&nextPF, sizeof(int));
            newMainFile.write((char*)&isMain, sizeof(bool));
            newMainFile.close();
        }
    }

    void add(Registro NuevoRegistro) {

        ifstream auxp(auxFilename, ios::binary | ios::app);

        ifstream mainp(mainFilename, ios::binary);
        if (!mainp.is_open()) {
            throw runtime_error("No se pudo abrir el archivo main");
        }
        auxp.seekg(0,ios::end);
        int SizeAux=auxp.tellg()/sizeof(Registro);

        mainp.seekg(0,ios::end);
        int SizeMain=((int)mainp.tellg() - sizeof(int) - sizeof(bool))/sizeof(Registro);

        auxp.close();
        mainp.close();

        ifstream auxFile(auxFilename, ios::binary | ios::app);
        auxFile.seekg(0,ios::end);


        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) {
            throw runtime_error("No se pudo abrir el archivo main");
        }

        int HnextPF;
        bool HisMain;
        mainFile.read((char*)&HnextPF, sizeof(int));
        mainFile.read((char*)&HisMain, sizeof(bool));

        Registro FirstRecord;
        mainFile.read((char*)&FirstRecord, sizeof(Registro));

        if(FirstRecord.id>=NuevoRegistro.id){

            if(HisMain){
                NuevoRegistro.nextPF=HnextPF;
                NuevoRegistro.isMain=HisMain;
                HnextPF=SizeAux;
                HisMain=false;

                ofstream auxFile2(auxFilename, ios::binary | ios::app);
                auxFile2.write((char*)&NuevoRegistro, sizeof(Registro));
                auxFile2.close();

                ofstream mainFile(mainFilename, ios::binary | ios::in | ios::out);
                mainFile.write((char*)&HnextPF, sizeof(int));
                mainFile.write((char*)&HisMain, sizeof(bool));
                mainFile.close();
            }

            else{
                Registro PrevioRecord;
                ifstream auxFile(auxFilename, ios::binary | ios::app);
                auxFile.seekg(HnextPF*sizeof(Registro));
                auxFile.read((char*)&PrevioRecord,sizeof(Registro));
                if(PrevioRecord.id>NuevoRegistro.id){
                    NuevoRegistro.nextPF=HnextPF;
                    NuevoRegistro.isMain=false;
                    HnextPF=SizeAux;
                }
                else{
                    int posPrevio=HnextPF;
                    Registro SPrevioRecord;
                    auxFile.seekg(PrevioRecord.nextPF*sizeof(Registro));
                    auxFile.read((char*)&SPrevioRecord,sizeof(Registro));
                    while(SPrevioRecord.id<=NuevoRegistro.id and SPrevioRecord.isMain){
                        posPrevio=PrevioRecord.nextPF;
                        PrevioRecord=SPrevioRecord;
                        auxFile.seekg(SPrevioRecord.nextPF*sizeof(Registro));
                        auxFile.read((char*)&SPrevioRecord,sizeof(Registro));
                    }
                    NuevoRegistro.nextPF=PrevioRecord.nextPF;
                    NuevoRegistro.isMain=PrevioRecord.isMain;
                    PrevioRecord.nextPF=SizeAux;
                    PrevioRecord.isMain=false;
                    ofstream auxFile3(auxFilename, ios::binary | ios::in | ios::out);
                    auxFile3.seekp(posPrevio*sizeof(Registro));
                    auxFile3.write((char*)&PrevioRecord, sizeof(Registro));
                    auxFile3.close();
                }

                ofstream mainFile(mainFilename, ios::binary | ios::in | ios::out);
                mainFile.write((char*)&HnextPF, sizeof(int));
                mainFile.write((char*)&HisMain, sizeof(bool));
                mainFile.close();

                ofstream auxFile3(auxFilename, ios::binary | ios::app);
                auxFile3.write((char*)&NuevoRegistro, sizeof(Registro));
                auxFile3.close();
            }
            return;
        }

        mainFile.seekg(0, ios::end);

        if ((int)mainFile.tellg() <= sizeof(int) + sizeof(bool)) {
            NuevoRegistro.nextPF = -1;
            ofstream mainFile(mainFilename, ios::binary | ios::app);
            mainFile.write((char*)&NuevoRegistro, sizeof(Registro));
            mainFile.close();
            return;
        }

        else {

            int inicio = 0;
            mainFile.seekg(0, ios::end);
            int tamMain = mainFile.tellg() / sizeof(Registro);
            int fin=tamMain;
            mainFile.seekg(0, ios::beg);

            Registro record;
            Registro PrevRecord;
            int medio;
            int pos;
            bool lastMain=true;
            while (inicio <= fin) {
                medio = (inicio + fin) / 2;
                mainFile.seekg(medio * sizeof(Registro) + sizeof(int) + sizeof(bool));
                mainFile.read((char *) &record, sizeof(Registro));
                if(record.id<NuevoRegistro.id){
                    inicio = medio + 1;
                    pos=medio;
                    PrevRecord=record;
                }
                else{
                    fin = medio - 1;
                    pos=medio-1;
                }
                if(record.id==NuevoRegistro.id){
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
                mainFile.seekg(pos * sizeof(Registro) + sizeof(int) + sizeof(bool), ios::beg);
                mainFile.read((char *) &PrevRecord, sizeof(Registro));
            }

            while(!PrevRecord.isMain and PrevRecord.id<NuevoRegistro.id){
                lastMain=false;
                ifstream auxFile1(auxFilename, ios::binary | ios::app);
                auxFile1.seekg(PrevRecord.nextPF*sizeof(Registro));
                auxFile1.read((char*)&record, sizeof(Registro));
                auxFile1.close();
                if(record.id<=NuevoRegistro.id){
                    pos=PrevRecord.nextPF;
                    PrevRecord=record;
                }
                else{
                    break;
                }
            }
            NuevoRegistro.nextPF=PrevRecord.nextPF;
            NuevoRegistro.isMain=PrevRecord.isMain;
            PrevRecord.nextPF=SizeAux;
            PrevRecord.isMain=false;
            if(lastMain){
                ofstream mainFile2(mainFilename, ios::binary | ios::in | ios::out);
                mainFile2.seekp(pos * sizeof(Registro)+sizeof(int)+sizeof(bool),ios::beg);
                mainFile2.write((char*)&PrevRecord, sizeof(Registro));
                mainFile2.close();
            } else{
                ofstream auxFile2(auxFilename, ios::binary | ios::in | ios::out);
                auxFile2.seekp(pos * sizeof(Registro),ios::beg);
                auxFile2.write((char*)&PrevRecord, sizeof(Registro));
                auxFile2.close();
            }
            ofstream auxFile2(auxFilename, ios::binary | ios::app);
            auxFile2.write((char*)&NuevoRegistro, sizeof(Registro));
            auxFile2.close();
        }

        auxFile.close();
        mainFile.close();

        if(SizeAux+1>int(log(SizeMain))){
            merge(mainFilename,auxFilename);
        }
    }


    void merge(string &mainFilename, string &auxFilename) {
        ofstream outputFile("merge.dat", ios::binary);
        if (!outputFile.is_open()) {
            throw runtime_error("No se pudo abrir el archivo de salida");
        }


        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) {
            throw runtime_error("No se pudo abrir el archivo main");
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
                Registro record;
                Registro temp;

                mainFile.seekg(nextPos * sizeof(Registro) + sizeof(int) + sizeof(bool));
                mainFile.read((char*) &record, sizeof(Registro));

                if (!record.isMain){
                        temp = record;
                        temp.isMain = true;
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        temp.nextPF = contador;
                        outputFile.write((char*) &temp, sizeof(Registro));
                }
                else{
                    contador++;
                    if (record.nextPF == -1){contador = -1;}
                    int tempnext = record.nextPF;
                    record.nextPF = contador;
                    outputFile.write((char*) &record, sizeof(Registro));
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
                Registro record;
                Registro temp;
                if (Whcfile){
                    mainFile.seekg(nextPos * sizeof(Registro) + sizeof(int) + sizeof(bool));
                    mainFile.read((char*) &record, sizeof(Registro));

                    if (!record.isMain){
                        temp = record;
                        temp.isMain = true;
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        temp.nextPF = contador;
                        outputFile.write((char*) &temp, sizeof(Registro));
                    }
                    else{
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        int tempnext = record.nextPF;
                        record.nextPF = contador;
                        outputFile.write((char*) &record, sizeof(Registro));
                        record.nextPF = tempnext;
                    }
                }
                else {
                    auxFile.seekg(nextPos * sizeof(Registro));
                    auxFile.read((char*) &record, sizeof(Registro));

                    if (!record.isMain){
                        temp = record;
                        temp.isMain = true;
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        temp.nextPF = contador;
                        outputFile.write((char*) &temp, sizeof(Registro));
                    }
                    else {
                        contador++;
                        if (record.nextPF == -1){contador = -1;}
                        int tempnext = record.nextPF;
                        record.nextPF = contador;
                        outputFile.write((char*) &record, sizeof(Registro));
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
            throw runtime_error("No se pudo abrir el archivo de salida");
        }

        int HnextPF = 0;
        int HisMain = 1;
        main.write((char*)&HnextPF, sizeof(int));
        main.write((char*)&HisMain, sizeof(bool));

        main.close();
    }

    void remove(T key){
        merge(mainFilename,auxFilename);

        fstream mainFile(mainFilename, ios::binary| ios::in | ios::out);
        if (!mainFile.is_open()) {
            throw runtime_error("No se pudo abrir el archivo main");
        }

        int inicio = 0;
        mainFile.seekg(0, ios::end);
        int fin = ((int)mainFile.tellg() - inicio) / sizeof(Registro) - 1;
        mainFile.seekg(0, ios::beg);

        Registro record;
        Registro PrevRecord;
        int pos;
        while (inicio <= fin) {
            int medio = (inicio + fin) / 2;
            mainFile.seekg(medio * sizeof(Registro) + sizeof(int) + sizeof(bool));
            mainFile.read((char*)&record, sizeof(Registro));

            if (record.id == key) {
                pos = medio;
                break;
            } else if (record.id < key) {
                inicio = medio + 1;
                if (key - record.id < key - PrevRecord.id) {
                    PrevRecord = record;
                }
            } else {
                fin = medio - 1;
            }

            pos = -4;
        }

        if (pos == -4){
            cout<<"No existe el ID"<<endl;
            return;
        }

        if (pos == 0){
            int HnextPF;
            HnextPF = 1;
            mainFile.seekp(0, ios::beg);
            mainFile.write((char*)&HnextPF, sizeof(int));

            Registro rmrecord;
            mainFile.seekg(pos * sizeof(Registro) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.read((char*) &rmrecord, sizeof(Registro));

            rmrecord.nextPF = -2;

            mainFile.seekp(pos * sizeof(Registro) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.write((char*) &rmrecord, sizeof(Registro));
        }

        else{
            Registro prevrecord;
            mainFile.seekg((pos-1) * sizeof(Registro) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.read((char*) &prevrecord, sizeof(Registro));

            Registro rmrecord;
            mainFile.seekg(pos * sizeof(Registro) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.read((char*) &rmrecord, sizeof(Registro));

            prevrecord.nextPF = rmrecord.nextPF;
            rmrecord.nextPF = -2;

            mainFile.seekp((pos-1) * sizeof(Registro) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.write((char*) &prevrecord, sizeof(Registro));

            mainFile.seekp(pos * sizeof(Registro) + sizeof(int) + sizeof(bool), ios::beg);
            mainFile.write((char*) &rmrecord, sizeof(Registro));
        }
        mainFile.close();
    }

    vector<Registro> search(T key) {
        vector<Registro> result;
        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) throw runtime_error("No se pudo abrir el archivo main");

        int inicio = 0;
        mainFile.seekg(0, ios::end);
        int fin = ((int)mainFile.tellg() - inicio) / sizeof(Registro) - 1;
        mainFile.seekg(0, ios::beg);

        Registro record;
        Registro PrevRecord;

        while (inicio <= fin) {
            int medio = (inicio + fin) / 2;
            mainFile.seekg(medio * sizeof(Registro) + sizeof(int) + sizeof(bool));
            mainFile.read((char*)&record, sizeof(Registro));

            if (record.id == key) {
                if (record.nextPF == -2){
                    cout << "El registro se encuentra eliminado" <<endl;
                    return result;
                }
                else{
                    result.push_back(record);
                    return result;
                }
            } else if (record.id < key) {
                inicio = medio + 1;
                if (key - record.id < key - PrevRecord.id) {
                    PrevRecord = record;
                }
            } else {
                fin = medio - 1;
            }
        }

        ifstream auxFile(auxFilename, ios::binary);
        if (!auxFile.is_open()) throw runtime_error("No se pudo abrir el archivo auxiliar");

        while(auxFile.read((char*)&record, sizeof(Registro))){
            if (PrevRecord.id == key) {
                result.push_back(PrevRecord);
                return result;
            }
        }

        cout << "No se encontro el ID" << endl;

        mainFile.close();
        auxFile.close();
        return result;
    }

    vector<Registro> rangeSearch(T beginkey, T endkey) {
        vector<Registro> result;
        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) throw runtime_error("No se pudo abrir el archivo main");

        ifstream auxFile(auxFilename, ios::binary);
        if (!auxFile.is_open()) throw runtime_error("No se pudo abrir el archivo auxiliar");

        Registro record;
        int inicio = 0;
        mainFile.seekg(0, ios::end);
        int fin = ((int)mainFile.tellg() - inicio) / sizeof(Registro) - 1;
        mainFile.seekg(0, ios::beg);

        while (inicio <= fin) {
            int medio = (inicio + fin) / 2;
            mainFile.seekg(medio * sizeof(Registro) + sizeof(int) + sizeof(bool));
            mainFile.read((char*)&record, sizeof(Registro));

            if (record.id >= beginkey) {
                fin = medio - 1;
            } else {
                inicio = medio + 1;
            }
        }

        bool found = false;

        while (record.id <= endkey) {
            if (found && record.id < beginkey) {
                break;
            }

            if (record.id >= beginkey) {
                result.push_back(record);
                found = true;
            }
            if(record.nextPF==-1)break;
            if (record.isMain) {
                mainFile.seekg(record.nextPF * sizeof(Registro)  + sizeof(int) + sizeof(bool));
                mainFile.read((char*)&record, sizeof(Registro));
            } else {
                auxFile.seekg(record.nextPF * sizeof(Registro));
                auxFile.read((char*)&record, sizeof(Registro));

            }
        }

        mainFile.close();
        auxFile.close();
        return result;
    }
};
