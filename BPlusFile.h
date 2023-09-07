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
  int count;
  long next;
  int m;

  Bucket(int page_size){
    count = 0;
    page_size -= sizeof(long) - (2*sizeof(int));
    this->m = page_size / sizeof(Record1);
    records = vector<Record1>(this->m);
  }

  void write(ofstream &file){
    file.write(reinterpret_cast<const char*>(&count), sizeof(int));
    file.write(reinterpret_cast<const char*>(&m), sizeof(int));
    for (int i = 0; i < m; i++) {
      file.write((char*)&records[i], sizeof(Record1));
    }
    file.write(reinterpret_cast<char*>(&next), sizeof(long));
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
    this->children = new long[M];
    count = 0;
    pre_leaf = true;
  }

  void write(ofstream &file, int M){
    file.write(reinterpret_cast<char*>(&count), sizeof(count));
    file.write(reinterpret_cast<char*>(&pre_leaf), sizeof(pre_leaf));
    for (int i = 0; i < M-1; i++) {
      file.write(reinterpret_cast<const char*>(&keys[i]), sizeof(TK));
    }
    for (int i = 0; i < M; i++) {
      file.write(reinterpret_cast<const char*>(&children[i]), sizeof(long));
    }
  }
};


using namespace std;

template<typename TK>
class BPlusFile{
  string filename;
  string indexname;
  long page_size;
  int M;
  TK index_atribute;
  long root;

public:
  BPlusFile(TK _index_atribute):root(-1), index_atribute(_index_atribute){
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
    
    int page_s = page_size - sizeof(int) - sizeof(bool);
    this->M  = (page_s + sizeof(TK))/(4+sizeof(TK)) ;
    ofstream index(this->indexname, ios::binary | ios::app);
    ofstream file(this->filename, ios::binary | ios::app);
    
    index.close();
    file.close();
  }

  vector<Record1> search(TK key){
    vector<Record1> res;
    ifstream index(this->indexname, ios::binary);
    index.seekg(0, ios::beg);
    long pos = search_recursive(root, key, index);
    if(pos == -1)
      return res;
    ifstream data(this->filename, ios::binary);
    data.seekg(pos, ios::beg);
    Bucket bucket(page_size);
    bucket.read(data);
    for (int i = 0; i < bucket.count; i++) {
      if(bucket.records[i].key == key)
        res.push_back(bucket.records[i]);
    }
    return res;
  }
  
  bool add(Record1 record){
    add(root, record);
  }

  bool remove(Record1 record){

  }
  ~BPlusFile(){}

private:
  long search_recursive(long pos_node, TK key, ifstream &index){
    if(pos_node == -1)
      return -1;
    Node<TK> node(M);
    node.read(index, M);
    int bajada = -1;
    for(int i = 0; i < node.count; i++){
      if(key <= node.keys[i]){
        bajada = i;
        break;
      }
    }
    if(bajada == -1) bajada = node.count;
    if(node.pre_leaf)
      return bajada;
    return search_recursive(node.children[bajada], key, index);
  }
};
