#pragma once
#include <iostream>
using namespace std;

class BPlusFile{
  string filename;

public:
  BPlusFile(){
    this->filename = "bplus_datos.dat";
  }

  ~BPlusFile(){}
};
