#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <sys/utsname.h>
// #include "methods.h"
#include "structs.h"
#include <vector>
#include <queue>
#include <stack>
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
    next = -1;
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

  void read(fstream &file){
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
    this->children = new long[M+1];
    count = 0;
    pre_leaf = true;
    next_del = -1;
  }

  void write(fstream &file, int M){
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    file.write(reinterpret_cast<const char*>(&pre_leaf), sizeof(pre_leaf));
    for (int i = 0; i < M; i++) {
      file.write(reinterpret_cast<const char*>(&keys[i]), sizeof(TK));
    }
    for (int i = 0; i < M+1; i++) {
      file.write(reinterpret_cast<const char*>(&children[i]), sizeof(long));
    }
    file.write(reinterpret_cast<char*>(&next_del), sizeof(long));
  }

  void read(fstream &file, int M){
    //count leaf keys children
    file.read((char*)(&this->count), sizeof(bool));
    file.read((char*)(&this->pre_leaf), sizeof(int));
    for (int i = 0; i < this->count; i++) {
      file.read((char*)(&this->keys[i]),sizeof(TK));
    }
    for (int i = 0; i < this->count+1; i++) {
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
  bool root_hoja;

public:
  BPlusFile(TK _index_atribute):root(-1), index_atribute(_index_atribute){
    this->filename = "bplus_datos.dat";
    this->indexname = "bplus_index.dat";

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
      file.write(reinterpret_cast<char*>(&value), sizeof(long));
    }
    
    index.close();
    file.close();
  }

  

  vector<Record1> search(TK key){
    vector<Record1> res;
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    fstream data(this->indexname, ios::binary | ios::in | ios::out);
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
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    if (!index.is_open()) throw ("No se puede abrir el archivo");

    fstream data(this->filename, ios::binary | ios::in | ios::out);
    if (!data.is_open()) throw ("No se puede abrir el archivo");

    if(root_hoja)
      return add_recursive(root, -1, record.key, record, true, index, data);
    return add_recursive(root, -1, record.key, record, false, index, data);
  }
  
  

  bool remove(TK key){
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    if (!index.is_open()) throw ("No se puede abrir el archivo");

    fstream data(this->filename, ios::binary | ios::in | ios::out);
    if (!data.is_open()) throw ("No se puede abrir el archivo");

    data.seekg(0, ios::end);
    int tam1 = data.tellg();
    if (tam1 == 4) {
      return false;
    }

    index.seekg(0, ios::end);
    int tam2 = index.tellg();
    bool leaf = false;
    if (tam2 == 4) {
      leaf = true;
    }

    index.seekg(0, ios::beg);
    data.seekg(0, ios::beg);

    bool rpta = remove_recursive(root, key, -1, 0, leaf, index, data);
    
    index.close();
    data.close();
    
    return rpta;
  }

  void displayTree(){
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    fstream data(this->filename, ios::binary | ios::in | ios::out);

    queue<long> queueq;
    queueq.push(this->root);
    stack<int> niveles;
    niveles.push(1);
    int i, j, n;
    while (!queueq.empty()) {
      n = queueq.size();
      for (i = 0; i < n; i++) {
        long pos_node = queueq.front();
        
      }
    }
  }
  
  ~BPlusFile(){}

private:
  
  bool add_recursive(long pos_node, long pos_padre, TK key, Record1 value, bool leaf, fstream &index, fstream &data){
    if(pos_node == -1){
      Bucket node(page_size);
      node.records[0] = value;
      node.count++;
      this->root = 4;
      data.seekp(4, ios::beg);
      node.write(data);
      root_hoja = true;
      return true;
    }
    if(leaf){
      bool correcto = insertar(pos_node, true, key, value, index, data);
      Bucket node(page_size);
      data.seekg(pos_node, ios::beg);
      node.read(data);
      if(node.count == M)
        split_hoja(pos_node, pos_padre, index, data);
      return correcto;
    }
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
    bool correcto;
    if(node.pre_leaf)
      correcto = add_recursive(node.children[bajada], pos_node, key, value, true, index, data);
    else
      correcto = add_recursive(node.children[bajada], pos_node, key, value, false, index, data);

    index.seekg(pos_node, ios::beg);
    node.read(index, M);

    if(node.count == M)
      split_interno(pos_node, pos_padre, index, data);
    return correcto;
  }

  bool insertar(long &pos_node, bool leaf, TK key, Record1 value, fstream index, fstream data){
    if (pos_node == -1) {  
      pos_node = 4;
      if (leaf) {
        Bucket node(page_size);
        node.records.push_back(value);
        node.count++;
        data.seekp(4, ios::beg);
        node.write(data);
        return true;
      }
      // Si es nodo interno 
      Node<TK> node(M);
      node.keys[0] = key;
      node.count++;
      index.seekp(4, ios::beg);
      node.write(index, M);  
      return true;
    }

    if (leaf) {
      Bucket node(page_size);
      data.seekg(pos_node, ios::beg);
      node.read(data);
      Record1 temp;
      for (int i = 0; i < node.count; i++) {
        if (key == node.records[i].key) {
          return false;
        }
        else if (key < node.records[i].key) {
          temp = node.records[i];
          node.records[i] = value;
          value = temp;
          key = temp.key;
        }
      }

      node.records[node.count] = value;
      node.count++;

      // Escribiendo valores;
      
      data.seekp(pos_node, ios::beg);
      node.write(data);
      return true;
    }
    // Si no es hoja 

    Node<TK> node(M);
    index.seekg(pos_node, ios::beg);
    node.read(index, M);

    long temp;
    for (int i = 0; i < node.count; i++) {
      if (key < node.keys[i]) {
        temp = node.keys[i];
        node.keys[i] = key;
        key = temp;
      }
    }

    node.keys[node.count] = key;
    node.count++;

    index.seekp(pos_node, ios::beg);
    node.write(index, M);    
    return true;
  }

  void split_hoja(long pos_node, long pos_padre, fstream &index, fstream &data){
    root_hoja = false;
    Bucket node1(page_size);
    Bucket node2(page_size);

    data.seekg(pos_node, ios::beg);
    Bucket node(page_size);
    node.read(data);
    
    node2.next = node.next;
    TK medio;

    for (int i = 0; i < node.count; i++) {
      if(i < (node.m-1)/2){
        node1.records[node1.count] = node.records[i];
        node1.count++;
      }
      else if(i == (node.m-1)/2){
        medio = node.records[i].key;
        insertar(pos_padre, true, medio, node.records[i], index, data);
        node1.records[node1.count] = node.records[i];
        node1.count++;
      }
      else{
        node2.records[node2.count] = node.records[i];
        node2.count++;
      }
    }
    anclar_hojas(node1, node2, pos_padre, medio, index, data);
  }

  void split_interno(long pos_node, long pos_padre, fstream &index, fstream &data){
    Node<TK> node1(M);
    Node<TK> node2(M);

    index.seekg(pos_node, ios::beg);
    Node<TK> node(M);
    node.read(index, M);
    
    if(node.pre_leaf){
      node1.pre_leaf = true;
      node2.pre_leaf = true;
    }
    TK medio;
    for (int i = 0; i < node.count; i++) {
      if(i < (M-1)/2){
        node1.keys[node1.count] = node.keys[i];
        node1.count++;
      }
      else if(i == (M-1)/2){
        medio = node.keys[i];
        insertar(pos_padre, false, medio, Record1());
      }
      else{
        node2.keys[node2.count] = node.keys[i];
        node2.count++;
      }
      ordenar_punteros(node1, node2, pos_node, index);
      anclar_interno(node1, node2, pos_padre, medio, index);
    }
  }

  int write_DataDel(Bucket bucket, fstream& data ){
    long value;
    data.seekg(0, ios::beg);
    data.read((char*)(&value), sizeof(long));
    int tam;
    if (value == -1) {
      data.seekg(0, ios::end);
      tam = data.tellg();

      // INSERTACION
      
      data.seekp(0, ios::end);
      bucket.write(data);
      return  tam;
    }
    // Existe header
    Bucket temp(page_size);
    data.seekg(value, ios::beg);
    tam = value;
    temp.read(data);
    long new_value = temp.next_del;
    data.seekp(0, ios::beg);
    data.write(reinterpret_cast<char*>(&new_value), sizeof(long));

    data.seekp(value, ios::beg);
    bucket.write(data);
    return tam;
  }

  int write_IndexDel(Node<TK>node, fstream& index){
    long value;
    index.seekg(0, ios::beg);
    index.read((char*)(&value), sizeof(long)) ;
    int indice, tam;
    // No hay  header
    if (value == -1) {
      index.seekg(0, ios::end);
      tam = index.tellg();

      // INSERTACION

      index.seekp(0, ios::end);
      node.write(index, M);
      return tam;
    }
    // Existe header
    Node<TK> temp(M);
    index.seekg(value, ios::beg);
    tam = value;
    temp.read(index, M);
    long new_value = temp.next_del;
    index.seekp(0, ios::beg);
    index.write(reinterpret_cast<char*>(&new_value), sizeof(long));

    index.seekp(value, ios::beg);
    node.write(index, M);

    return tam;
  }
  
  void anclar_internos(Node<TK> node1, Node<TK> node2, long pos_node, TK medio, fstream& index){
    index.seekg(pos_node, ios::beg);
    Node<TK> node(M);
    node.read(index, M);
    bool finded = false;
    long temp, act;
    int i = 0;
    for (i = 0 ; i < node.count; i++) {
      if (finded) {
        act = temp;
        temp = node.children[i];
        node.children[i] = act;
      }
      else if ( medio == node.keys[i]) {
        finded = true;
        node.children[i] = write_IndexDel(node1, index);
        i++;
        temp = node.children[i];
        node.children[i] = write_IndexDel(node2, index);
      }
    }
    node.children[node.count] = temp;
    index.seekp(pos_node, ios::beg);
    node.write(index, M);
    return;
  }
  // unir los buckets
  void anclar_hojas(Bucket b1, Bucket b2, long pos_node, TK medio, fstream& index, fstream& data){
    index.seekg(pos_node, ios::beg);
    Node<TK> node(M);
    node.read(index, M);
    if (pos_node == root) {
      node.pre_leaf = true;  
    }
    bool finded = false;
    long temp, act;
    int i = 0;
    for (int i = 0; i < node.count; i++) {
      if (finded) {
        act = temp;
        temp = node.children[i];
        node.children[i] = act;
      }
      else if (medio == node.keys[i]) {
        finded = true;
        long next_ = write_DataDel(b2, data);
        node.children[i+1] = next_;
        b1.next = next_;
        // node.children[i]
        data.seekp(node.children[i], ios::beg);
        b2.write(data);
        i++;
        temp = node.children[i];
      }
    }
    node.children[node.count] = temp;
    index.seekp(pos_node, ios::beg);
    node.write(index, M);
    return;
  }

  void ordenar_punteros(Node<TK> node1, Node<TK> node2, long pos_node, fstream &index){
    index.seekg(pos_node, ios::beg);
    Node<TK> node(M);
    node.read(index, M);
    for (int i = 0; i < node.count; i++) {
      if(i <= node1.count)
        node1.children[i] = node.children[i];
      else
        node2.children[i - node1.count - 1] = node.children[i];
    }
    return;
  }

  bool remove_recursive(long pos_node, TK key, long pos_padre, int pos_child, bool leaf, fstream &index, fstream &data){
    if (leaf) {
      data.seekg(pos_node, ios::beg);
      data.seekp(pos_node, ios::beg);
      Bucket bucket(page_size);
      bucket.read(data);  

      for (int i = 0; i < bucket.count; i++) {
        if (bucket.records[i].key == key) {
          delete_node(bucket, i, data);
          if (bucket.count < (M)/2 && (pos_node != root || bucket.count < 1)) {
            arreglar_node();
            return true;
          }
        }
      }
      return false;
    }
    else {
      index.seekg(pos_node, ios::beg);
      Node<TK> node(M);
      node.read(index, M);

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
        rpta = remove_recursive(node.children[bajada], key, pos_node, bajada, true, index, data);
      }
      else {
        rpta = remove_recursive(node.children[bajada], key, pos_node, bajada, false, index, data);
      }
      if (node.count < (M)/2 && (pos_node != root || node.count < 1)) {
        arreglar_node();
        return true;
      }
      return rpta;
    }
  }

  void delete_node(Bucket &bucket, int pos, fstream &data){
    for (int i = pos; i < bucket.count - 1; i++) {
      bucket.records[i] = bucket.records[i+1];
    }
    bucket.count--;
    bucket.write(data);
  }

  void arreglar_node(long pos_node, long pos_padre, int pos_child, bool leaf, fstream &index, fstream &data){
    if(pos_padre == -1){
      if(leaf){
        root = -1;
        return;
      }
      Node<TK> node(M);
      index.seekg(pos_node, ios::beg);
      node.read(index, M);
      index.seekg(node.children[0], ios::beg);
      Node<TK> nuevo_padre(M);
      nuevo_padre.read(index, M);
      index.seekg(4, ios::beg);
      nuevo_padre.write(index, M);
      return;
    }

    long left = -1;
    long right = -1;
    bool sibling_available = false;
    Node<TK> padre(M);
    if(leaf){
      Node<TK> node_l(M);
      Node<TK> node_r(M);
      index.seekg(pos_padre, ios::beg);
      padre.read(index, M);
      if(pos_child > 0){
        left = padre.children[pos_child-1];
        index.seekg(left, ios::beg);
        node_l.read(index, M);
      }
      if(pos_child < padre.count){
        right = padre.children[pos_child+1];
        index.seekg(right, ios::beg);
        node_r.read(index, M);
      }
      if(left != -1 && node_l.count > M/2){
        sibling_available = true;
        rotar_internos(pos_node, left, pos_padre, pos_child, false, index, index);
      }else if(right != -1 && node_r.count > M/2){
        sibling_available = true;
        rotar_internos(pos_node, right, pos_padre, pos_child, true, index, index);
      }
      if(sibling_available) return;
    }
      Bucket node_l(page_size);
      Bucket node_r(page_size);
      index.seekg(pos_padre, ios::beg);
      padre.read(index, M);
      if(pos_child > 0){
        left = padre.children[pos_child-1];
        data.seekg(left, ios::beg);
        node_l.read(index);
      }
      if(pos_child < padre.count){
        right = padre.children[pos_child+1];
        data.seekg(right, ios::beg);
        node_r.read(index);
      }
      if(left != -1 && node_l.count > M/2){
        sibling_available = true;
        rotar_hojas(pos_node, left, pos_padre, pos_child, false, data, data);
      }else if(right != -1 && node_r.count > M/2){
        sibling_available = true;
        rotar_hojas(pos_node, right, pos_padre, pos_child, true, data, data);
      }
      if(sibling_available) return;
    
  }

  void unir_en_hoja(long pos_node, long left, long right, long pos_padre, int pos_child, bool leaf, fstream &index, fstream &data){
    Bucket node(page_size);
    data.seekg(pos_node, ios::beg);
    node.read(data);

    Node<TK> padre(M);
    index.seekg(pos_padre, ios::beg);
    padre.read(index, M);

    long last_next_del;
    data.seekg(0, ios::beg);
    data.read(reinterpret_cast<char*>(&last_next_del), sizeof(long));

    if(left != -1){
      Bucket node_l(page_size);
      data.seekg(left, ios::beg);
      node_l.read(data);

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
      data.seekp(0, ios::beg);
      data.write(reinterpret_cast<const char*>(&pos_node), sizeof(long));
      node.next_del = last_next_del;

      //Reescritura de nodos actualizados.
      index.seekp(pos_padre, ios::beg);
      padre.write(index, M);
      data.seekp(pos_node, ios::beg);
      node.write(data);
      data.seekp(left, ios::beg);
      node_l.write(data);
      return;
    }
    Bucket node_r(page_size);
    data.seekp(right, ios::beg);
    node_r.read(data);
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
    data.seekp(0, ios::beg);
    data.write(reinterpret_cast<const char*>(&right), sizeof(long));
    node_r.next_del = last_next_del;

    //Reescritura de nodos actualizados.
    index.seekp(pos_padre, ios::beg);
    padre.write(index, M);
    data.seekp(pos_node, ios::beg);
    node.write(data);
    data.seekp(right, ios::beg);
    node_r.write(data);
    return;
  }

  void unir_en_interno(long pos_node, long left, long right, long pos_padre, int pos_child, bool leaf, fstream &index){
    Node<TK> padre(M);
    index.seekg(pos_padre, ios::beg);
    padre.read(index, M);

    Node<TK> node(M);
    index.seekg(pos_node, ios::beg);
    node.read(index, M);

    long last_next_del;
    index.seekg(0, ios::beg);
    index.read(reinterpret_cast<char*>(&last_next_del), sizeof(long));

    if(left != -1){
      Node<TK> node_l(M);
      index.seekg(left, ios::beg);
      node_l.read(index, M);
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
      index.seekp(0, ios::beg);
      index.write(reinterpret_cast<const char*>(&pos_node), sizeof(long));
      node.next_del = last_next_del;

      //Reescritura de nodos actualizados.
      index.seekp(pos_padre, ios::beg);
      padre.write(index, M);
      index.seekp(pos_node, ios::beg);
      node.write(index, M);
      index.seekp(left, ios::beg);
      node_l.write(index, M);
      return;
    }

    Node<TK> node_r(M);
    index.seekg(right, ios::beg);
    node_r.read(index, M);
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
    index.seekp(0, ios::beg);
    index.write(reinterpret_cast<const char*>(&right), sizeof(long));
    node_r.next_del = last_next_del;

    //Reescritura de nodos actualizados.
    index.seekp(pos_padre, ios::beg);
    padre.write(index, M);
    index.seekp(pos_node, ios::beg);
    node.write(index, M);
    index.seekp(right, ios::beg);
    node_r.write(index, M);
    return;
  }

  void rotar_internos(long pos_node, long pos_node_prest, long pos_node_padre, int index1, bool right, fstream &index){
    // Lectura
    // Nodo que incumple
    index.seekg(pos_node);
    Node<TK> node(M);
    node.read(index, M);

    // Nodo prestamista
    index.seekg(pos_node_prest);
    Node<TK> node_prest(M);
    node.read(index, M);

    // Nodo padre 
    index.seekg(pos_node_padre);
    Node<TK> node_padre(M);
    node.read(index, M);
    if (right) {

      node.keys[node.count] = node_padre.keys[index1];
      node_padre.keys[index1] = node_prest.keys[0];
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
      node.keys[0] = node_padre.keys[index1-1];
      node_prest.count--;
      node.count++;
      
      for (int i = node.count-1; i > -1 ; i--) {
        node.children[i+1] = node_prest.children[i];
      }
      node.children[0] = node_prest.children[node_prest.count+1];
    }
    // Escritura
    index.seekp(pos_node);
    node.write(index, M);

    index.seekp(pos_node_prest);

    node_prest.write(index, M);

    index.seekp(pos_node_padre);
    node_padre.write(index, M);
  }
 
  void rotar_hojas(long pos_bucket, long pos_bucket_prest, long pos_node_padre, int index1, bool right,fstream &index, fstream &data){
    index.seekg(pos_node_padre);
    Node<TK> node_padre;
    node_padre.read(index, M);

    data.seekp(pos_bucket);
    Bucket bucket(page_size);
    bucket.read(data);

    data.seekg(pos_bucket_prest);
    Bucket bucket_prest(page_size);
    bucket_prest.read(data);

    if (right) {
      bucket.records[bucket.count] = bucket_prest.records[0];
      node_padre.keys[index1] = bucket_prest.records[0].key;
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
      node_padre.keys[index1-1] = bucket_prest.records[bucket_prest.count-1].key;
      bucket.count++;
    }
    index.seekp(pos_node_padre);
    node_padre.write(index, M);

    data.seekp(pos_bucket);
    bucket.write(data);

    data.seekp(pos_bucket_prest);
    bucket_prest.write(data);
  }

  long search_recursive(long pos_node, TK key, fstream &index){
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
