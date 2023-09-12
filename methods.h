#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct Record1{
  int key;

  Record1(){}
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

  vector<Record1> rangeSearch(){
    vector<Record1> res;
    //metodo.rangeSearch();
    return res;
  }

  void remove(){
    //metodo.remove();
  }

  ~MethodSelector(){}
};
