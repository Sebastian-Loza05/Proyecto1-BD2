#include <iostream>
// #include "AvlFile.h"
#include "BPlusFile.h"
// #include "SequentialFile.h"
#include "methods.h"
#include "parserSQL.h"

using namespace std;

int main (int argc, char *argv[]) {

  //Se pedira escoger con que metodo se trabajara(pensando en la UI).
  // int metodo;
  // do{
  //   cout<<"1.- AvlFile."<<endl;
  //   cout<<"2.- Sequential File."<<endl;
  //   cout<<"3.- Bplus File."<<endl;
  //   cout<<"Ingrese el metodo a utilizar: ";
  //   cin>>metodo;
  // }while(metodo < 1 || metodo > 3);

  string sentencia = "";
  while(true){
    cout<<"Inserte su sentencia SQL: "<<endl;
    getline(cin, sentencia);
    Scanner scanner(sentencia + " ");
    Parser parser(&scanner);
    parser.parse();
  }


  // if(metodo == 1)
  //   MethodSelector<AVLFile> method;
  // else if(metodo == 2)
  //   MethodSelector<SequentialFile> method;
  // else
  //   MethodSelector<BPlusFile<int>> method;
  
  return 0;
}
