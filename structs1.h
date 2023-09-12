#pragma once
#include <forward_list>
#include <iostream>

using namespace std;
template<typename TK, typename TV>
struct Casilla{
  TK key;
  TV *value;
  Casilla()=default;
  explicit Casilla(TK key):key(key), value(nullptr){};
  Casilla(TK key, TV value):key(key){
    this->value = &value;
  }
  bool operator<(const Casilla<TK, TV> &otro){
    return this->key < otro.key;
  }

  bool operator<=(const Casilla<TK, TV> &otro){
    return this->key <= otro.key;
  }

  bool operator>( const Casilla<TK, TV> &otro){
    return this->key > otro.key;
  }

  bool operator>=( const Casilla<TK, TV> &otro){
    return this->key >= otro.key;
  }

  bool operator==( const Casilla<TK, TV> &otro){
    return this->key == otro.key;
  }

  bool operator!=( const Casilla<TK, TV> &otro){
    return this->key != otro.key;
  }

};

template<typename TK, typename TV>
bool operator<(TK otro, const Casilla<TK, TV> &casilla){
  return otro < casilla.key;
}

template<typename TK, typename TV>
bool operator<=(TK otro, const Casilla<TK, TV> &casilla){
  return otro <= casilla.key;
}

template<typename TK, typename TV>
bool operator>(TK otro, const Casilla<TK, TV> &casilla){
  return otro > casilla.key ;
}

template<typename TK, typename TV>
bool operator>=(TK otro, const Casilla<TK, TV> &casilla){
  return otro >= casilla.key ;
}

template<typename TK, typename TV>
bool operator==(TK otro, const Casilla<TK, TV> &casilla){
  return casilla.key == otro;
}

template<typename TK, typename TV>
ostream& operator << (ostream& os, const Casilla<TK, TV>& casilla){
  os << casilla.key;
  // if(casilla.value != nullptr){
  //   os<<":";
  //   os<<casilla.value<<endl;
  // }
  return os;
}

template<typename TK, typename TV>
struct Node{
  Casilla<TK, TV>* keys;
  Node** children;
  Node* next;
  int count;
  bool leaf;

  Node(int M, bool hoja):count(0), leaf(hoja){
    keys = new Casilla<TK, TV>[M];
    if (!hoja){
      children = new Node<TK,TV>*[M+1];
      for (int i = 0; i < M+1; i++) {
        children[i] = nullptr;
      }
    }
    else{
      next = nullptr;
      children = nullptr;
    }
  }
  
  void print(){
    cout<<"node: ";
    for (int i = 0; i < count; i++) {
      cout<<keys[i]<<", ";
    }
    cout<<endl;
  }
  void killSelf() {
    if(!this->leaf){
      for (int i = 0; i < this->count+1; i++) {
        this->children[i]->killSelf();
      }
      delete[] this->children;
    }
    else{
      delete next;
      next = nullptr;
    }
    delete keys;
    delete this;
  }

};
