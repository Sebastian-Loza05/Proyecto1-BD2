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

  void write(fstream &file){
    file.write(reinterpret_cast<const char*>(&count), sizeof(int));
    file.write(reinterpret_cast<const char*>(&m), sizeof(int));
    for (int i = 0; i < m; i++) {
      file.write((char*)&records[i], sizeof(Record1));
    }
    file.write(reinterpret_cast<char*>(&next), sizeof(long));
  } 

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
    this->children = new long[M];
    count = 0;
    pre_leaf = true;
  }

  void write(fstream &file, int M){
    file.write(reinterpret_cast<char*>(&count), sizeof(count));
    file.write(reinterpret_cast<char*>(&pre_leaf), sizeof(pre_leaf));
    for (int i = 0; i < M-1; i++) {
      file.write(reinterpret_cast<const char*>(&keys[i]), sizeof(TK));
    }
    for (int i = 0; i < M; i++) {
      file.write(reinterpret_cast<const char*>(&children[i]), sizeof(long));
    }
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
    ifstream data(this->filename, ios::binary);
    data.seekg(0, ios::end);
    if(data.tellg() == 0)
      return res;
    index.seekg(0, ios::end);
    long pos = 0;
    if(index.tellg() > 0){
      index.seekg(0, ios::beg);
      pos = search_recursive(root, key, index);
    }
    data.seekg(pos, ios::beg);
    Bucket bucket(page_size);
    bucket.read(data);
    data.close();
    index.close();
    for (int i = 0; i < bucket.count; i++) {
      if(bucket.records[i].key == key)
        res.push_back(bucket.records[i]);
    }
    data.close();
    index.close();
    return res;
  }
  
  bool add(Record1 record){
    add(root, record);
  }

  bool remove(TK key){
    ifstream indexR(this->indexname, ios::binary);
    if (!indexR.is_open()) throw ("No se puede abrir el archivo");
    fstream indexW(this->indexname, ios::binary | ios::in | ios::out);
    if (!indexW.is_open()) throw ("No se puede abrir el archivo");

    ifstream dataR(this->filename, ios::binary);
    if (!dataR.is_open()) throw ("No se puede abrir el archivo");
    fstream dataW(this->filename, ios::binary | ios::in | ios::out);
    if (!dataW.is_open()) throw ("No se puede abrir el archivo");

    dataR.seekg(0, ios::end);
    int tam1 = dataR.tellg();
    if (tam1 == 0) {
      return false;
    }

    indexR.seekg(0, ios::end);
    int tam2 = indexR.tellg();
    bool leaf = false;
    if (tam2 == 0) {
      leaf = true;
    }

    indexR.seekg(0, ios::beg);
    dataR.seekg(0, ios::beg);

    bool rpta = remove_recursive(root, key, -1, 0, leaf, indexR, indexW, dataR, dataW);
    

    indexR.close();
    indexW.close();

    dataR.close();
    dataW.close();
    
    return rpta;
  }
  ~BPlusFile(){}

private:

  

  bool remove_recursive(long pos_node, TK key, long pos_padre, int pos_child, bool leaf, ifstream &indexR, fstream &indexW, ifstream &dataR, fstream &dataW){
    if (leaf) {
      dataR.seekg(pos_node, ios::beg);
      dataW.seekg(pos_node, ios::beg);
      Bucket bucket(page_size);
      bucket.read(dataR);

      for (int i = 0; i < bucket.count; i++) {
        if (bucket.records[i].key == key) {
          delete_node(bucket, i, dataW);
          if (bucket.count < (M-1)/2 && (pos_node != root || bucket.count < 1)) {
            arreglar_node();
            return true;
          }
        }
      }
      return false;
    }
    else {

      indexR.seekg(pos_node, ios::beg);
      Node<TK> node(M);
      node.read(indexR, M);

      int bajada = -1;
  
      for (int i = 0; i < node.count; i++) {
        //delete_node(); // Lo hizo sebas
        if (key <= node.keys[i]){
          bajada = i;
          break;
        }
      }
    
      if (bajada == -1) bajada = node.count;
      bool rpta;
      if (node.pre_leaf) {
        rpta = remove_recursive(node.children[bajada], key, pos_node, bajada, true, indexR, indexW, dataR, dataW);
      }
      else {
        rpta = remove_recursive(node.children[bajada], key, pos_node, bajada, false, indexR, indexW, dataR, dataW);
      }

      if (node.count < (M-1)/2 && (pos_node != root || node.count < 1)) {
        arreglar_node();
        return true;
      }
      return rpta;
    }
  }
  long search_recursive(long pos_node, TK key, ifstream &index){
    index.seekg(pos_node, ios::beg);
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

  void delete_node(Bucket &bucket, int pos, fstream &dataW){
    for (int i = pos; i < bucket.count - 1; i++) {
      bucket.records[i] = bucket.records[i+1];
    }
    bucket.count--;
    bucket.write(dataW);
  }
};
