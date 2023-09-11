#include <iostream>
#include <ctime>
#include <vector>
#include "bplus2.h"

void print_vec(vector<int>v){
  cout<<"v: ";
  for (int i = 0; i < v.size(); i++) {
    cout << v[i] << " ";
  }
  cout << endl;
}

int main() {
    // Semilla para la generación de números aleatorios
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // Crear una instancia de Bplus
    BPlus<int,string> bplus(3);
    vector<int> valores;
    string c;
    // Generar y insertar 80 datos aleatorios uno por uno
    for (int i = 0; i < 80; ++i) {
      int random_int = 0 + std::rand() % 300; // Generar un número entero aleatorio
      valores.push_back(random_int);
      print_vec(valores);
      std::string random_str = "String_" + std::to_string(i); // Generar una cadena aleatoria
      bplus.insert(random_int, random_str); // Insertar los valores aleatorios en Bplus
      bplus.print();
      cin >> c;
    }
    return 0;
}
