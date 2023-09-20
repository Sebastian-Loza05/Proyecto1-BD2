#pragma once
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

struct Record1{
  // long key;
  char key[20];
  char nombre[20];
  char producto[20];
  char marca[20];
  float precio;
  int cantidad;

  Record1(char key_[20], char nombre_[20], char producto_[20],char marca_[20],float precio_, int cantidad_){
  // Record1(long key_, char nombre_[20], char producto_[20],char marca_[20],float precio_, int cantidad_){
    // this->key = key_;
    strncpy(key, key_, sizeof(key));
    strncpy(nombre, nombre_, sizeof(nombre));
    strncpy(producto, producto_, sizeof(producto));
    strncpy(marca, marca_, sizeof(marca));
    this->precio = precio_;
    this->cantidad = cantidad_;
  };

  Record1(){};

  void print(){
    cout << key << "\t" << nombre << "\t" << producto << "\t" << marca << "\t" << precio << "\t" << cantidad << endl;
  }
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
