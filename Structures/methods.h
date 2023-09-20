#pragma once
#include "Record.h"

class MethodSelector{
public:
  MethodSelector(){}
  
  virtual bool add(Record record){
    return false;
  }

  virtual void display(){
   
  }

  bool search(){
    //metodo.search();
    return true;
  }

  virtual void display_all(){

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
