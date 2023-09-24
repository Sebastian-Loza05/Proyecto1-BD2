#pragma once
#include "Record.h"
#include <vector>


// template<typename TK>
class MethodSelector{
public:
  MethodSelector(){}
  
  virtual bool add(Record record){
    return false;
  }

  virtual vector<Record> load() {

    return vector<Record>();
  }

  virtual void display(){
    return;
  }

  Record search(){
    return Record();
  }

  virtual void display_all(){
    return;
  }

  virtual vector<Record> rangeSearch(){
    return vector<Record>();
  }

  virtual bool remove( char* key ){
    return false;
  }

  virtual bool remove( int key ){
    return false;
  }

  virtual bool remove( double key ){
    return false;
  }

  virtual bool remove( float key ){
    return false;
  }

  virtual bool remove( char key ){
    return false;
  }

  virtual bool remove( long key ){
    return false;
  }
  ~MethodSelector(){}
};
