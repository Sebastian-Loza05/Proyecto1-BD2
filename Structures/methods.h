#pragma once
#include "Record.h"
#include <utility>
#include <vector>


// template<typename TK>
class MethodSelector{
public:
  MethodSelector(){}
  
  virtual bool add(Record record){
    cout<<"add-method"<<endl;
    return false;
  }

  virtual vector<Record> load() {

    return vector<Record>();
  }

  virtual void display(){
    return;
  }


  virtual void display_all(){
    return;
  }

  virtual vector<Record> rangeSearch(int min, int max){
    return vector<Record>();
  }

  virtual vector<Record> rangeSearch(long min, long max){
    return vector<Record>();
  }

  virtual vector<Record> rangeSearch(char* min, char* max){
    return vector<Record>();
  }

  virtual bool remove( char* key ){
    return false;
  }

  virtual bool remove( int key ){
    return false;
  }

  virtual bool remove( long key ){
    return false;
  }

  virtual pair<Record, bool> search(char* key){
    return make_pair(Record(), false);
  }

  virtual pair<Record, bool> search(int key){
    return make_pair(Record(), false);
  }
  
  virtual pair<Record, bool> search(long key){
    return make_pair(Record(), false);
  }
  ~MethodSelector(){}
};
