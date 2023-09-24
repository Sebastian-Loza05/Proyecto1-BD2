#include <iostream>
#include <typeinfo>
#include <sstream>
// #include "BPlusFile.h"
#include "SequentialFile.h"
#include "Structures/AVLFile.h"

using namespace std;

template<typename  T>
void ingreso(T *method){
  // BPlusFile<long , sizeof(long)> bplus;
  ifstream archivo("datos_prueba.csv");
  
  if (!archivo.is_open()) {
    cout << "Error al abrir el archivo" << endl;
    return;
  }

  string linea;
  vector<string> campos;

  istringstream lineaStream(linea);
  int counter = 0;
  while (getline(archivo, linea)) {
    istringstream lineaStream(linea);
    string campo;

    while (getline(lineaStream, campo, ',')) {
      campos.push_back(campo);
    }

    char key[20];
    // long key;
    char nombre[20];
    char producto[20];
    char marca[20];
    float precio;
    int cantidad;
    
    // key = stoi(campos[0]);
    
    strncpy(key, campos[0].c_str(), sizeof(key) - 1);
    nombre[sizeof(key) - 1 ]= '\0';

    strncpy(nombre, campos[1].c_str(), sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1 ]= '\0';


    strncpy(producto, campos[2].c_str(), sizeof(producto) - 1);
    producto[sizeof(producto) - 1 ]= '\0';

    strncpy(marca, campos[3].c_str(), sizeof(marca) - 1);
    marca[sizeof(marca) - 1 ]= '\0';

    // cout << campos[4] << endl;
    precio = stof(campos[4]);
    cantidad = stoi(campos[5]);

    // cout << "---prev---" << endl;
    // method->display_all();
    // cout << "--**----" << endl;
    Record record(key,nombre,producto,marca,precio,cantidad);
    cout << "Insert: ";
    record.print();
    string asd;
    // if (counter > 96)
    //   cin >> asd;
    // cout << endl << endl << endl;
      bool inser = 0;
    // method->display_all();
    method->display();
    cin >> inser;
    if (inser) {
      cout << "Key: ";
      char key_del[20];
      cin >> key_del;

      pair<Record,bool> asd__ = method->search(record);
    }
    else {
      cout << "Key: ";
      char key_del[20];
      cin >> key_del;
      bool asd___ = method->remove(key_del);
      cout << "Rpta: " << asd___ << endl;
      method->display();
    }
    
    // cout << "Rpta: " << asd__ << endl;
    // cout << "Asda" << endl;
    // cout << "Asda" << endl;
    // for (const string& valor: campos){
    //   cout << valor << "\t";
    // }
    // cout << endl;
    cout<<"counter: "<<counter<<endl;
    counter++;
    // cout << "--desp----" << endl;
    // method->display_all();
    // cout << "------" << endl;
    
    // bplus.displayTree();
    //
    // cin >> asd;
    if(counter == 200000)break;
    campos.clear();
  }
  // bplus.displayTree();

  archivo.close();

  return;
}

int main (int argc, char *argv[]) {

  //Se pedira escoger con que metodo se trabajara(pensando en la UI).
  // int metodo;
  // do{
  //   cout<<"1.- AvlFile."<<endl;
  //   cout<<"2.- Sequential File."<<endl;
  //   cout<<"3.- Bplus File."<<endl;
  //   cout<<"Ingrese el metodo a utilizar: ";
  //   cin>>metodo;
  // } while ( metodo < 1 || metodo > 3 );
  //
  MethodSelector *method = nullptr;
  //
  // if (metodo == 1) {
    method = new AVLFile<char*>(); // STRUCT 1
  // }
  
  ingreso(method);
  // SequentialFile<int> *seq = new SequentialFile<int>;
  // ingreso(seq);
  

  
  return 0;
}
