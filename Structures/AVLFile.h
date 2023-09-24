#ifndef AVL_AVL_H
#define AVL_AVL_H

#include <fstream>
#include <functional>
#include "Record.h"
#include "functions.h"
#include "methods.h"
#include <iomanip>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

struct NodeBTAVL{
  Record data;
  long left;
  long right;

  long height;
  long next_del;

  NodeBTAVL(){
      left = right = -1;
      height = next_del = 0;
  }

  NodeBTAVL(Record record){
      left = right = -1;
      height = next_del = 0;
      this->data = record;
  }

  void print(){
    cout << left << "\t" << right << "\t" << height << "\t" << next_del << "\t";
    data.print();

  }
};

template<typename T>
class AVLFile: public MethodSelector {
private:
    long pos_root;
    string filename;
    long counter = 0;
    int i = 0;

    long size();
    int sizeNode();
    void addvector(vector<NodeBTAVL> &aux, long value);
    void rangeSearch(long pos_node, T begin, T end, vector<Record> &aux, ifstream &data){
      if (pos_node == -1) {
        return;
      }
      NodeBTAVL temp;
      data.seekg(pos_node, ios::beg);
      data.read( (char*)(&temp), sizeof(NodeBTAVL) );

      if ( menor_igual(begin, temp.data.key ) ) {
        rangeSearch(temp.left, begin, end, aux, data);
      }
      if (menor_igual(begin, temp.data.key) && menor_igual(temp.data.key, end) ) {
        aux.push_back(temp.data);
      } 
      if ( menor_igual(temp.data.key, end) ) {
        rangeSearch(temp.right, begin, end, aux, data);
      }
      return;
      
    };

public:
    AVLFile(){
      this->filename = "AVLdata.dat";
      ofstream file(this->filename, ios::binary | ios::app);
      file.close();

      fstream data( this->filename, ios::binary | ios::in | ios::out );
      // p -> write    g -> read 
      data.seekg(0, ios::end);
      int tam = data.tellg();
      if ( tam <= 0 ) {
        this->pos_root = -1;
        data.seekp(0, ios::beg);
        // pos_root
        data.write( reinterpret_cast<const char*>(&this->pos_root), sizeof(long) );
        // next_del
        data.write( reinterpret_cast<const char*>(&this->pos_root), sizeof(long) );
      }
      else {
        data.seekg(0, ios::beg);
        data.read( (char*)(&this->pos_root), sizeof(long) ); 
      }
      data.close();
    };

    bool add(Record record) override{
      fstream data(this->filename, ios::out | ios::in); 
      if (!data.is_open()) return false;

      NodeBTAVL temp(record);
      if ( this->pos_root == -1 ) {
        data.seekg(sizeof(long), ios::beg);
        long next_del_act;
        long pos_r_l;
        data.read((char*)(&next_del_act), sizeof(long));

        if ( next_del_act == -1 ) {
          data.seekg(0, ios::end);
          data.seekp(0, ios::end);
          this->pos_root = ( sizeof(long) + sizeof(long) );
          data.seekp( 0, ios::beg );
          data.write( reinterpret_cast<char*>(&this->pos_root), sizeof(long) );

          data.seekp( (sizeof(long) + sizeof(long)), ios::beg );
          data.write( (char*)&temp, sizeof(NodeBTAVL) );
          return true;
        }
        NodeBTAVL nuevo_temp(record);
        data.seekg( (2 * sizeof(long)) + ( (next_del_act-1) * sizeof(NodeBTAVL) ), ios::beg );
        data.read( (char*)(&temp), sizeof(NodeBTAVL) );
        long new_del = temp.next_del;
        temp = nuevo_temp;
        data.seekp(sizeof(long), ios::beg);
        data.write( reinterpret_cast<const char*>(&new_del), sizeof(long) );
        this->pos_root = (2*sizeof(long)) + ( (next_del_act-1) * sizeof(NodeBTAVL) );
        data.seekp( 0, ios::beg );
        data.write( reinterpret_cast<char*>(&this->pos_root), sizeof(long) );
        data.seekp( (2 * sizeof(long)) + ( (next_del_act-1) * sizeof(NodeBTAVL)), ios::beg);
        data.write( (char*)&temp, sizeof(NodeBTAVL) );
        
        return true;
      }
      
      data.seekg( this->pos_root, ios::beg );
      data.read( reinterpret_cast<char*>(&temp), sizeof(NodeBTAVL) );
      bool result = add(this->pos_root, -1, false, record, temp, data);
      data.close();
      return result;
    };
    pair<Record, bool> search(T key) override {
      ifstream data(this->filename, ios::binary);
      
      pair<Record, bool> result = search(this->pos_root, key, data);
        
      data.close();
      return result;
      
    };
    vector<Record> rangeSearch(T begin, T end) override {
      vector<Record> result;
      ifstream data(this->filename, ios::binary);
        
      rangeSearch(this->pos_root, begin, end, result, data);
      
      data.close();
      return  result;
    };

    void display() override {
      ifstream data(this->filename, ios::binary);
      if ( !data.is_open() ) return;
      
      display(this->pos_root, data);
      data.close();
    };


    void display_all() override {
      ifstream data(this->filename, ios::binary);
      data.seekg(0, ios::end);
      int tam = data.tellg();
      tam /= sizeof(NodeBTAVL);
      data.seekg(0, ios::beg);
      long root, del;
      data.read( (char*)(&root), sizeof(long));
      data.read( (char*)(&del), sizeof(long));
      cout << "pos_root = "<< root << " next_del = " << del << endl;
      data.seekg( 2 * sizeof(long), ios::beg);
      NodeBTAVL temp;
      for (int i = 0; i < tam; i++) {
        data.read( reinterpret_cast<char*>(&temp), sizeof(NodeBTAVL) );
        temp.print();
      }
      data.close();
      return;
    };

    bool remove(T key) override {
      fstream data(this->filename, ios::binary | ios::in | ios::out);
      if (!data.is_open()) return false;
      
      bool result = remove(this->pos_root, -1, false, key, data);
      
      data.close();

      return result;
    }
    vector<Record> load() override {
      vector<Record> result;
      ifstream data(this->filename, ios::binary);

      load(this->pos_root, result, data);
        
      data.close();
      return result;
    } 
private:
    void load(long pos_node, vector<Record> &result, ifstream &data){
      if (pos_node == -1) {
        return;
      }
      
      NodeBTAVL temp;
      data.seekg(pos_node, ios::beg);
      data.read( (char*)(&temp), sizeof(NodeBTAVL) );
      
      load(temp.left, result, data);
      result.push_back(temp.data);
      load(temp.right, result, data);
      return;
    };
    void display(long pos_node, ifstream& data) {
      if ( pos_node == -1 ) {
        return;
      }
      queue<long> queueq;
      queueq.push(this->pos_root);
      while (!queueq.empty()) {
        int n = queueq.size();
        for (int i = 0; i < n; i++) {
          long pos_node = queueq.front();
          NodeBTAVL temp;
          queueq.pop();
          data.seekg(pos_node, ios::beg);
          data.read( reinterpret_cast<char*>(&temp), sizeof(NodeBTAVL) );
          cout << "<" << temp.data.key << " H: " << temp.height << "> ";
          if (temp.left != -1) {
            queueq.push(temp.left);
          }
          if (temp.right != -1) {
            queueq.push(temp.right);
          }
        }
        cout << endl;
      }
    };

    
    bool remove(long pos_node, long pos_node_prev, bool left, T key, fstream &data){
      bool result = true;
      NodeBTAVL temp_prev;
      if ( pos_node == -1 ) {
        return false;
      }
      NodeBTAVL temp;
      data.seekg(pos_node, ios::beg);
      data.read( (char*)(&temp), sizeof(NodeBTAVL) );

      if ( menor(key, temp.data.key) ) {
        result = remove( temp.left, pos_node, true, key, data );
      } 
      else if ( menor(temp.data.key, key) ) {
        result = remove( temp.right, pos_node, false, key, data );
      }
      else if ( (temp.left != -1) && (temp.right != -1) ) {
        if (pos_node_prev != -1) {
          data.seekg(pos_node_prev, ios::beg);
          data.read( (char*)(&temp_prev), sizeof(NodeBTAVL) );
        }
        NodeBTAVL temp_new;
        data.seekg(temp.left, ios::beg);
        data.read( (char*)(&temp_new), sizeof(NodeBTAVL) );
        long temp_new_right_last = temp.left, temp_new_right_last_prev = pos_node;
        bool is_left = true;
        while ( temp_new.right != -1 ) {
          temp_new_right_last_prev = temp_new_right_last;
          temp_new_right_last = temp_new.right;
          data.seekg(temp_new.right, ios::beg);
          data.read( (char*)(&temp_new), sizeof(NodeBTAVL) );
          is_left = false;
        }
        Record temp_data = temp_new.data;
        result = remove( temp_new_right_last, temp_new_right_last_prev, is_left, temp_new.data.key, data );
        temp.data = temp_data;
        
        data.seekp( pos_node, ios::beg );
        data.write( (char*)&temp, sizeof(NodeBTAVL) );
      }
      else {
        if ( pos_node_prev == -1 ) {
          if (temp.left != -1) {
            this->pos_root = temp.left;
          } 
          else if (temp.right != -1) {
            this->pos_root = temp.right;
          }
          else {
            this->pos_root = -1;
          }

          long act_next_del;
          data.seekg( sizeof(long), ios::beg );
          data.read( (char*)(&act_next_del), sizeof(long) );
          temp.next_del = act_next_del;
          act_next_del = (pos_node - 2 * (sizeof(long))) / sizeof(NodeBTAVL) + 1;
          data.seekp(0, ios::beg);
          data.write( reinterpret_cast<const char*>(&this->pos_root), sizeof(long) );
          data.seekp(sizeof(long), ios::beg);
          data.write( reinterpret_cast<const char*>(&act_next_del), sizeof(long));
          
          data.seekp( pos_node, ios::beg);
          data.write( (char*)(&temp), sizeof(NodeBTAVL));
        }
        else {
          data.seekg(pos_node_prev, ios::beg);
          data.read( (char*)(&temp_prev), sizeof(NodeBTAVL) );
          if ( temp.left != -1 ) {
            if (left) {
              temp_prev.left = temp.left;
            }
            else{
              temp_prev.right = temp.left;
            }
          }
          else if ( temp.right != -1 ) {
            if (left) {
              temp_prev.left = temp.right;
            }
            else {
              temp_prev.right = temp.right;
            }
          }
          else {
            if (left) {
              temp_prev.left = -1;
            }
            else {
              temp_prev.right = -1;
            }
          }
          data.seekp(pos_node_prev, ios::beg);
          data.write( (char*)(&temp_prev), sizeof(NodeBTAVL) );
          long act_next_del;
          data.seekg(sizeof(long), ios::beg);
          data.read( (char*)(&act_next_del), sizeof(long) );
          temp.next_del = act_next_del;
          act_next_del = (pos_node - 2 * (sizeof(long))) / sizeof(NodeBTAVL) + 1;
          data.seekp( sizeof(long), ios::beg );
          data.write( reinterpret_cast<const char*>(&act_next_del), sizeof(long) );
          data.seekp( pos_node, ios::beg );
          data.write( (char*)(&temp), sizeof(NodeBTAVL) );
        }
      }
      if (!result) {
        return false;
      }
       
      if ( pos_node_prev != -1 ) {
        data.seekg( pos_node_prev, ios::beg );
        data.read( (char*)(&temp_prev), sizeof(NodeBTAVL) );
        if ( menor(key, temp_prev.data.key) ) {
          balance(temp, pos_node, true, pos_node_prev, data);
        }
        else {
          balance(temp, pos_node, false, pos_node_prev, data);
        }
      }
      else {
        // No importa left o right pq es root
        balance(temp, pos_node, false, pos_node_prev, data);
      }
      return true;
    };

    bool add(long pos_node, long pos_node_prev, bool left, Record value , NodeBTAVL node_prev, fstream &data){
      long pos_node_next;
      bool result = true;
      NodeBTAVL temp;
      NodeBTAVL temp_prev;
      if (pos_node == -1) {
        data.seekg(sizeof(long), ios::beg);
        long next_del_act;
        long pos_node_new;
        data.read((char*)(&next_del_act), sizeof(long));
        if ( next_del_act == -1 ) {
          data.seekg(0, ios::end);
          data.seekp(0, ios::end);
          pos_node_new = data.tellg();
          temp.data = value;
          data.write( (char*)&temp, sizeof(NodeBTAVL) );
        }
        else {
          NodeBTAVL nuevo_temp(value);
          data.seekg( (2 * sizeof(long)) + ( (next_del_act-1) * sizeof(NodeBTAVL) ), ios::beg); 
          data.read( (char*)(&temp), sizeof(NodeBTAVL) );
          long new_del = temp.next_del;
          temp = nuevo_temp;
          data.seekp(sizeof(long), ios::beg);
          data.write( reinterpret_cast<const char*>(&new_del), sizeof(long));
          
          data.seekp( (2 * sizeof(long)) + ( (next_del_act-1) * sizeof(NodeBTAVL)), ios::beg);
          data.write( (char*)&temp, sizeof(NodeBTAVL) );
          pos_node_new = ( 2 * sizeof(long) + ( (next_del_act-1) * sizeof(NodeBTAVL)) );
        }
        
        // Escritura node anterior
        
        data.seekg( pos_node_prev, ios::beg );
        data.read( (char*)(&temp_prev), sizeof(NodeBTAVL) );
        if (left) {
          temp_prev.left = pos_node_new;
        }
        else {
          temp_prev.right = pos_node_new;
        }
        
        data.seekp( pos_node_prev, ios::beg );
        data.write( (char*)&temp_prev, sizeof(NodeBTAVL) ); 
        
        return true;
      }

      if ( igual_igual(value.key, node_prev.data.key) ){
        return false;
      }    
      else if ( menor(value.key, node_prev.data.key) ){
        pos_node_next = node_prev.left;
        if ( pos_node_next == -1) {
          result = add(pos_node_next, pos_node, true, value, temp, data); 
        }
        else {
          data.seekg( pos_node_next, ios::beg );
          data.read( reinterpret_cast<char*>(&temp), sizeof(NodeBTAVL) );
          result = add(pos_node_next, pos_node, true, value, temp, data);
        }
          
        if (!result) {
          return false;
        }
      }
      else {
        pos_node_next = node_prev.right;
        if ( pos_node_next == -1) {
          result = add(pos_node_next, pos_node, false, value, temp, data);
        }
        else {
          data.seekg( pos_node_next, ios::beg );
          data.read( reinterpret_cast<char*>(&temp), sizeof(NodeBTAVL) );
          result = add(pos_node_next, pos_node, false, value, temp, data);
        }

        if (!result) {
          return false;
        }
      }

      if ( pos_node_prev != -1 ) {      
        data.seekg( pos_node_prev, ios::beg );
        data.read( (char*)(&temp_prev), sizeof(NodeBTAVL) );
        if ( menor(value.key, temp_prev.data.key) ) {
          balance(temp, pos_node, true, pos_node_prev, data);
        }
        else {
          balance(temp, pos_node, false, pos_node_prev, data);
        }
      }
      else {
        // No importa left o right pq es root
        balance(temp, pos_node, false, pos_node_prev, data);
      }
      return true;
  }
    pair<Record,bool> search(long pos_node, T key, ifstream &data){
      if (pos_node == -1) {
        return make_pair(Record(), false);
      }

      NodeBTAVL temp;
      data.seekg(pos_node, ios::beg);
      data.read( (char*)&temp, sizeof(NodeBTAVL) );
      
      if ( igual_igual(key, temp.data.key) ) {
        return make_pair(temp.data, true);
      }
      else if ( menor(key, temp.data.key) ){
        return search(temp.left, key, data);
      }
      else {
        return search(temp.right, key, data);
      }  
    };
    void updateHeight(long pos_node, fstream &data){
      if ( pos_node == -1 ) {
        return;
      }
      NodeBTAVL node;
      data.seekg( pos_node, ios::beg);
      data.read( (char*)&node, sizeof(NodeBTAVL) );

      node.height = max( height(node.left, data), height(node.right, data) ) + 1;
      data.seekp(pos_node, ios::beg);
      data.write( (char*)&node, sizeof(NodeBTAVL) );
      return;
    };
    void balance(NodeBTAVL node, long pos_node, bool is_left, long pos_node_prev, fstream &data){
      if (pos_node == -1){
          return;
      }
      NodeBTAVL temp;
      data.seekg( pos_node, ios::beg );
      data.read( (char*)(&temp), sizeof(NodeBTAVL) );
      
      if ( balancingFactor(pos_node, data) == -2 ) {
        if ( balancingFactor(temp.right, data) == 1 ) {
          NodeBTAVL temp_right;
          data.seekg( temp.right, ios::beg );
          data.read( (char*)(&temp_right), sizeof(NodeBTAVL) );
          
          left_rota(temp_right, temp.right, is_left, pos_node_prev, data);
          right_rota(temp, pos_node, is_left, pos_node_prev, data);
        }
        else{
          right_rota(temp, pos_node, is_left, pos_node_prev, data);
        }
      }
      else if ( balancingFactor(pos_node, data) == 2 ) {
        if ( balancingFactor(temp.left, data) == -1 ) {
          NodeBTAVL temp_left;
          data.seekg( temp.left, ios::beg );
          data.read( (char*)(&temp_left), sizeof(NodeBTAVL) );
          right_rota(temp_left, temp.left, is_left, pos_node_prev, data);
          left_rota(temp, pos_node, is_left, pos_node_prev, data);
        }
        else {
          left_rota(temp, pos_node, is_left, pos_node_prev, data);
        }
      }
      updateHeight(pos_node, data);
    };
    int balancingFactor(long pos_node, fstream &data){
      if (pos_node == -1) {
        return -1;
      }
      NodeBTAVL temp; 
      data.seekg(pos_node, ios::beg);
      data.read( reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL) );
      return height(temp.left, data) - height(temp.right, data);
    };
    int height(long pos_node, fstream &data){
      if ( pos_node == -1 ) {
        return -1;
      }
      NodeBTAVL temp; 
      data.seekg(pos_node, ios::beg);
      data.read( reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL) );
      long height_left = height(temp.left, data), height_right = height(temp.right, data);
      if (height_left > height_right) {
        return height_left + 1;
      }
      return height_right + 1;
    };
    void left_rota(NodeBTAVL &node, long &pos_node, bool is_left, long pos_node_prev, fstream &data){
      NodeBTAVL temp_prev;
      NodeBTAVL temp_left;
      long pos_node_left = node.left;
      if (pos_node_prev == -1) {
        this->pos_root = node.left;
        data.seekp(0, ios::beg);
        data.write( reinterpret_cast<const char*>(&this->pos_root), sizeof(long) );
      }
      else if (is_left) {
        data.seekg( pos_node_prev, ios::beg );
        data.read( (char*)(&temp_prev), sizeof(NodeBTAVL) );
        temp_prev.left = node.left;
        data.seekp( pos_node_prev, ios::beg );
        data.write( (char*)&temp_prev, sizeof(NodeBTAVL) );
      }
      else {
        data.seekg( pos_node_prev, ios::beg );
        data.read( (char*)(&temp_prev), sizeof(NodeBTAVL) );
        temp_prev.right = node.left;
        data.seekp( pos_node_prev, ios::beg );
        data.write( (char*)&temp_prev, sizeof(NodeBTAVL) );
      }
      data.seekg( node.left, ios::beg );
      data.read( (char*)(&temp_left), sizeof(NodeBTAVL) );
      node.left = temp_left.right;
      temp_left.right = pos_node;

      data.seekp( pos_node_left, ios::beg );
      data.write( (char*)&temp_left, sizeof(NodeBTAVL) );
      
      data.seekp( pos_node, ios::beg );
      data.write( (char*)&node, sizeof(NodeBTAVL) );
      
      updateHeight(pos_node, data);
      updateHeight(pos_node_left, data);

      node = temp_left;
      pos_node = pos_node_left;
    };
    void right_rota(NodeBTAVL &node, long &pos_node, bool is_left, long pos_node_prev, fstream &data){
      NodeBTAVL temp_prev;
      NodeBTAVL  temp_right;
      long pos_node_right = node.right;
      
      if (pos_node_prev == -1) {
        this->pos_root = node.right;
        data.seekp(0, ios::beg);
        data.write( reinterpret_cast<const char*>(&this->pos_root), sizeof(long) );
      }
      else if (is_left) {
        data.seekg( pos_node_prev, ios::beg );
        data.read( (char*)(&temp_prev), sizeof(NodeBTAVL) );
        temp_prev.left = node.right;
        data.seekp( pos_node_prev, ios::beg );
        data.write( (char*)&temp_prev, sizeof(NodeBTAVL) );
      }
      else {
        data.seekg( pos_node_prev, ios::beg );
        data.read( (char*)(&temp_prev), sizeof(NodeBTAVL) );
        temp_prev.right = node.right;
        data.seekp( pos_node_prev, ios::beg );
        data.write( (char*)&temp_prev, sizeof(NodeBTAVL) );
      }
    
      data.seekg( node.right, ios::beg );
      data.read( (char*)(&temp_right), sizeof(NodeBTAVL) );

      node.right = temp_right.left;
      temp_right.left = pos_node;

      data.seekp( pos_node_right, ios::beg );
      data.write( (char*)&temp_right, sizeof(NodeBTAVL) );

      data.seekp( pos_node, ios::beg );
      data.write( (char*)&node, sizeof(NodeBTAVL) );

      updateHeight(pos_node, data);
      updateHeight(pos_node_right, data);

      node = temp_right;
      pos_node = pos_node_right;
    };
    Record minValue(long node);
    Record maxValue(long node);
    int size(long node);
    bool isBalanced(long node);
};



#endif // AVL_AVL_H
