#include <iostream>
#include <queue>
#include <stack>
#include <forward_list>
#include "structs1.h"

using namespace std;


template<typename TK, typename TV>
class BPlus{
  int M;
  Node<TK, TV>* root;
  public:
  BPlus():root(nullptr), M(3){}

  explicit BPlus(int M):root(nullptr), M(M){}

  void insert(TK key, TV value){
    insert_rec(root, nullptr, key, value);
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
    int i, j, n;
    while (!queueq.empty())
    {
      n = queueq.size();
      for (i = 0; i < n; i++) {
        Node<TK, TV> *node = queueq.front();
        cout << "<";
        for ( j = 0; j < node->count; j++) {
          cout << node->keys[j] << ",";
        }
      cout << "> ";
      queueq.pop();
      if (!node->leaf) {
        queueq.push(node->children[0]);
        for (int j = 1; j <= node->count; j++) {
          queueq.push(node->children[j]);
        }
      }
      }
      cout << endl;
    }
  }
  
  void print(){
    printBPlusNode(root,0);
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
  void printBPlusNode(Node<TK, TV>* node, int depth) {
    if (node == nullptr) return;

    std::cout << "Profundidad " << depth << ": ";
    for (int i = 0; i < node->count; i++) {
      std::cout << node->keys[i] << " ";
    }  
    
    std::cout << std::endl;
    if(!node->leaf){
      for (int i = 0; i < node->count+1; i++) {
        printBPlusNode(node->children[i], depth + 1);
      }
    }
  }

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

  void insert_rec(Node<TK, TV>* node, Node<TK, TV>* padre, TK key, TV value){
    if(node == nullptr){
      Node<TK, TV>* new_node;
      Casilla<TK, TV> c_key;
      new_node = new Node<TK, TV>(M+1, true);
      c_key = Casilla<TK, TV>(key, value);
      new_node->keys[new_node->count] = c_key;
      new_node->count++;
      root = new_node;
      return;
    }
    else if(node->leaf){
      insertar(node, key, &value);
      if(node->count == M){
        split(node, padre);
      }
      return;
    }
    int bajada = -1;
    for(int i = 0; i < node->count; i++){
      if(key <= node->keys[i]){
        bajada = i;
        break;
      }
    }
    if(bajada == -1) bajada = node->count;
    insert_rec(node->children[bajada], node, key, value);
    // node->print();
    if(node->count == M){
      split(node, padre);
    }
  }

  void insertar(Node<TK,TV>* &node, TK key, TV* value){
    Casilla<TK, TV> c_key;
    if(node == nullptr) {
      node = new Node<TK, TV> (M+1,false);  
      c_key = Casilla<TK, TV>(key);
      node->keys[0] = c_key;
      node->count++;
      this->root = node;
      return;
    }

    if (node->leaf) 
      c_key = Casilla<TK, TV> (key, *value);
    else
      c_key = Casilla<TK, TV> (key);

    for (int i = 0; i < node->count; i++) {
      if ( key == node->keys[i]) 
        return;
      else if (node->keys[i] > c_key) {
        Casilla<TK, TV> antiguo = node->keys[i];
        node->keys[i] = c_key;
        c_key = antiguo;
      }
    }
    node->keys[node->count] = c_key;
    node->count++;
    return;
  }

  void split(Node<TK, TV>* node, Node<TK, TV>* padre){
    Node<TK, TV>* new_node1;
    Node<TK, TV>* new_node2;
    TK medio;
    if(node->leaf){
      new_node1 = new Node<TK, TV>(M+1, true);
      new_node2 = new Node<TK, TV>(M+1, true);
      new_node1->next = new_node2;
      new_node2->next = node->next;
    }
    else{
      new_node1 = new Node<TK, TV>(M+1, false);
      new_node2 = new Node<TK, TV>(M+1, false);
    }
    bool split_root = false;
    for (int i = 0; i < node->count; i++) {
      if(i < (M-1)/2){
        new_node1->keys[new_node1->count] = node->keys[i];
        new_node1->count++;
      }
      else if(i == (M-1)/2){
        medio = node->keys[i].key;
        insertar(padre, medio, node->keys[i].value);
        if(node->leaf){
          new_node1->keys[new_node1->count] = node->keys[i];
          new_node1->count++;
        }
      }
      else{
        new_node2->keys[new_node2->count] = node->keys[i];
        new_node2->count++;
      }
    }
    anclar(new_node1, new_node2, padre, medio);
    if(!node->leaf){
      ordenar_punteros(new_node1, new_node2, node);
    }
    return;
  }

  void anclar(Node<TK, TV>* node1, Node<TK, TV>* node2, Node<TK, TV>* padre, TK key){
    bool finded = false; 
    int i;
    Node<TK, TV> *temp1;
    // padre->print();
    // node1->print();
    // node2->print();
    for (i = 0; i < padre->count; i++) {
      if (finded) {
        temp1 = padre->children[i];
        padre->children[i] = node2;
        node2 = temp1;
      }
      else if ( key == padre->keys[i]) {
        finded = true;
        padre->children[i] = node1;
      }
    }
    padre->children[padre->count] = node2;
    return;
  }

  void ordenar_punteros(Node<TK, TV>* node1, Node<TK, TV>* node2, Node<TK, TV>* node){
    for (int i = 0; i < node->count+1; i++) {
      if(i <= node1->count)
        node1->children[i] = node->children[i];
      else
        node2->children[i-node1->count-1] = node->children[i];
    }
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
    for(int i = indice; i < node->count - 1; i++){
      node->keys[i] = node->keys[i+1];
    }
    node->count--;
    return;
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
    return;
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

    for(int i = node->count-1; i > -1; i--)
      node->keys[i+1] = node->keys[i];
    node->keys[0] = node1->keys[node1->count-1];
    node1->count--;
    padre->keys[index-1] = node1->keys[node1->count-1];
    node->count++;
    return;
  }
};
