#pragma once
#include <forward_list>
#include <iostream>

using namespace std;
template<typename TK, typename TV>
struct Casilla{
  TK key;
  forward_list<TV>* values;
  Casilla()=default;
  explicit Casilla(TK key):key(key), values(nullptr){};
  Casilla(TK key, TV value):key(key){
    values = new forward_list<TV>;
    values->push_back(value);
  };
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
  if(casilla.values != nullptr){
    os<<":"<<endl;
    casilla.values->mostrar();
  }
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
    keys = new Casilla<TK, TV>[M-1];
    if (!hoja){
      children = new Node<TK,TV>*[M];
      for (int i = 0; i < M; i++) {
        children[i] = nullptr;
      }
    }
    else{
      next = nullptr;
      children = nullptr;
    }
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
