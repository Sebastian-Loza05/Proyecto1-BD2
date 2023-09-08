#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <forward_list>
#include "structs.h"

using namespace std;


template<typename TK, typename TV>
class BPlus{
  int M;
  Node<TK, TV>* root;
  public:
  BPlus():root(nullptr), M(3){}

  explicit BPlus(int M):root(nullptr), M(M){}

  void insert(TK key, TV value){
    int bajada = 0;
    queue<Node<TK, TV>* > nodos_hijos;
    stack<Node<TK, TV>* > nodos;
    insert_recursive(root, nullptr, key, value, bajada, nodos, nodos_hijos, true);
  }

  void remove(TK key, TV value){
    remove2(root, key, value, nullptr, 0);
  }

  bool search(TK key){
    Node<TK, TV>* node = search_rec(root, key);
    for (int i = 0; i < node->count; i++) {
      if(key == node->keys[i])
        return true;
    }
    return false;
  }

  void toString(){
    Node<TK, TV>* temp = root;
    while(!temp->leaf){
      temp = temp->children[0];
    }
    while(temp != nullptr){
      cout<<"| ";
      for(int i = 0; i < temp->count; i++)
        cout<<temp->keys[i]<<", ";
      cout<<"| ";
      temp = temp->next;
    }
    cout<<endl;
  }

  void displayTree(){
    queue<Node<TK, TV>*> queueq;
    queueq.push(this->root);
    stack<int> niveles;
    niveles.push(1);
    while (!queueq.empty())
    {
      Node<TK, TV> *node = queueq.front();
      cout<<"| ";
      for(int i = 0; i < node->count; i++)
        cout<<node->keys[i]<<", ";
      cout<<"| ";
      niveles.top()--;
      if(niveles.top() == 0){
        niveles.pop();
        cout<<endl;
      }
      queueq.pop();
      if(node->children != nullptr){
        niveles.push(node->count+1);
        for(int i = 0; i < node->count+1; i++){
          queueq.push(node->children[i]);
        }
      }
    }
  }

  forward_list<Casilla<TK, TV>>* range_search(TK begin, TK end){
    Node<TK, TV>* inicio = search_rec(root, begin);
    forward_list<Casilla<TK, TV>>* result = new forward_list<Casilla<TK, TV>>();
    while(inicio != nullptr && end >= inicio->keys[0]){
      for (int i = 0; i < inicio->count; i++) {
        if(begin <= inicio->keys[i] && end >= inicio->keys[i])
          result->push_back(inicio->keys[i]);
      }
      inicio = inicio->next;
    }
    return result;
  }

  TK get_min(){
    Node<TK, TV>* temp = root;
    while(!temp->leaf){
      temp = temp->children[0];
    }
    return temp->keys[0].key;
  }

  TK get_max(){
    Node<TK, TV>* temp = root;
    while(!temp->leaf){
      temp = temp->children[temp->count];
    }
    return temp->keys[temp->count-1].key;
  }

  ~BPlus(){

  }

  private:
  Node<TK, TV>* search_rec(Node<TK, TV>* node, TK key){
    if (node == nullptr)return nullptr;
    else if(node->leaf)
      return node;
    int bajada = -1;
    for(int i = 0; i < node->count; i++){
      if(key <= node->keys[i]){
        bajada = i;
        break;
      }
    }
    if(bajada == -1) bajada = node->count;
    return search_rec(node->children[bajada],key);
  }

  void insert_recursive(Node<TK, TV>* &node, Node<TK, TV>* prev, TK key, TV value, int &bajada, queue<Node<TK, TV>* > &cola, stack<Node<TK, TV>* > &nodos_hijos, bool hoja){
    if (node == nullptr){
      Node<TK, TV>* new_node ;
      Casilla<TK, TV> c_key;
      if(hoja){
        new_node = new Node<TK, TV>(M,true);
        c_key = Casilla<TK, TV>(key, value);
      }
      else{
        new_node = new Node<TK, TV>(M,false);
        c_key = Casilla<TK, TV>(key);
      }
      new_node->keys[new_node->count] = c_key;
      new_node->count++;
      root = new_node;
      cola.push(root);
      bajada = 0;
      return;
    }
    else if(node->leaf){
      if(!hoja)
        node->leaf = false;
      insertar(node, prev, key, value, bajada, cola, nodos_hijos, hoja);
    }
    else{
      cola.push(node);
      bajada = -1;
      for(int i = 0; i < node->count; i++){
        if(key <= node->keys[i]){
          bajada = i;
          break;
        }
      }
      if(bajada == -1) bajada = node->count;
      if(bajada > 0){
        if(node->children[bajada]->leaf)
          prev = nullptr;
        else
          prev = node->children[bajada-1];
      }
      else{
        if(prev != nullptr){
          prev = prev->children[prev->count];
        }
      }
      insert_recursive(node->children[bajada], prev, key, value, bajada, cola, nodos_hijos, hoja);
    }
  }

  void insertar(Node<TK, TV>* &node, Node<TK, TV>* prev, TK key, TV value, int &bajada, queue<Node<TK, TV>* > &cola, stack<Node<TK, TV>* > &hijos, bool hoja){
    if(hoja){
      for(int i = 0; i < node->count; i++){
        if(key == node->keys[i]){
          node->keys[i].values->push_back(value);
          return;
        }
      }
    }
    if(node->count < M-1){
      ingresar(node, key, value, hoja);
      queue<Node<TK, TV>* >  cola_vacia;
      cola = cola_vacia;
      cola.push(node);
      return;
    }
    Casilla<TK, TV> c_key;
    if(hoja)
      c_key = Casilla<TK, TV>(key, value);
    else
      c_key = Casilla<TK, TV>(key);

    Node<TK, TV>* new_node1;
    Node<TK, TV>* new_node2;
    if(hoja){
      new_node1 = new Node<TK, TV>(M, true);
      new_node2 = new Node<TK, TV>(M, true);
      new_node1->next = new_node2;
      new_node2->next = node->next;
    }
    else{
      new_node1 = new Node<TK, TV>(M, false);
      new_node2 = new Node<TK, TV>(M, false);
    }
    int extra = 0;
    Casilla<TK, TV> medio = Casilla<TK, TV>(-2222);
    for(int i = 0; i < node->count+1; i++){
      if(i < node->count/2){
        if(c_key < node->keys[i-extra] && c_key != medio){
          extra = 1;
          new_node1->keys[new_node1->count] = c_key;
        }
        else
          new_node1->keys[new_node1->count] = node->keys[i-extra];
        new_node1->count++;
      }
      else if(i > node->count/2){
        if (i-extra == node->count || (c_key < node->keys[i-extra] && c_key != medio && extra != 1)){
          extra = 1;
          new_node2->keys[new_node2->count] = c_key;
        }
        else
          new_node2->keys[new_node2->count] = node->keys[i-extra];
        new_node2->count++;
      }
      else{
        if(c_key < node->keys[i-extra] && extra != 1){
          if(hoja){
            new_node1->keys[new_node1->count] = c_key;
            new_node1->count++;
            medio = Casilla<TK, TV>(key, value);
          }
          else{
            medio = Casilla<TK, TV>(key);
          }
          extra = 1;
        }
        else {
          medio = node->keys[i-extra];
          if(hoja){
            new_node1->keys[new_node1->count] = node->keys[i-extra];
            new_node1->count++;
          }
        }
      }
    }
    node->keys = new_node1->keys;
    node = new_node1;
    Node<TK, TV>* nodo;
    if(cola.empty()){
      nodo = nullptr;
      hijos.push(new_node2);
      hijos.push(new_node1);
    }
    else{
      nodo = cola.back();
      if(nodo->leaf){
        Node<TK, TV>* padre = new Node<TK, TV>(M,false);
        padre->keys = nodo->keys;
        nodo = padre;
      }
      cola.pop_back();
      bajada = -1;
      for(int i = 0; i < nodo->count; i++){
        if(new_node2->keys[0] <= nodo->keys[i]){
          bajada = i;
          break;
        }
      }
      if(bajada == -1) bajada = nodo->count;
      for (int i = nodo->count; i >= 0; i--) {
        if(i == bajada){
          hijos.push(new_node2);
          hijos.push(new_node1);
        }
        else{
          hijos.push(nodo->children[i]);
        }
      }
    }
    if(nodo != nullptr)
      nodo->leaf = true;
    insert_recursive(nodo, prev, medio.key, value, bajada, cola, hijos, false);
    if(nodo != nullptr)
      nodo->leaf = false;
    anclar(cola, hijos, prev);
  }

  void anclar(queue<Node<TK, TV>* > &nodos, stack<Node<TK, TV>* > &hijos, Node<TK, TV>* prev){
    int size = nodos.size();
    bool prev_usado = false;
    for (int i = 0; i < size; i++) {
      bool avanzar = false;
      Node<TK, TV>* nodo = nodos.front();
      nodos.pop();
      if(hijos.is_empty())
        return;
      if(nodo->children[0] != nullptr){
        if(nodo->children[0]->keys[0] != hijos.top()->keys[0])
          avanzar = true;
      }
      for (int j = 0; j < nodo->count+1; j++) {
        if(avanzar)
          break;
        Node<TK, TV>* nodo1 = hijos.top();
        hijos.pop();
        if(j == 0){
          if(prev != nullptr && nodo1->leaf && nodo1->keys[0].key > prev->keys[prev->count].key && !prev_usado){
            prev->next = nodo1;
            prev_usado = true;
          }
        }
        if(nodo1->leaf && j < nodo->count && nodo1->children == nullptr)
          nodo1->next = hijos.top();
        else if(nodo1->leaf && nodo1->children == nullptr && hijos.size()>0)
          nodo1->next = hijos.top();
        nodo->children[j] = nodo1;
        nodos.push(nodo1);
      }
      nodo->leaf = false;
    }
  }

  void ingresar(Node<TK, TV>* node, TK key, TV value, bool hoja){
    bool insertado = false;
    Casilla<TK, TV> c_key ;
    if (hoja)
      c_key = Casilla<TK, TV>(key, value);
    else
      c_key = Casilla<TK, TV>(key);
    for (int i = 0; i < node->count; i++) {
      if(c_key < node->keys[i]){
        insertado = true;
        Casilla<TK, TV> antiguo = node->keys[i];
        node->keys[i] = c_key;
        c_key = antiguo;
      }
    }
    node->keys[node->count] = c_key;
    node->count++;
    return;
  }


  void remove2(Node<TK, TV>* node,TK key, TV value, Node<TK, TV>* padre,int child_index){
    if(node == nullptr){
      return;
    }
    int indice= -1;
    for(int i = 0; i < node->count; i++){
      if(key == node->keys[i] && node->leaf){
        delete_node(node,key, value, padre,i, child_index);
        if(node->count < (M-1)/2 && (node != root || node->count < 1)){
          arreglar_node(node, padre, child_index, true);
          return;
        }
      }
      else {
        if(key <= node->keys[i]){
          indice = i;
          break;
        }
      }
    }
    if(indice == -1) indice = node->count;
    if(!node->leaf)
      remove2(node->children[indice],key,value,node,indice);
    if(node->count < (M-1)/2 && (node != root || node->count < 1)){
      arreglar_node(node, padre, child_index, false);
    }
    //Arreglar si esta mal
  }

  void delete_node(Node<TK, TV>* node,TK key, TV value, Node<TK, TV>* padre,int indice,int child_index){
    if(node->leaf){
      int values_size = node->keys[indice].values->size();
      if(values_size > 1){
        for (int i = 0; i < values_size; i++) {
          if((*(node->keys[indice].values))[i] == value){
            node->keys[indice].values->remove(i);
            break;
          }
        }
        return;
      }
      for(int i = indice; i < node->count - 1; i++){
        node->keys[i] = node->keys[i+1];
      }
      node->count--;
      return;
    }
  }

  void arreglar_node(Node<TK, TV>* node,Node<TK, TV>* padre,int child_index, bool hoja){
    if(padre == nullptr){
      root = node->children[0];
      return;
    }
    Node<TK, TV>* left = nullptr;
    Node<TK, TV>* right = nullptr;
    bool sibling_available=false;

    if(child_index > 0)
      left = padre->children[child_index-1];
    if(child_index < padre->count)
      right = padre->children[child_index+1];
    if(left != nullptr && left->count > (M-1)/2){
      sibling_available = true;
      if(hoja)rotar_hojas(node,left,child_index,false,padre);
      else rotar_internos(node,left,child_index,false,padre);

    }
    if(right != nullptr && right->count > (M-1)/2){
      sibling_available = true;

      if(hoja)rotar_hojas(node,right,child_index,true,padre);
      else rotar_internos(node,right,child_index,true,padre);
    }
    if(sibling_available) return;

    TK* newValues = new TK[M-1];

    if(left != nullptr){
      if(!hoja){
        left->keys[left->count] = padre->keys[child_index-1];
        left->count++;
      }

      if(!node->leaf)
        left->children[left->count] = node->children[0];

      for(int i = 0; i< node->count; i++){
        left->keys[left->count]= node->keys[i];
        left->count++;
        node->count--;
        if(!node->leaf)
          left->children[left->count] = node->children[i+1];
      }
      if(node->leaf)
        left->next = node->next;
      for(int i = child_index-1; i<padre->count-1; i++){
        padre->keys[i] = padre->keys[i+1];
      }
      padre->count--;
      for(int i = child_index; i<padre->count+1; i++){
        padre->children[i] = padre->children[i+1];
      }
      return;

    }
    if(right !=nullptr){
      TK* temp_keys = new TK[M-1];
      Node<TK, TV>** temp_children = new Node<TK, TV>*[M];
      int i = 0;
      if(!hoja){
        node->keys[node->count] = padre->keys[child_index];
        node->count++;
      }
      if(!node->leaf)
        node->children[node->count] = right->children[0];
      for(int j = 0; j < right->count; j++){
        node->keys[node->count] = right->keys[j];
        node->count++;
        right->count--;
        if(!node->leaf)
          node->children[node->count] = right->children[j+1];
      }

      if(node->leaf)
        node->next = right->next;

      for(int i = child_index; i<padre->count-1; i++){
        padre->keys[i] = padre->keys[i+1];
      }
      padre->count--;
      for(int i = child_index+1; i<padre->count+1; i++){
        padre->children[i] = padre->children[i+1];
      }
      return;
    }

  }

  void rotar_internos(Node<TK, TV>* node, Node<TK, TV>* node1, int index, bool derecha, Node<TK, TV>* padre){
    if(derecha){ // prestamista derecha 
      node->keys[node->count] = padre->keys[index];
      padre->keys[index] = node1->keys[0];
      for(int i = 0; i < node1->count-1;i++){
        node1->keys[i] = node1->keys[i+1];
      }
      node1->count--;
      node->count++;
      if(node->leaf) return;
      //arreglar punteros
      node->children[node->count] = node1->children[0];
      for(int i = 0; i < node1->count+1;i++){
        node1->children[i] = node1->children[i+1];
      }
      return;
    }
    else{ // prestamista izquierda
      for(int i = node->count-1; i > -1; i--)
        node->keys[i+1] = node->keys[i];
      node->keys[0] = padre->keys[index-1];
      padre->keys[index-1] = node1->keys[node1->count-1];
      node1->count--;
      node->count++;
      if(node->leaf) return;
      //arreglar punteros
      for(int i = node->count-1; i > -1;i--){
        node->children[i+1] = node1->children[i];
      }
      node->children[0] = node1->children[node1->count+1];

    }
  }

  void rotar_hojas(Node<TK, TV>* node, Node<TK, TV>* node1, int index, bool derecha, Node<TK, TV>* padre){
    if(derecha){
      node->keys[node->count] = node1->keys[0];
      padre->keys[index] = node1->keys[0];
      for(int i = 0; i < node1->count-1;i++){
        node1->keys[i] = node1->keys[i+1];
      }
      node1->count--;
      node->count++;
      return;
    }
    else{
      for(int i = node->count-1; i > -1; i--)
        node->keys[i+1] = node->keys[i];
      node->keys[0] = node1->keys[node1->count-1];
      node1->count--;
      padre->keys[index-1] = node1->keys[node1->count-1];
      node->count++;
      return;
    }
  }
};
