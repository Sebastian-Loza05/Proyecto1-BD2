#include <iostream>
#include <typeinfo>
#include <sstream>
#include "BPlusFile.h"
#include "SequentialFile.h"
#include "Structures/AVLFile.h"
#include "Structures/Record.h"
#include "Structures/methods.h"
#include <chrono>
using namespace std;

template<typename  T>
void ingreso(T *method){
  // method->display_all();
  // int a;
  // cin >> a;
  // BPlusFile<long , sizeof(long)> bplus;
  ifstream archivo("datos.csv");
  // ifstream archivo("datos2.csv");
  cout << "asdasdsa" << endl; 
  if (!archivo.is_open()) {
    cout << "Error al abrir el archivo___" << endl;
    return;
  }

  string linea;
  vector<string> campos;
  vector <Record> records_;

  istringstream lineaStream(linea);
  int counter = 0;
  while (getline(archivo, linea)) {
    istringstream lineaStream(linea);
    string campo;

    while (getline(lineaStream, campo, ',')) {
      campos.push_back(campo);
    }

    int key;
    char nombre[20];
    char producto[20];
    char marca[20];
    float precio;
    int cantidad;

    key = stoi(campos[0]);
    strncpy(nombre, campos[1].c_str(), sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1 ]= '\0';


    strncpy(producto, campos[2].c_str(), sizeof(producto) - 1);
    producto[sizeof(producto) - 1 ]= '\0';

    strncpy(marca, campos[3].c_str(), sizeof(marca) - 1);
    marca[sizeof(marca) - 1 ]= '\0';

    // cout << campos[4] << endl;
    precio = stof(campos[4]);
    cantidad = stoi(campos[5]);
    Record record(key,nombre,producto,marca,precio,cantidad);
    records_.push_back(record);
    // record.print();

    // cout << "---prev---" << endl;
    // method->display_all();
    // cout << "--**----" << endl;


    // char key[20];
    // char genero[20];
    // char profesion[20];
    // int edad;
    // float sueldo;
    //
    // strncpy(key, campos[0].c_str(), sizeof(key) - 1);
    // key[sizeof(key) - 1 ]= '\0';
    // strncpy(genero, campos[1].c_str(), sizeof(genero) - 1);
    // genero[sizeof(genero) - 1 ]= '\0';
    // strncpy(profesion, campos[2].c_str(), sizeof(profesion) - 1);
    // profesion[sizeof(profesion) - 1 ]= '\0';
    // edad = stoi(campos[3]);
    // sueldo = stof(campos[4]);
    // Record2 record(key,genero,profesion,edad,sueldo);
    // record.print();
    // Record
    // if (counter > 96)
    //   cin >> asd;
    // cout << endl << endl << endl;
    // method->display();
    // vector<Record> vec1 = method->load();
    // 
    // for (int i = 0; i < vec1.size(); i++) {
    //   vec1[i].print();
    //   cout << "-----" << endl;
    //}

    // cin >> inser;
    // if (inser) {
    //   cout << "Key: ";
    //   int key_insert_b, key_insert_e;
    //   // cin >> key_insert_b;
    //   // pair<Record,bool> asd = method->search(key_insert_b);
    //   // char key_insert_b[20], key_insert_e[20];
    //   cin >> key_insert_b >> key_insert_e;
    //   //
    //   vector<Record> vec = method->rangeSearch(key_insert_b,key_insert_e);
    //   for (int i = 0; i < vec.size(); i++) {
    //     vec[i].print();
    //   }
    //     cout << "-----" << endl;
    //
    //   // if (asd.second) {
    //   //   asd.first.print();
    //   // }
    //   // else{
    //   //   cout << "No existe este elemento" << endl;
    //   // }
    //   cin >> key_insert_b;
    // }
    // else {
    //   bool asd___ = method->add(record);
    //   // cout << "Key: ";
    //   // char key_del[20];
    //   // cin >> key_del;
    //   // bool asd___ = method->remove(key_del);
    //   // cout << "Rpta: " << asd___ << endl;
    //   // method->display();
    // }
    
    // method->display_all();
    // cout << "Rpta: " << asd__ << endl;
    // cout << "Asda" << endl;
    // cout << "Asda" << endl;
    // for (const string& valor: campos){
    //   cout << valor << "\t";
    // }
    // cout << endl;
    // cout<<"counter: "<<counter<<endl;
    counter++;
    // cout << "--desp----" << endl;
    // method->display_all();
    // cout << "------" << endl;
    
    // bplus.displayTree();
    //
    // cin >> asd;
    if(counter == 10000){
      break;
    }
    campos.clear();
  }
  // bplus.displayTree();

  archivo.close();
  chrono::time_point<std::chrono::system_clock> t_inicio,t_final;
  t_inicio = chrono::high_resolution_clock::now();
  
  for (int i = 0; i < records_.size(); i++) {
    bool asd = method->add(records_[i]);
    // records_[i].print();
  }
  t_final  = chrono::high_resolution_clock::now(); 
  std::chrono::duration<double, std::milli> t = t_final - t_inicio;
  method->display_all();
  cout << "Tiempo = " << t.count() << "ms" << endl;


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
  // MethodSelector *method = nullptr;
  //
  // if (metodo == 1) {
    // method = new AVLFile<char*>(); // STRUCT 1
  // }
  
  // ingreso(method);
  // MethodSelector<Record2> *seq = new BPlusFile<Record2, char*, 20>();
  // MethodSelector<Record> *seq = new BPlusFile<Record, int, sizeof(int)>();
  MethodSelector<Record> *seq = new AVLFile<Record, int>;
  // MethodSelector<Record> *seq = new AVLFile<Record, int>;
  // MethodSelector<Record2> *seq = new SequentialFile<Record2, char*>();
  // MethodSelector<Record> *seq = new SequentialFile<Record, int>();
  ingreso(seq);
  
  

  
  return 0;
}
