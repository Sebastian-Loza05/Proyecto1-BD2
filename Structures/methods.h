#pragma once
#include "Record.h"
#include <utility>
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

  virtual pair<Record, bool> search(char* key){
    return make_pair(Record(), false);
  }

  virtual pair<Record, bool> search(int key){
    return make_pair(Record(), false);
  }

  virtual pair<Record, bool> search(float key){
    return make_pair(Record(), false);
  }

  virtual pair<Record, bool> search(double key){
    return make_pair(Record(), false);
  }

  virtual pair<Record, bool> search(char key){
    return make_pair(Record(), false);
  }
  
  virtual pair<Record, bool> search(long key){
    return make_pair(Record(), false);
  }
  ~MethodSelector(){}
};
