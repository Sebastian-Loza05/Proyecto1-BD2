#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <sys/utsname.h>
#include "methods.h"
#include <unistd.h>
//Si usas windows descomenta la linea de abajo y comenta la de arriba.
// #include <windows.h>

struct Bucket{
  
  vector<Record1> records;
  long next;
  int count;
  int m;

  void read(ifstream &file){
    //count m records next
    file.read((char*)(&this->count), sizeof(int));
    file.read((char*)(&this->m), sizeof(int));
    for (int i = 0; i < this->count; i++) {
      file.read((char*)(&this->records[i]), sizeof(Record1));
    }
    file.read((char*)(&this->next), sizeof(long));
  }
};

template<typename TK>
struct Node{
  TK* keys;
  long* children;
  int count;
  bool pre_leaf;

  Node(int M){
    this->keys = new TK[M];
  }

  void read(ifstream &file, int M){
    //count leaf keys children
    file.read((char*)(&this->count), sizeof(bool));
    file.read((char*)(&this->pre_leaf), sizeof(int));
    for (int i = 0; i < M; i++) {
      file.read((char*)(&this->keys[i]),sizeof(TK));
    }
    for (int i = 0; i < M; i++) {
      file.read((char*)(&this->children[i]),sizeof(long));
    }

  }
};


using namespace std;

template<typename T>
class BPlusFile{
  string filename;
  string indexname;
  long page_size;
  int M;
  T index_atribute;
  long root;

public:
  BPlusFile(T _index_atribute, int _M = 3):root(-1), index_atribute(_index_atribute), M(_M){
    this->filename = "bplus_datos.dat";
    this->indexname = "index.dat";

    //definicion del page_size dependiendo del sistema operativo y la maquina.
    struct utsname unameData;
    if (uname(&unameData) == 0) {
      page_size = sysconf(_SC_PAGESIZE);
      //Si usas windows descomenta estas lineas de abajo y comenta la de arriba.
      /*
      SYSTEM_INFO system_info;
      GetSystemInfo(&system_info);
      DWORD p_size = system_info.dwPageSize;
      page_size = static_cast<long>(p_size);
      */
    } else {
      page_size = 4096;
    }

    this->M = this->page_size / sizeof(Node<T>);
    ofstream index(this->indexname, ios::binary | ios::app);
    ofstream file(this->filename, ios::binary | ios::app);
    
    index.close();
    file.close();
  }

  vector<Record1> search(T key){
    vector<Record1> res;
    ifstream index(this->indexname, ios::binary);
    long pos = search_recursive(root, key, index);

    if(pos == -1)
      return res;

  }
  
  bool add(Record1 record){
    add(root, record);
  }

  bool remove(Record1 record){

  }
  ~BPlusFile(){}

private:
  long search_recursive(long pos_node, T key, ifstream &index){
    if(pos_node == -1)
      return -1;
    int count = 0;
    index.read(reinterpret_cast<char*>(&count), sizeof(int));
    for (size_t i = 0; i < count; i++) {
      
    }
    index.seekg(sizeof(long), ios::cur);
  }
};
