#include <iostream>
#include <fstream>
#include <vector>
#include <cstring> 
using namespace std;

template <typename... Ts>
struct MiTupla {
    tuple<Ts...> valores;

    MiTupla(Ts... args) : valores(make_tuple(args...)) {}

    template <size_t... Is>
    void imprimir(index_sequence<Is...>) {
        ((cout << "Valor " << Is + 1 << ": " << get<Is>(valores) << endl), ...);
    }
    void imprimir() {
        imprimir(index_sequence_for<Ts...>());
    }
};

bool operator<(char *numero, const char* a) {
  // Realiza la comparaciasdón personalizada aquí
  // En este ejemplo, se compara la longitud de la cadena con el número
  return true;
}
int main(){
  // MiTupla miTupla("asd", 42, 3.14, "Hola");

  // miTupla.imprimir();
  // ifstream archivo("datos.csv");
  // 
  // if (!archivo.is_open()) {
  //   cout << "Error al abrir el archivo" << endl;
  //   return 0;
  // }
  //
  // string linea;
  // vector<string> campos;
  //
  // istringstream lineaStream(linea);
  // int counter = 0;
  // while (getline(archivo, linea)) {
  //   istringstream lineaStream(linea);
  //   string campo;
  //
  //   while (getline(lineaStream, campo, ',')) {
  //     campos.push_back(campo);
  //   }
  //

  const char* cadena1 = "Hola_";
  const char* cadena2 = "Hola";

    int resultado = std::strcmp(cadena1, cadena2);

    if (resultado <= 0) {
        std::cout << "cadena1 es menor o igual que cadena2." << std::endl;
    } else if (resultado > 0) {
        std::cout << "cadena1 es mayor que cadena2." << std::endl;
    } 
  return 0;
}


