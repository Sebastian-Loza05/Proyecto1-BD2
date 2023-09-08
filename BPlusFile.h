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
  long next_del;

  Node(int M){
    this->keys = new TK[M];
    this->children = new long[M];
    count = 0;
    pre_leaf = true;
    next_del = -1;
  }

  void write(fstream &file, int M){
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    file.write(reinterpret_cast<const char*>(&pre_leaf), sizeof(pre_leaf));
    for (int i = 0; i < M-1; i++) {
      file.write(reinterpret_cast<const char*>(&keys[i]), sizeof(TK));
    }
    for (int i = 0; i < M; i++) {
      file.write(reinterpret_cast<const char*>(&children[i]), sizeof(long));
    }
    file.write(reinterpret_cast<char*>(&next_del), sizeof(long));
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
    file.read((char*)(&this->next_del), sizeof(long));
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
      page_size = 4092;
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
          if (bucket.count < (M)/2 && (pos_node != root || bucket.count < 1)) {
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

      if (node.count < (M)/2 && (pos_node != root || node.count < 1)) {
        arreglar_node();
        return true;
      }
      return rpta;
    }
  }

  void rotar_internos(long pos_node, long pos_node_prest, long pos_node_padre, int index, bool right, ifstream &indexR, fstream &indexW){
    // Lectura
    // Nodo que incumple
    indexR.seekg(pos_node);
    Node<TK> node(M);
    node.read(indexR, M);

    // Nodo prestamista
    indexR.seekg(pos_node_prest);
    Node<TK> node_prest(M);
    node.read(indexR, M);

    // Nodo padre 
    indexR.seekg(pos_node_padre);
    Node<TK> node_padre(M);
    node.read(indexR, M);
    if (right) {

      node.keys[node.count] = node_padre.keys[index];
      node_padre.keys[index] = node_prest.keys[0];
      for (int i = 0; i < node_prest.count-1; i++) {
        node_prest.keys[i] = node_prest.keys[i+1];
      }
      node_prest.count--;
      node.count++;
      // arreglando punteros
      node.children[node.count] = node_prest.children[0];
      for (int i = 0; i < node_prest.count+1; i++) {
        node_prest.children[i] = node_prest.children[i+1];
      }
      
    }
    else {
      for (int i = node.count-1; i > -1; i--) {
        node.keys[i+1] = node.keys[i];
      }
      node.keys[0] = node_padre.keys[index-1];
      node_prest.count--;
      node.count++;
      
      for (int i = node.count-1; i > -1 ; i--) {
        node.children[i+1] = node_prest.children[i];
      }
      node.children[0] = node_prest.children[node_prest.count+1];
    }
    // Escritura
    indexW.seekg(pos_node);
    node.write(indexW, M);

    indexW.seekg(pos_node_prest);
    node_prest.write(indexW, M);

    indexW.seekg(pos_node_padre);
    node_padre.write(indexW, M);
  }
 
  void rotar_hojas(long pos_bucket, long pos_bucket_prest, long pos_node_padre, int index, bool right,ifstream &indexR, fstream &indexW, ifstream &dataR, fstream &dataW){
    indexR.seekg(pos_node_padre);
    Node<TK> node_padre;
    node_padre.read(indexR, M);

    dataR.seekg(pos_bucket);
    Bucket bucket(page_size);
    bucket.read(dataR);

    dataR.seekg(pos_bucket_prest);
    Bucket bucket_prest(page_size);
    bucket_prest.read(dataR);

    if (right) {
      bucket.records[bucket.count] = bucket_prest.records[0];
      node_padre.keys[index] = bucket_prest.records[0].key;
      for (int i = 0; i < bucket_prest.count-1; i++) {
        bucket_prest.records[i] = bucket_prest.records[i+1];
      }
      bucket_prest.count--;
      bucket.count++;
    }
    else{
      for (int i = bucket.count; i > -1; i--) {
        bucket.records[i+1] = bucket.records[i];
      }
      bucket.records[0] = bucket_prest.records[bucket_prest.count-1];
      bucket_prest.count--;
      node_padre.keys[index-1] = bucket_prest.records[bucket_prest.count-1].key;
      bucket.count++;
    }
    indexW.seekg(pos_node_padre);
    node_padre.write(indexW, M);

    dataW.seekg(pos_bucket);
    bucket.write(dataW);

    dataW.seekg(pos_bucket_prest);
    bucket_prest.write(dataW);

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
