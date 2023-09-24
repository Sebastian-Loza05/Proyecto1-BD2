#include <iostream>
#include "sequential.h"

using namespace std;
void AddRecordsToMain(const string& mainFilename) {
    ofstream mainFile(mainFilename, ios::binary);

    if (!mainFile.is_open()) {
        cerr << "Error al abrir el archivo principal." << endl;
    }

    int initialIntValue = 0;
    bool initialBoolValue = true;

    mainFile.write((char*)&initialIntValue, sizeof(int));
    mainFile.write((char*)&initialBoolValue, sizeof(bool));

    mainFile.close();

    Registro mainRecords[] = {
            {2, "Ana", "producto", 20, 7 , 1, true},
            {3, "Bartola","producto", 20, 7 , 0, false},
            {8, "Boris","producto", 20, 7 , 3, true},
            {11, "Celia","producto", 20, 7 , 4, true},
            {12, "Cesar","producto", 20, 7 , -1, true},
    };

    ofstream mainFileAppend(mainFilename, ios::binary | ios::app);

    if (!mainFileAppend.is_open()) {
        cerr << "Error al abrir el archivo principal para añadir registros." << endl;
        return;
    }

    for (const auto& record : mainRecords) {
        mainFileAppend.write((const char*)&record, sizeof(Registro));
    }

    mainFileAppend.close();
}

void AddRecordsToAux(const string& auxFilename) {
    Registro auxRecords[] = {
            {6, "Barth","producto", 20, 7 , 2, true}
            /*
            {3, "Barth", 1, true},
            {4, "Beto", 2, true},
            {6, "Carlos", 3, true},
            {8, "Dana", 4, true}*/
    };

    ofstream auxFile(auxFilename, ios::binary);

    if (!auxFile.is_open()) {
        cerr << "Error al abrir el archivo auxiliar para añadir registros." << endl;
        return;
    }

    for (const auto& record : auxRecords) {
        auxFile.write((const char*)&record, sizeof(Registro));
    }

    auxFile.close();
}

void PrintMainRecords(const string& mainFilename) {
    ifstream mainFile(mainFilename, ios::binary);

    if (!mainFile.is_open()) {
        cerr << "Error al abrir el archivo principal." << endl;
        return;
    }

    cout << "Registros en el archivo principal:" << endl;

    int HnextPF;
    bool HisMain;
    mainFile.read((char*)&HnextPF, sizeof(int));
    mainFile.read((char*)&HisMain, sizeof(bool));
    cout<<"Header: "<<HnextPF<<"|"<<HisMain<<endl;
    cout<<"--------------------"<<endl;

    while (mainFile) {
        Registro record;
        mainFile.read((char*)&record, sizeof(Registro));
        if (!mainFile.eof()) {
            cout << "id: " << record.id << ", nombre: " << record.nombre << ", nextPF: " << record.nextPF << ", isMain: " << record.isMain << endl;
        }
    }

    mainFile.close();
}

void PrintAuxRecords(const string& auxFilename) {
    ifstream auxFile(auxFilename, ios::binary);

    if (!auxFile.is_open()) {
        cerr << "Error al abrir el archivo auxiliar." << endl;
        return;
    }

    cout << "Registros en el archivo auxiliar:" << endl;

    while (auxFile) {
        Registro record;
        auxFile.read((char*)&record, sizeof(Registro));
        if (!auxFile.eof()) {
            cout << "id: " << record.id << ", nombre: " << record.nombre << ", nextPF: " << record.nextPF << ", isMain: " << record.isMain << endl;
        }
    }

    auxFile.close();
}

int main() {
    string mainFilename = "main.dat";
    string auxFilename = "aux.dat";
    AddRecordsToMain(mainFilename);
    AddRecordsToAux(auxFilename);
    SequentialFile<int> sqFile(mainFilename, auxFilename);

    while (true) {
        cout << "1. Agregar un nuevo registro" << endl;
        cout << "2. Imprimir registros en el archivo principal" << endl;
        cout << "3. Imprimir registros en el archivo auxiliar" << endl;
        cout << "4. Realizar el merge y mostrar registros resultantes" << endl;
        cout << "5. Remover registro por id" << endl;
        cout << "6. Buscar un registro por id" << endl;
        cout << "7. Buscar rango de registros por id" << endl;
        cout << "8. Salir" << endl;
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                Registro nuevoRegistro;
                nuevoRegistro.setData();
                sqFile.add(nuevoRegistro);
                break;
            }
            case 2: {
                PrintMainRecords(mainFilename);
                break;
            }
            case 3: {
                PrintAuxRecords(auxFilename);
                break;
            }
            case 4: {
                sqFile.merge(mainFilename,auxFilename);
                cout << "Registros en el archivo resultante:" << endl;
                PrintMainRecords(mainFilename);
                break;
            }
            case 5: {
                int reg;
                cout<< "Registro a eliminar por id: "<<endl;
                cin >> reg;
                sqFile.remove(reg);
                cout << "Registro eliminado" <<endl;
                break;
            }
            case 6: {
                int reg;
                cout<< "Registro a buscar por id: "<<endl;
                cin >> reg;

                vector<Registro> result;
                result = sqFile.search(reg);

                for (auto c : result){
                    cout << "id:" << c.id << endl;
                    cout << "Nombre: " << c.nombre << endl;
                }

                break;
            }
            case 7: {
                int min, max;
                cout<< "ingresa el minimo: "<<endl;
                cin >> min;

                cout<< "ingresa el minimo: "<<endl;
                cin >> max;

                vector<Registro> result;

                result = sqFile.rangeSearch(min, max);
                for (auto c : result){
                    cout << "id:" << c.id << endl;
                    cout << "Nombre: " << c.nombre << endl;
                }
                break;
            }
            case 8:
                return 0;
            default:
                cout << "Opción no válida. Introduce un número válido." << endl;
        }
    }
}

