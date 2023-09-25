#pragma once
#include "Structures/Record.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <memory>
#include <strings.h>
//si usas windows comenta la linea de abajo
//#include <sys/utsname.h>
#include "Structures/methods.h"
// #include "structs.h"
#include <vector>
#include <queue>
#include <stack>
//#include <unistd.h>
//Si usas windows descomenta la linea de abajo y comenta la de arriba.
//#include <windows.h>


template<typename R>
struct Bucket{
  vector<R> records;
  int count;
  long next;
  int m;
  long next_del;

  Bucket(int page_size){
    count = 0;
    page_size -= (sizeof(long) + (2*sizeof(int)));
    this->m = page_size / sizeof(R);
    //this->m = 5;
    records = vector<R>(this->m);
    next_del = -1;
    next = -1;
  }

  void write(fstream &file){
    file.write(reinterpret_cast<char*>(&count), sizeof(int));
    file.write(reinterpret_cast<char*>(&m), sizeof(int));
    for (int i = 0; i < m; i++) {
      file.write((char*)&records[i], sizeof(R));
      // cout << "-----WRITE----"<< endl;
      // records[i].print();
    }
    file.write(reinterpret_cast<const char*>(&next), sizeof(long));
    file.write(reinterpret_cast<const char*>(&next_del), sizeof(long));
  } 

  void read(fstream &file){
    file.read((char*)(&this->count), sizeof(int));
    file.read((char*)(&this->m), sizeof(int));
    for (int i = 0; i < this->m; i++) {
      file.read(reinterpret_cast<char* >(&this->records[i]), sizeof(R));
      // file.read((char*)(&this->records[i]), sizeof(Record));
    }
    file.read((char*)(&this->next), sizeof(long));
    file.read((char*)(&this->next_del), sizeof(long));
  }

};


template<typename R>
inline ostream& operator<<(ostream& os, Bucket<R> buck){
  os<<"count: "<<buck.count<<endl;
  os<<"next: "<<buck.next<<endl;
  os<<"m: "<<buck.m<<endl;
  os<<"next_del: "<<buck.next_del<<endl;
  os<<"keys: "<<endl; 
  for (int i = 0; i < buck.count; i++) {
    os<<buck.records[i].key<<", ";
  }
  return os;
}

template<typename TK, size_t n>
struct Node{
  TK* keys;
  long* children;
  int count;
  bool pre_leaf;
  long next_del;

  Node(int M){
    inicializarKeys(this->keys, M, n);

    this->children = new long[M+1];
    count = 0;
    pre_leaf = false;
    next_del = -1;
  }

  void write(fstream &file, int M){

    file.write(reinterpret_cast<const char*>(&count), sizeof(int));
    file.write(reinterpret_cast<const char*>(&pre_leaf), sizeof(bool));
    for (int i = 0; i < M; i++) {
      escribirN(file, keys[i], n);
    }
    for (int i = 0; i < M+1; i++) {
      file.write(reinterpret_cast<const char*>(&children[i]), sizeof(long));
    }
    file.write(reinterpret_cast<const char*>(&next_del), sizeof(long));
  }

  void read(fstream &file, int M){
    file.read((char*)(&this->count), sizeof(int));
    file.read((char*)(&this->pre_leaf), sizeof(bool));
    for (int i = 0; i < M; i++) {
      leerN(file, keys[i], n);
    }
    for (int i = 0; i < M+1; i++) {
      file.read((char*)(&this->children[i]),sizeof(long));
    }
    file.read((char*)(&this->next_del), sizeof(long));
  }

  void print(){
    cout<<"count: "<<count<<endl;
    cout<<"pre_leaf: "<<pre_leaf<<endl;
    cout<<"next_del: "<<next_del<<endl;
    cout<<"keys: "; 
    for (int i = 0; i < count; i++) {
      cout<<keys[i]<<", ";
    }
    cout<<endl;
    cout<<"children: "; 
    for (int i = 0; i < count+1; i++) {
      cout<<children[i]<<", ";
    }
    cout<<endl;
  }

};



template<typename R, typename TK, size_t N>
class BPlusFile : public MethodSelector<R>{
  string filename;
  string indexname;
  long page_size;
  int M;
  TK index_atribute;
  long root;
  bool root_hoja;

public:
  BPlusFile(){
    this->filename = "bplus_datos.dat";
    this->indexname = "bplus_index.dat";

    //definicion del page_size dependiendo del sistema operativo y la maquina.
    /*struct utsname unameData;
    if (uname(&unameData) == 0) {
      //page_size = sysconf(_SC_PAGESIZE);
    } else {
      page_size = 4092;
    }*/
    //Si usas windows descomenta estas lineas de abajo y comenta las de arriba.
    /*SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    DWORD p_size = system_info.dwPageSize;
    page_size = static_cast<long>(p_size);
    */
    page_size=4092;

    int page_s = page_size - sizeof(long) - sizeof(bool) - sizeof(int) + N;
    int X  = (page_s)/(N + sizeof(long)) ;
    this->M = X-1;
    //this->M = 5;
    ofstream index(this->indexname, ios::binary | ios::app);
    ofstream file(this->filename, ios::binary | ios::app);
    // cout<<"Se agrego"<<endl;
    file.seekp(0,ios::end);
    index.seekp(0,ios::end);
    int tam1 = file.tellp();
    int tam2 = index.tellp();

    long value = -1;
    if (tam2 == 0) {      
      index.write(reinterpret_cast<char*>(&value), sizeof(long));
    }
    if (tam1 == 0) {
      root = -1;
      file.write(reinterpret_cast<char*>(&value), sizeof(long));
    }
    else{
      fstream index1(this->indexname, ios::binary | ios::in | ios::out);
      fstream file1(this->indexname, ios::binary | ios::in | ios::out);
      index1.seekp(0, ios::end);
      int tamI = index1.tellp();
      if (tamI > sizeof(long)) {
        index1.seekp(0, ios::beg);
        long del_act;
        index1.read((char*)(&del_act), sizeof(long));

        Node<TK,N> node(M);
        index1.seekg(sizeof(long), ios::beg);
        node.read(index1, M);
        if(node.next_del == -1){
          root_hoja = false;
          root = sizeof(long);
        }
        else{
          root_hoja = true;
          file1.seekg(0, ios::beg);
          long value_data = -1;
          file1.read((char*)(&value_data), sizeof(long));
          if(value_data == sizeof(long))
            root = -1;
          else 
            root = sizeof(long);
        }
      }
      else{
        root_hoja = true;
        root = sizeof(long);
      }
      index1.close();
      file1.close();
    }
    index.close();
    file.close();
  }

  vector<R> rangeSearch(TK min, TK max) override{
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    if (!index.is_open()) throw ("No se puede abrir el archivo");

    fstream data(this->filename, ios::binary | ios::in | ios::out);
    if (!data.is_open()) throw ("No se puede abrir el archivo");
    vector<R> res;

    data.seekg(0, ios::end);
    if(data.tellg() <= sizeof(long))
      // data vacia
      return res;

    long pos = sizeof(long);
  
    index.seekg(0, ios::end);
    if (index.tellg() > sizeof(long)) {
      index.seekg(sizeof(long), ios::beg);
      pos = search_recursive(root, min, index);
    }
    cout << "Pos: " << pos << endl;
    long next_exist = pos;
    Bucket<R> bucket(page_size);
    
    while (next_exist != -1) {
      data.seekg(next_exist, ios::beg);
      bucket.read(data);
      for (int i = 0; i < bucket.count; i++) {
        if ( menor_igual(min, bucket.records[i].key) && menor_igual(bucket.records[i].key, max) ){
        // if (min <= bucket.records[i].key && bucket.records[i].key <= max) {
          res.push_back(bucket.records[i]);
        }
      }
      next_exist = bucket.next;
    }

    index.close();
    data.close();

    return res;

  } 

  pair<R,bool> search(TK key) override{
    R res;
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    fstream data(this->indexname, ios::binary | ios::in | ios::out);
    data.seekg(sizeof(long), ios::end);
    if(data.tellg() == sizeof(long))
      return make_pair(R(), false);

    index.seekg(0, ios::end);
    long pos = sizeof(long);
    // Existen Nodos y no solo hojas
    if(index.tellg() > sizeof(long)){
      index.seekg(sizeof(long), ios::beg);
      pos = search_recursive(root, key, index);
    }
    data.seekg(pos, ios::beg);
    Bucket<R> bucket(page_size);
    bucket.read(data);
    data.close();
    index.close();
    for (int i = 0; i < bucket.count; i++) {
      if ( igual_igual(bucket.records[i].key , key) )
      // if(bucket.records[i].key == key)
        res = bucket.records[i];
    }
    data.close();
    index.close();
    return make_pair(res, true);
  }

  bool add(R record) override{

    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    if (!index.is_open()) throw ("No se puede abrir el archivo");

    fstream data(this->filename, ios::binary | ios::in | ios::out);
    if (!data.is_open()) throw ("No se puede abrir el archivo");

    index.seekg(0, ios::end);
    index.seekg(0, ios::beg);  
    bool res = false;
    if(root_hoja)
       res = add_recursive(root, -1, record.key, record, true, index, data);
    else res = add_recursive(root, -1, record.key, record, false, index, data);
    index.close();
    data.close();
    return res;
  }
  
  

  bool remove(TK key) override{
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    if (!index.is_open()) throw ("No se puede abrir el archivo");

    fstream data(this->filename, ios::binary | ios::in | ios::out);
    if (!data.is_open()) throw ("No se puede abrir el archivo");

    data.seekg(0, ios::end);
    int tam1 = data.tellg();
    if (tam1 == sizeof(long)) {
      return false;
    }

    index.seekg(0, ios::end);
    int tam2 = index.tellg();
    bool leaf = false;
    if (tam2 == sizeof(long)) {
      leaf = true;
    }

    index.seekg(0, ios::beg);
    data.seekg(0, ios::beg);
    
    bool rpta;
    if(root_hoja)
      rpta = remove_recursive(root, key, -1, 0, true, index, data);
    else
      rpta = remove_recursive(root, key, -1, 0, leaf, index, data);
    
    index.close();
    data.close();
    
    return rpta;
  }

  void toString(){
    fstream data(this->filename, ios::binary | ios::in | ios::out);
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    if(!root_hoja){
      Node<TK, N> temp1(M);
      // Node<TK> temp1(M);
      index.seekg(root, ios::beg);
      temp1.read(index, M);
      while (!temp1.pre_leaf){
        index.seekg(temp1.children[0], ios::beg);
        temp1.read(index, M);
      }
      data.seekg(temp1.children[0], ios::beg);
    }
    else{
      data.seekg(sizeof(long), ios::beg);
    }
    Bucket<R> temp(page_size);
    temp.read(data);
    int contador = 0;
    while(temp.next != -1){
      cout<< "<";
      for(int j = 0; j < temp.count; j++ ){
        cout << temp.records[j].key << ",";
        contador++;
      }
      cout << "> ";
      data.seekg(temp.next, ios::beg);
      temp.read(data);
    }
    cout<< "<";
    for(int j = 0; j < temp.count; j++ ){
      cout << temp.records[j].key << ",";
      contador++;
    }
    cout << "> ";
    cout<<endl;
    cout<<"contador: "<<contador<<endl;
    data.close();
    index.close();
  }

  vector<R> load() override {
    fstream data(this->filename, ios::binary | ios::in | ios::out);
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    if(!root_hoja){
      Node<TK, N> temp1(M);
      // Node<TK> temp1(M);
      index.seekg(root, ios::beg);
      temp1.read(index, M);
      while (!temp1.pre_leaf){
        index.seekg(temp1.children[0], ios::beg);
        temp1.read(index, M);
      }
      data.seekg(temp1.children[0], ios::beg);
    }
    else{
      data.seekg(sizeof(long), ios::beg);
    }
    Bucket<R> temp(page_size);
    temp.read(data);
    int contador = 0;
    vector<R> res;
    while(temp.next != -1){
      for(int j = 0; j < temp.count; j++ ){
        res.push_back(temp.records[j]);
        contador++;
      }
      data.seekg(temp.next, ios::beg);
      temp.read(data);
    }
    for(int j = 0; j < temp.count; j++ ){
      res.push_back(temp.records[j]);
      contador++;
    }
    cout<<"contador: "<<contador<<endl;
    data.close();
    index.close();
    return res;
  }

  void display_all() override{
    fstream index(this->indexname, ios::binary | ios::in | ios::out);
    fstream data(this->filename, ios::binary | ios::in | ios::out);
    // return;
    if(!root_hoja){
      queue<long> queueq;
      queueq.push(this->root);
      stack<int> niveles;
      niveles.push(1);
      int i, j, n;
      while (!queueq.empty()) {
        n = queueq.size();
        for (i = 0; i < n; i++) {
          long pos_node = queueq.front();
          index.seekg(pos_node, ios::beg);
          Node<TK, N> node(M);
          // Node<TK> node(M);
          node.read(index, M);
          cout<< "<";
          for(j = 0; j < node.count; j++ )
            cout << node.keys[j] << ",";
          cout << "> ";
          queueq.pop();
          if(!node.pre_leaf){
            queueq.push(node.children[0]);
            for(int j = 1; j <= node.count; j++)
              queueq.push(node.children[j]);
          }
        }
        cout << endl;
      }
    }
    toString();
  }
  
  ~BPlusFile(){}

private:
  
  bool add_recursive(long pos_node, long pos_padre, TK key, R value, bool leaf, fstream &index, fstream &data){
    if(pos_node == -1){
      Bucket<R> node(page_size);
      node.records[0] = value;
      node.count++;
      this->root = sizeof(long);
      write_DataDel(node, data);
      root_hoja = true;
      return true;
    }
    if(leaf){
      bool correcto = insertar(pos_node, true, key, value, index, data);
      Bucket<R> node(page_size);
      data.seekg(pos_node, ios::beg);
      node.read(data);
      if(node.count == node.m)
        split_hoja(pos_node, pos_padre, index, data);
      return correcto;
    }
    index.seekg(pos_node, ios::beg);
    Node<TK, N> node(M);
    // Node<TK> node(M);
    node.read(index, M);

    int bajada = -1;
    for(int i = 0; i < node.count; i++){
      if (menor_igual(key, node.keys[i])){
      // if(key <= node.keys[i]){
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

  bool insertar(long &pos_node, bool leaf, TK key, R value, fstream &index, fstream &data){
    if (pos_node == -1) {  
      pos_node = sizeof(long);
      // Si es nodo interno 
      Node<TK, N> node(M);
      Node<TK, N> temp(M);
      if(root_hoja){
        node.pre_leaf = true;
        root_hoja = false;
        long header;
        index.seekg(0, ios::beg);
        index.read((char*)&header, sizeof(long));
        // cout<<"Header: "<<header<<endl;
        if(header != -1){
          index.seekg(header, ios::beg);
          temp.read(index, M);
          index.seekp(0, ios::beg);
          index.write(reinterpret_cast<char*>(&temp.next_del), sizeof(long));
        }
      }
      node.keys[0] = key;
      node.count++;
      index.seekp(sizeof(long), ios::beg);
      node.write(index, M);  

      // Node<TK, N> n1(M);
      // index.seekg(sizeof(long), ios::beg);
      // n1.read(index, M);
      // cout<<"\nNuevo root"<<endl;
      // n1.print();
      // cout<<endl;
      // exit(0);
      return true;
    }

    if (leaf) {
      Bucket<R> node(page_size);
      data.seekg(pos_node, ios::beg);
      node.read(data);
      R temp;
      for (int i = 0; i < node.count; i++) {
        if (igual_igual(key, node.records[i].key)){
          return false;
        }
        else if (menor(key, node.records[i].key)){
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

    Node<TK, N> node(M);
    index.seekg(pos_node, ios::beg);
    node.read(index, M);

    TK temp;
    for (int i = 0; i < node.count; i++) {
      if (menor(key, node.keys[i])){
      // if (key < node.keys[i]) {
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
    Bucket<R> node1(page_size);
    Bucket<R> node2(page_size);

    data.seekg(pos_node, ios::beg);
    Bucket<R> node(page_size);
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
        insertar(pos_padre, false, medio, node.records[i], index, data);
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
    Node<TK, N> node1(M);
    Node<TK, N> node2(M);

    index.seekg(pos_node, ios::beg);
    Node<TK, N> node(M);
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
        insertar(pos_padre, false, medio, R(),index, data);
      }
      else{
        node2.keys[node2.count] = node.keys[i];
        node2.count++;
      }
    }
    ordenar_punteros(node1, node2, node, index);
    anclar_internos(node1, node2, pos_padre, medio, index);
    // exit(0);
  }

  int write_DataDel(Bucket<R> bucket, fstream& data ){
    long value;
    data.seekg(0, ios::beg);
    data.read((char*)(&value), sizeof(long));
    int tam;
    if (value == -1) {
      // INSERTACION
      data.seekp(0, ios::end);
      tam = data.tellp();
      bucket.write(data);
      return  tam;
    }
    // Existe header
    Bucket<R> temp(page_size);
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

  int write_IndexDel(Node<TK, N>node, fstream& index){
    long value;
    index.seekg(0, ios::beg);
    index.read((char*)(&value), sizeof(long)) ;
    int indice, tam;
    // No hay  header
    if (value == -1) {
      // INSERTACION

      index.seekp(0, ios::end);
      tam = index.tellp();
      node.write(index, M);
      return tam;
    }
    // Existe header
    Node<TK, N> temp(M);
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
  
  void anclar_internos(Node<TK, N> node1, Node<TK, N> node2, long pos_node, TK medio, fstream& index){
    index.seekg(pos_node, ios::beg);
    Node<TK, N> node(M);
    node.read(index, M);
    node.pre_leaf = false;
    bool finded = false;
    long temp, act;
    int i = 0;
    for (i = 0 ; i < node.count; i++) {
      if (finded) {
        act = temp;
        temp = node.children[i];
        node.children[i] = act;
      }
      else if ( igual_igual(medio, node.keys[i]) ){
      // else if ( medio == node.keys[i]) {
        finded = true;
        node.children[i] = write_IndexDel(node1, index);
        ++i;
        temp = node.children[i];
        node.children[i] = write_IndexDel(node2, index);
        if(i==node.count) temp = node.children[i];
      }
    }
    node.children[node.count] = temp;
    index.seekp(pos_node, ios::beg);
    node.write(index, M);
    return;
  }
  // unir los buckets
  void anclar_hojas(Bucket<R> b1, Bucket<R> b2, long pos_node, TK medio, fstream& index, fstream& data){
    index.seekg(pos_node, ios::beg);
    Node<TK, N> node(M);
    // Node<TK> node(M);
    node.read(index, M);
    if (pos_node == root) {
      node.pre_leaf = true;  
    }

    if(node.count == 1){
      node.children[0] = sizeof(long);
      long next_ = write_DataDel(b2, data);
      data.seekp(sizeof(long), ios::beg);
      node.children[1] = next_;
      b1.next = next_;
      b1.write(data);
    }
    else{
      bool finded = false;
      long temp, act;
      int i = 0;
      for (int i = 0; i < node.count; i++) {
        if (finded) {
          act = temp;
          temp = node.children[i];
          node.children[i] = act;
        }
        else if ( igual_igual(medio, node.keys[i]) ){
        // else if (medio == node.keys[i]) {
          finded = true;
          long next_ = write_DataDel(b2, data);
          temp = node.children[i+1];
          node.children[i+1] = next_;
          b1.next = next_;
          data.seekp(node.children[i], ios::beg);
          b1.write(data);
          i++;
          if(i==node.count) temp = node.children[i];
        }
      }
      node.children[node.count] = temp;
    }
    index.seekp(pos_node, ios::beg);
    node.write(index, M);
    return;
  }

  void ordenar_punteros(Node<TK, N> &node1, Node<TK, N> &node2, Node<TK, N> node, fstream &index){
    for (int i = 0; i < node.count+1; i++) {
      if(i <= node1.count)
        node1.children[i] = node.children[i];
      else
        node2.children[i - node1.count - 1] = node.children[i];
    }
    return;
  }

  bool remove_recursive(long pos_node, TK key, long pos_padre, int pos_child, bool leaf, fstream &index, fstream &data){
    if (pos_node == -1)
      return true;

    if (leaf) {
      data.seekg(pos_node, ios::beg);
      Bucket<R> bucket(page_size);
      bucket.read(data);  

      for (int i = 0; i < bucket.count; i++) {
        if ( igual_igual(bucket.records[i].key, key) ){
          delete_node(bucket, pos_node, i, data);
          data.seekg(pos_node, ios::beg);
          bucket.read(data);  
          if (bucket.count < (bucket.m-1)/2 && ((pos_node != root || !root_hoja) || bucket.count < 1)) {
            arreglar_node(pos_node, pos_padre, pos_child, true, index, data);
            return true;
          }
        }
      }
      return false;
    }
    else {
      index.seekg(pos_node, ios::beg);
      Node<TK, N> node(M);
      node.read(index, M);

      int bajada = -1;
  
      for (int i = 0; i < node.count; i++) {
        if (menor_igual(key, node.keys[i]) ){
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
      index.seekg(pos_node, ios::beg);
      node.read(index, M);
      if (node.count < (M-1)/2 && (pos_node != root || node.count < 1)) {
        arreglar_node(pos_node, pos_padre, pos_child, false, index, data);
        return true;
      }
      return rpta;
    }
  }

  void delete_node(Bucket<R> bucket, long pos_node, int pos, fstream &data){
    for (int i = pos; i < bucket.count - 1; i++) {
      bucket.records[i] = bucket.records[i+1];
    }
    bucket.count--;
    data.seekg(pos_node, ios::beg);
    bucket.write(data);
  }

  void arreglar_node(long pos_node, long pos_padre, int pos_child, bool leaf, fstream &index, fstream &data){
    if(pos_padre == -1){
      if(leaf){
        this->root = -1;
        long value;
        data.seekg(0, ios::beg);
        data.read((char*)(&value), sizeof(long));
        data.seekg(pos_node, ios::beg);
        Bucket<R> node_root(page_size);
        node_root.read(data);
        node_root.next_del = value;
        data.seekp(0, ios::beg);
        long newHeader = sizeof(long);
        data.write(reinterpret_cast<char*>(&newHeader), sizeof(long));
        node_root.write(data);
        return;
      }
      Node<TK, N> node(M);
      index.seekg(pos_node, ios::beg);
      node.read(index, M);

      //leer el header
      long value;
      index.seekg(0, ios::beg);
      index.read((char*)(&value), sizeof(long)) ;
      if(node.pre_leaf){
        root = node.children[0];
        root_hoja = true;
        node.next_del = value;
        index.seekp(0, ios::beg);
        index.write(reinterpret_cast<char*>(&pos_node), sizeof(long));
        index.seekg(sizeof(long), ios::beg);
        node.write(index, M);
      }
      index.seekg(node.children[0], ios::beg);
      Node<TK, N> nuevo_padre(M);
      nuevo_padre.read(index, M);
      index.seekp(sizeof(long), ios::beg);
      nuevo_padre.write(index, M);
      //actualizar next_del
      nuevo_padre.next_del = value;
      index.seekp(node.children[0], ios::beg);
      nuevo_padre.write(index, M);
      index.seekp(0, ios::beg);
      index.write(reinterpret_cast<char*>(&node.children[0]), sizeof(long));

      return;
    }

    long left = -1;
    long right = -1;
    bool sibling_available = false;
    Node<TK, N> padre(M);
    index.seekg(pos_padre, ios::beg);
    padre.read(index, M);
    if(!leaf){
      Node<TK, N> node_l(M);
      Node<TK, N> node_r(M);
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
      if(left != -1 && node_l.count > (M-1)/2){
        sibling_available = true;
        rotar_internos(pos_node, left, pos_padre, pos_child, false, index);
      }else if(right != -1 && node_r.count > (M-1)/2){
        sibling_available = true;
        rotar_internos(pos_node, right, pos_padre, pos_child, true, index);
      }
      if(sibling_available) return;
    }
    else{
      Bucket<R> node_l(page_size);
      Bucket<R> node_r(page_size);
      if(pos_child > 0){
        left = padre.children[pos_child-1];
        data.seekg(left, ios::beg);
        node_l.read(data);
      }
      if(pos_child < padre.count){
        right = padre.children[pos_child+1];
        data.seekg(right, ios::beg);
        node_r.read(data);
      }
      if(left != -1 && node_l.count > (node_l.m-1)/2){
        sibling_available = true;
        rotar_hojas(pos_node, left, pos_padre, pos_child, false, index, data);
      }else if(right != -1 && node_r.count > (node_r.m-1)/2){
        sibling_available = true;
        rotar_hojas(pos_node, right, pos_padre, pos_child, true, index, data);
      }
      if(sibling_available) return;
    }
    if(leaf){
      unir_en_hoja(pos_node, left, right, pos_padre, pos_child, index, data);
      return;
    }
    unir_en_interno(pos_node, left, right, pos_padre, pos_child, index);
    return;
    
  }

  void unir_en_hoja(long pos_node, long left, long right, long pos_padre, int pos_child, fstream &index, fstream &data){
    Bucket<R> node(page_size);
    data.seekg(pos_node, ios::beg);
    node.read(data);

    Node<TK, N> padre(M);
    index.seekg(pos_padre, ios::beg);
    padre.read(index, M);

    long last_next_del;
    data.seekg(0, ios::beg);
    data.read(reinterpret_cast<char*>(&last_next_del), sizeof(long));

    if(left != -1){
      Bucket<R> node_l(page_size);
      data.seekg(left, ios::beg);
      node_l.read(data);
      int tam = node.count;
      for (int i = 0; i < tam; i++) {
        node_l.records[node_l.count] = node.records[i];
        node_l.count++;
        node.count--;
      }
      node_l.next = node.next;
      for (int i = pos_child-1; i < padre.count - 1; i++) {
        padre.keys[i] = padre.keys[i+1];
      }
      padre.count--;
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
    Bucket<R> node_r(page_size);
    data.seekp(right, ios::beg);
    node_r.read(data);
    int tam = node_r.count;
    for (int i = 0; i < tam; i++) {
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

  void unir_en_interno(long pos_node, long left, long right, long pos_padre, int pos_child, fstream &index){

    Node<TK, N> padre(M);
    index.seekg(pos_padre, ios::beg);
    padre.read(index, M);
    
    Node<TK, N> node(M);
    index.seekg(pos_node, ios::beg);
    node.read(index, M);

    long last_next_del;
    index.seekg(0, ios::beg);
    index.read(reinterpret_cast<char*>(&last_next_del), sizeof(long));

    if(left != -1){
      Node<TK, N> node_l(M);
      index.seekg(left, ios::beg);
      node_l.read(index, M);
      node_l.keys[node_l.count] = padre.keys[pos_child-1];
      node_l.count++;

      node_l.children[node_l.count] = node.children[0];
      
      int tam = node.count;
      for (int i = 0; i < tam; i++) {
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

    Node<TK, N> node_r(M);
    index.seekg(right, ios::beg);
    node_r.read(index, M);
    node.keys[node.count] = padre.keys[pos_child];
    node.count++;

    node.children[node.count] = node_r.children[0];
    
    int tam = node_r.count;
    for (int i = 0; i < tam; i++) {
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
    index.seekg(pos_node, ios::beg);
    Node<TK, N> node(M);
    node.read(index, M);
    
    // Nodo prestamista
    index.seekg(pos_node_prest, ios::beg);
    Node<TK, N> node_prest(M);
    node_prest.read(index, M);

    // Nodo padre 
    index.seekg(pos_node_padre, ios::beg);
    Node<TK, N> node_padre(M);
    node_padre.read(index, M);
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
      node_padre.keys[index1-1] = node_prest.keys[node_prest.count-1];
      node_prest.count--;
      node.count++;
      
      for (int i = node.count-1; i > -1 ; i--) {
        node.children[i+1] = node.children[i];
      }
      node.children[0] = node_prest.children[node_prest.count+1];
    }
    // Escritura
    index.seekp(pos_node, ios::beg);
    node.write(index, M);

    index.seekp(pos_node_prest, ios::beg);

    node_prest.write(index, M);

    index.seekp(pos_node_padre, ios::beg);
    node_padre.write(index, M);
  }
 
  void rotar_hojas(long pos_bucket, long pos_bucket_prest, long pos_node_padre, int index1, bool right,fstream &index, fstream &data){
    index.seekg(pos_node_padre, ios::beg);
    Node<TK, N> node_padre(M);
    node_padre.read(index, M);

    data.seekp(pos_bucket, ios::beg);
    Bucket<R> bucket(page_size);
    bucket.read(data);

    data.seekg(pos_bucket_prest, ios::beg);
    Bucket<R> bucket_prest(page_size);
    bucket_prest.read(data);

    if (right) {
      bucket.records[bucket.count] = bucket_prest.records[0];
      node_padre.keys[index1] = bucket_prest.records[0].key;
      index.seekp(pos_node_padre, ios::beg);
      node_padre.write(index, M);
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
      index.seekp(pos_node_padre, ios::beg);
      node_padre.write(index, M);
      bucket.count++;
    }

    data.seekp(pos_bucket, ios::beg);
    bucket.write(data);

    data.seekp(pos_bucket_prest, ios::beg);
    bucket_prest.write(data);
  }

  long search_recursive(long pos_node, TK key, fstream &index){
    index.seekg(pos_node, ios::beg);
    Node<TK, N> node(M);
    node.read(index, M);
    int bajada = -1;
    for(int i = 0; i < node.count; i++){
      // if(key <= node.keys[i]){
      if ( menor_igual(key, node.keys[i]) ){
        bajada = i;
        break;
      }
    }
    cout << "Lllego paso "<< endl;
    if(bajada == -1) bajada = node.count;
    if(node.pre_leaf){
      cout << "Lllego "<< endl;
      return node.children[bajada];
    }
    return search_recursive(node.children[bajada], key, index);
  }

};
