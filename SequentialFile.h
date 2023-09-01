#pragma once
#include <iostream>
using namespace std;

class SequentialFile{
  string filename;

public:
  SequentialFile(){
    //El nombre del archivo estara por defecto para cada metodo, si se necesitan mas archivos agregar tambien un nombre por defecto.
    
    this->filename = "sequential_datos.dat";
  }
  SequentialFile(string filename):filename(filename) {}

  ~SequentialFile(){}
};
