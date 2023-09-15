#include "BPlusFile.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <cstring>
void ingreso(){
  BPlusFile<long> bplus;
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

    Record1 record(key,nombre,producto,marca,precio,cantidad);
    record.print();
    string asd;
    // if (counter > 96)
    //   cin >> asd;
    bool asd__ = bplus.add(record);
    // cout << "Asda" << endl;
    bplus.displayTree();
    // cout << "Asda" << endl;
    // for (const string& valor: campos){
    //   cout << valor << "\t";
    // }
    // cout << endl;
    cout<<"counter: "<<counter<<endl;
    counter++;
    if(counter == 20000)break;
    campos.clear();
  }

  archivo.close();

  return;
}

int main(){

  ingreso();
  


  return 0;
}
