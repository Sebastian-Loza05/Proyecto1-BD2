#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include "functions.h"

using namespace std;

struct Record{
  int key;
  //char key[20];
  char nombre[20];
  char producto[20];
  char marca[20];
  float precio;
  int cantidad;
  
  Record(int key_, char nombre_[20], char producto_[20], char marca_[20], float precio_, int cantidad_){
  // Record(char key_[20], char nombre_[20], char producto_[20], char marca_[20], float precio_, int cantidad_){
    //strncpy(key, key_, sizeof(key));
    this->key = key_;
    strncpy(nombre, nombre_, sizeof(nombre));
    strncpy(producto, producto_, sizeof(producto));
    strncpy(marca, marca_, sizeof(marca));
    this->precio = precio_;
    this->cantidad = cantidad_;
  }

  Record(){};

  void print(){
    // cout << key << "\t" << nombre << "\t" << producto << "\t" << marca << "\t" << precio << "\t" << cantidad << endl;
    cout << key << "\t" << nombre << "\t" << producto << "\t" << endl;

  }
};
