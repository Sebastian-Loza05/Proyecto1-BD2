#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct Record{

};

template<typename T>
class MethodSelector{
  T metodo;
public:
  MethodSelector(){}
  
  void add(){
    //metodo.add();
  }

  bool search(){
    //metodo.search();
    return true;
  }

  vector<Record> rangeSearch(){
    vector<Record> res;
    //metodo.rangeSearch();
    return res;
  }

  void remove(){
    //metodo.remove();
  }

  ~MethodSelector(){}
};
