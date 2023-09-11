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
  long next_del;

  Bucket(int page_size){
    count = 0;
    page_size -= sizeof(long) - (2*sizeof(int));
    this->m = page_size / sizeof(Record1);
    records = vector<Record1>(this->m);
    next_del = -1;
  }

  void write(fstream &file){
    file.write(reinterpret_cast<const char*>(&count), sizeof(int));
    file.write(reinterpret_cast<const char*>(&m), sizeof(int));
    for (int i = 0; i < m; i++) {
      file.write((char*)&records[i], sizeof(Record1));
    }
    file.write(reinterpret_cast<char*>(&next), sizeof(long));
    file.write(reinterpret_cast<char*>(&next_del), sizeof(long));
  } 

  void read(ifstream &file){
    //count m records next
    file.read((char*)(&this->count), sizeof(int));
    file.read((char*)(&this->m), sizeof(int));
    for (int i = 0; i < this->count; i++) {
      file.read((char*)(&this->records[i]), sizeof(Record1));
    }
    file.read((char*)(&this->next), sizeof(long));
    file.read((char*)(&this->next_del), sizeof(long));
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
    index.seekp(0,ios::end);
    int tam = index.tellp();
    if (tam == 0) {
      long value = -1;
      index.write(reinterpret_cast<char*>(&value), sizeof(long));
    }
    
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
    if (tam1 == 4) {
      return false;
    }

    indexR.seekg(0, ios::end);
    int tam2 = indexR.tellg();
    bool leaf = false;
    if (tam2 == 4) {
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

  void delete_node(Bucket &bucket, int pos, fstream &dataW){
    for (int i = pos; i < bucket.count - 1; i++) {
      bucket.records[i] = bucket.records[i+1];
    }
    bucket.count--;
    bucket.write(dataW);
  }

  void arreglar_node(long pos_node, long pos_padre, int pos_child, bool leaf, ifstream &indexR, fstream &indexW, ifstream &dataR, fstream &dataW){
    if(pos_padre == -1){
      if(leaf){
        root = -1;
        return;
      }
      Node<TK> node(M);
      indexR.seekg(pos_node, ios::beg);
      node.read(indexR, M);
      indexR.seekg(node.children[0], ios::beg);
      Node<TK> nuevo_padre(M);
      nuevo_padre.read(indexR, M);
      indexW.seekg(4, ios::beg);
      nuevo_padre.write(indexW, M);
      return;
    }

    long left = -1;
    long right = -1;
    bool sibling_available = false;
    Node<TK> padre(M);
    if(leaf){
      Node<TK> node_l(M);
      Node<TK> node_r(M);
      indexR.seekg(pos_padre, ios::beg);
      padre.read(indexR, M);
      if(pos_child > 0){
        left = padre.children[pos_child-1];
        indexR.seekg(left, ios::beg);
        node_l.read(indexR, M);
      }
      if(pos_child < padre.count){
        right = padre.children[pos_child+1];
        indexR.seekg(right, ios::beg);
        node_r.read(indexR, M);
      }
      if(left != -1 && node_l.count > M/2){
        sibling_available = true;
        rotar_internos(pos_node, left, pos_padre, pos_child, false, indexR, indexW);
      }else if(right != -1 && node_r.count > M/2){
        sibling_available = true;
        rotar_internos(pos_node, right, pos_padre, pos_child, true, indexR, indexW);
      }
      if(sibling_available) return;
    }
      Bucket node_l(page_size);
      Bucket node_r(page_size);
      indexR.seekg(pos_padre, ios::beg);
      padre.read(indexR, M);
      if(pos_child > 0){
        left = padre.children[pos_child-1];
        dataR.seekg(left, ios::beg);
        node_l.read(indexR);
      }
      if(pos_child < padre.count){
        right = padre.children[pos_child+1];
        dataR.seekg(right, ios::beg);
        node_r.read(indexR);
      }
      if(left != -1 && node_l.count > M/2){
        sibling_available = true;
        rotar_hojas(pos_node, left, pos_padre, pos_child, false, dataR, dataW);
      }else if(right != -1 && node_r.count > M/2){
        sibling_available = true;
        rotar_hojas(pos_node, right, pos_padre, pos_child, true, dataR, dataW);
      }
      if(sibling_available) return;
    
  }

  void unir_en_hoja(long pos_node, long left, long right, long pos_padre, int pos_child, bool leaf, ifstream &indexR, fstream &indexW, ifstream dataR, fstream dataW){
    Bucket node(page_size);
    dataR.seekg(pos_node, ios::beg);
    node.read(dataR);

    Node<TK> padre(M);
    indexR.seekg(pos_padre, ios::beg);
    padre.read(indexR, M);

    long last_next_del;
    dataR.seekg(0, ios::beg);
    dataR.read(reinterpret_cast<char*>(&last_next_del), sizeof(long));

    if(left != -1){
      Bucket node_l(page_size);
      dataR.seekg(left, ios::beg);
      node_l.read(dataR);

      for (int i = 0; i < node.count; i++) {
        node_l.records[node_l.count] = node.records[i];
        node_l.count++;
        node.count--;
      }
      node_l.next = node.next;
      for (int i = pos_child-1; i < padre.count - 1; i++) {
        padre.keys[i] = padre.keys[i+1];
      }
      for (int i = pos_child; i < padre.count + 1; i++) {
        padre.children[i] = padre.children[i+1];
      }
      //falta escribir en arcchivo
      dataW.seekg(0, ios::beg);
      dataW.write(reinterpret_cast<const char*>(&pos_node), sizeof(long));
      node.next_del = last_next_del;

      //Reescritura de nodos actualizados.
      indexW.seekg(pos_padre, ios::beg);
      padre.write(indexW, M);
      dataW.seekg(pos_node, ios::beg);
      node.write(dataW);
      dataW.seekg(left, ios::beg);
      node_l.write(dataW);
      return;
    }
    Bucket node_r(page_size);
    dataR.seekg(right, ios::beg);
    node_r.read(dataR);
    for (int i = 0; i < node_r.count; i++) {
      node.records[node.count] = node_r.records[i];
      node.count++;
      node_r.count--;
    }
    node.next = node_r.next;
    for (int i = pos_child; i < padre.count-1; i++) {
      padre.keys[i] = padre.keys[i+1];
    }
    padre.count--;
    for (int i = pos_child+1; i < padre.count+1; i++) {
      padre.children[i] = padre.children[i+1];
    }

    //Actualizacion del header
    dataW.seekg(0, ios::beg);
    dataW.write(reinterpret_cast<const char*>(&right), sizeof(long));
    node_r.next_del = last_next_del;

    //Reescritura de nodos actualizados.
    indexW.seekg(pos_padre, ios::beg);
    padre.write(indexW, M);
    dataW.seekg(pos_node, ios::beg);
    node.write(dataW);
    dataW.seekg(right, ios::beg);
    node_r.write(dataW);
    return;
  }

  void unir_en_interno(long pos_node, long left, long right, long pos_padre, int pos_child, bool leaf, ifstream &indexR, fstream &indexW){
    Node<TK> padre(M);
    indexR.seekg(pos_padre, ios::beg);
    padre.read(indexR, M);

    Node<TK> node(M);
    indexR.seekg(pos_node, ios::beg);
    node.read(indexR, M);

    long last_next_del;
    indexR.seekg(0, ios::beg);
    indexR.read(reinterpret_cast<char*>(&last_next_del), sizeof(long));

    if(left != -1){
      Node<TK> node_l(M);
      indexR.seekg(left, ios::beg);
      node_l.read(indexR, M);
      node_l.keys[node_l.count] = padre.keys[pos_child-1];
      node_l.count++;

      node_l.children[node_l.count] = node.children[0];

      for (int i = 0; i < node.count; i++) {
        node_l.keys[node_l.count] = node.keys[i];
        node_l.count++;
        node.count--;
        node_l.children[node_l.count] = node.children[i+1];
      }
      for (int i = pos_child-1; i < padre.count-1; i++) {
        padre.keys[i] = padre.keys[i+1];
      }
      padre.count--;
      for (int i = pos_child; i < padre.count+1; i++) {
        padre.children[i] = padre.children[i+1];
      }
    //Escritura en archivo;
    
    //Actualizacion del header
      indexW.seekg(0, ios::beg);
      indexW.write(reinterpret_cast<const char*>(&pos_node), sizeof(long));
      node.next_del = last_next_del;

      //Reescritura de nodos actualizados.
      indexW.seekg(pos_padre, ios::beg);
      padre.write(indexW, M);
      indexW.seekg(pos_node, ios::beg);
      node.write(indexW, M);
      indexW.seekg(left, ios::beg);
      node_l.write(indexW, M);
      return;
    }

    Node<TK> node_r(M);
    indexR.seekg(right, ios::beg);
    node_r.read(indexR, M);
    node.keys[node.count] = padre.keys[pos_child];
    node.count++;

    node.children[node.count] = node_r.children[0];

    for (int i = 0; i < node_r.count; i++) {
      node.keys[node.count] = node_r.keys[i];
      node.count++;
      node_r.count--;
      node.children[node.count] = node_r.children[i+1];
    }
    for (int i = pos_child; i < padre.count-1; i++) {
      padre.keys[i] = padre.keys[i+1];
    }
    padre.count--;
    for (int i = pos_child+1; i < padre.count+1; i++) {
      padre.children[i] = padre.children[i+1];
    }
    //Escritura en archivo;
    
    //Actualizacion del header
    indexW.seekg(0, ios::beg);
    indexW.write(reinterpret_cast<const char*>(&right), sizeof(long));
    node_r.next_del = last_next_del;

    //Reescritura de nodos actualizados.
    indexW.seekg(pos_padre, ios::beg);
    padre.write(indexW, M);
    indexW.seekg(pos_node, ios::beg);
    node.write(indexW, M);
    indexW.seekg(right, ios::beg);
    node_r.write(indexW, M);
    return;
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
};
