#include <iostream>
#include "AvlFile.h"
#include "BPlusFile.h"
#include "SequentialFile.h"
#include "methods.h"

using namespace std;

int main (int argc, char *argv[]) {

  //Se pedira escoger con que metodo se trabajara(pensando en la UI).
  int metodo;
  do{
    cout<<"1.- AvlFile."<<endl;
    cout<<"2.- Sequential File."<<endl;
    cout<<"3.- Bplus File."<<endl;
    cout<<"Ingrese el metodo a utilizar: ";
    cin>>metodo;
  }while(metodo < 1 || metodo > 3);


  if(metodo == 1)
    MethodSelector<AVLFile> method;
  else if(metodo == 2)
    MethodSelector<SequentialFile> method;
  else
    MethodSelector<BPlusFile> method;
  
  return 0;
}
