#pragma once
#include <iostream>
using namespace std;

class SequentialFile{
private:
    string mainFilename;
    string auxFilename;

public:
  SequentialFile(){
    //El nombre del archivo estara por defecto para cada metodo, si se necesitan mas archivos agregar tambien un nombre por defecto.
    
    this->filename = "sequential_datos.dat";
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
  SequentialFile(string filename, string auxFilename) {
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

  void add(Registro NuevoRegistro){
    ifstream mainFile(mainFilename, ios::binary);
    if (!mainFile.is_open()) {
        throw runtime_error("No se pudo abrir el archivo main");
    }
    int HnextPF;
    bool HisMain;
    Registro FirstRecord;
    mainFile.read((char*)&HnextPF, sizeof(int));
    mainFile.read((char*)&HisMain, sizeof(bool));
    mainFile.read((char*)&FirstRecord, sizeof(Registro));
    if(FirstRecord.id>NuevoRegistro.id){
        if(HisMain){
            ifstream auxFile(auxFilename, ios::binary | ios::app);
            auxFile.seekg(0,ios::end);
            NuevoRegistro.nextPF=HnextPF;
            NuevoRegistro.isMain=HisMain;

            HnextPF=auxFile.tellg()/sizeof(Registro);
            HisMain=false;
            auxFile.close();

            ofstream auxFile2(auxFilename, ios::binary | ios::app);
            auxFile2.write((char*)&NuevoRegistro, sizeof(Registro));
            auxFile2.close();

            ofstream mainFile(mainFilename, ios::binary | ios::in | ios::out);
            mainFile.write((char*)&HnextPF, sizeof(int));
            mainFile.write((char*)&HisMain, sizeof(bool));
            mainFile.close();
        }
        else{
            Registro PrevRecord;
            ifstream auxFile(auxFilename, ios::binary | ios::app);
            auxFile.seekg(HnextPF*sizeof(Registro));
            auxFile.read((char*)&PrevRecord,sizeof(Registro));

            if(PrevRecord.id>NuevoRegistro.id){
                while(!PrevRecord.isMain){
                    auxFile.seekg(PrevRecord.nextPF*sizeof(Registro));
                    auxFile.read((char*)&PrevRecord,sizeof(Registro));
                }
            }
        }
        return;
    }

    mainFile.seekg(0, ios::end);
    if ((int)mainFile.tellg() <= sizeof(int) + sizeof(bool)) {
        NuevoRegistro.nextPF = -1;
        ofstream mainFile(mainFilename, ios::binary | ios::app);
        mainFile.write((char*)&NuevoRegistro, sizeof(Registro));
        mainFile.close();
    }

    else{

        int inicio = 0;
        mainFile.seekg(0, ios::end);
        int fin = ((int)mainFile.tellg() - inicio) / sizeof(Registro) - 1;
        mainFile.seekg(0, ios::beg);

        Registro record;
        Registro PrevRecord;
        int medio;
        while (inicio <= fin) {
            medio = (inicio + fin) / 2;
            mainFile.seekg(medio * sizeof(Registro) + sizeof(int) + sizeof(bool));
            mainFile.read((char*)&record, sizeof(Registro));

            if (record.id == NuevoRegistro.id) {
                PrevRecord = record;
                NuevoRegistro.nextPF=record.nextPF;
                NuevoRegistro.isMain=true;
                ifstream auxFile(auxFilename, ios::binary | ios::app);
                auxFile.seekg(0,ios::end);
                record.nextPF=auxFile.tellg()/sizeof(Registro);
                record.isMain=false;
                ofstream mainFile2(mainFilename, ios::binary | ios::in | ios::out);
                mainFile2.seekp(medio * sizeof(Registro) + sizeof(int) + sizeof(bool));
                mainFile2.write((char*)&record, sizeof(Registro));
                break;
            } else if (record.id < NuevoRegistro.id) {
                inicio = medio + 1;
                if (NuevoRegistro.id - record.id < NuevoRegistro.id - PrevRecord.id) {
                    PrevRecord = record;
                }
            } else {
                fin = medio - 1;
            }
        }

        while(!PrevRecord.isMain){

            ifstream auxFile(auxFilename, ios::binary | ios::app);
            ofstream auxFile2(auxFilename, ios::binary | ios::in | ios::out);
            int anter=PrevRecord.nextPF;
            auxFile2.seekp(PrevRecord.nextPF*sizeof(Registro));
            auxFile.seekg(PrevRecord.nextPF*sizeof(Registro));
            auxFile.read((char*)&PrevRecord,sizeof(Registro));
            auxFile.seekg(0,ios::end);
            if(PrevRecord.id > NuevoRegistro.id){
                PrevRecord.showData();
            }
            if(PrevRecord.isMain){
                NuevoRegistro.nextPF=PrevRecord.nextPF;
                NuevoRegistro.isMain=true;
                PrevRecord.isMain= false;
                PrevRecord.nextPF=auxFile.tellg()/sizeof(Registro);
                auxFile2.write((char*)&PrevRecord,sizeof(Registro));
                auxFile2.close();
                auxFile.close();
                break;
            }
        }
        if(PrevRecord.isMain){
            ofstream mainFile3(mainFilename,ios::binary | ios::in | ios::out);
            if(PrevRecord.nextPF==-1){
                NuevoRegistro.nextPF=-1;
                mainFile3.seekp((medio) * sizeof(Registro) + sizeof(int) + sizeof(bool));
            }
            else{
                NuevoRegistro.nextPF=PrevRecord.nextPF;
                mainFile3.seekp((medio-1) * sizeof(Registro) + sizeof(int) + sizeof(bool));
            }
            NuevoRegistro.isMain=true;
            ifstream auxFile(auxFilename,ios::binary);
            auxFile.seekg(0,ios::end);

            PrevRecord.isMain=false;
            PrevRecord.nextPF=auxFile.tellg()/sizeof(Registro);
            mainFile3.write((char*)&PrevRecord,sizeof(Registro));
            mainFile3.close();
        }
        ofstream auxFile(auxFilename, ios::binary | ios::app);
        auxFile.write((char*)&NuevoRegistro, sizeof(Registro));
        auxFile.close();
    }
    mainFile.close();
  }

    vector<Registro> search(int key) {
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
                result.push_back(record);
                return result;
            } else if (record.id < key) {
                inicio = medio + 1;
                if (key - record.id < key - PrevRecord.id) {
                    PrevRecord = record;
                }
            } else {
                fin = medio - 1;
            }
        }

        while(!PrevRecord.isMain){
            ifstream auxFile(auxFilename, ios::binary);
            if (!auxFile.is_open()) throw runtime_error("No se pudo abrir el archivo auxiliar");
            auxFile.seekg(PrevRecord.nextPF * sizeof(Registro));
            auxFile.read((char*)&PrevRecord, sizeof(Registro));
            if (PrevRecord.id == key) {
                result.push_back(PrevRecord);
                return result;
            }
        }

        return result;
    }

    vector<Registro> rangeSearch(int beginkey, int endkey) {
        vector<Registro> result;
        ifstream mainFile(mainFilename, ios::binary);
        if (!mainFile.is_open()) {
            throw runtime_error("No se pudo abrir el archivo main");
        }

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

            if (record.isMain) {
                mainFile.seekg(record.nextPF * sizeof(Registro)  + sizeof(int) + sizeof(bool));
                mainFile.read(reinterpret_cast<char*>(&record), sizeof(Registro));
            } else {
                ifstream auxFile(auxFilename, ios::binary);
                if (!auxFile.is_open()) {
                    throw runtime_error("No se pudo abrir el archivo auxiliar");
                }

                auxFile.seekg(record.nextPF * sizeof(Registro));
                auxFile.read(reinterpret_cast<char*>(&record), sizeof(Registro));
            }
        }

        return result;
    }


  ~SequentialFile(){}
};
