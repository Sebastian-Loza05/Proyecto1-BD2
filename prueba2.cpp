#include "BPlusFile.h"
#include "tokenSQL.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <cstring>
void ingreso(){
  BPlusFile<char*, 20> bplus;
  // bplus.displayTree();
  // return;
  // BPlusFile<long , sizeof(long)> bplus;
  ifstream archivo("datos.csv");
  
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

    Record record(key,nombre,producto,marca,precio,cantidad);
    record.print();
    string asd;
    // if (counter > 96)
    // cin >> asd;
    bool asd__ = bplus.remove(record.key);
    // bool asd__ = bplus.add(record);
    // cout<<"counter: "<<counter<<endl;
    counter++;
    bplus.displayTree();
    //
    // cin >> asd;
    if(counter == 5000)break;
    campos.clear();
  }
  bplus.displayTree();

  archivo.close();

  return;
}

int main(){

  ingreso();
  


  return 0;
}
