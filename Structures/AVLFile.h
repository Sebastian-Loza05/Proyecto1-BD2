#ifndef AVL_AVL_H
#define AVL_AVL_H

#include <fstream>
#include <functional>
#include "methods.h"

using namespace std;

struct NodeBTAVL{
  Record data;
  long left;
  long right;
  long height;

  NodeBTAVL(){
      left = right = -1;
  }

  NodeBTAVL(Record record){
      left = right = -1;
      this->data = record;
  }

  void print(){
    cout << left << "\t" << right << "\t" << height << "\t";
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
    void rangeSearch(long root, string begin, string end, vector<NodeBTAVL> &aux);

public:
    AVLFile();
    bool add(Record record) override;
    Record find(char key[8]);
    vector<NodeBTAVL> rangeSearch(string begin, string end);
    void display() override;


    void display_all() override;

private:
    void display(long pos, ifstream& data);

    bool add(long node, Record value, bool sit, long father);
    Record find(long nodepos, char key[5]);
    void update(NodeBTAVL &node, long pos);
    int height(long node);
    void balance(long node);
    pair<NodeBTAVL, long> getNode(long pos);
    int balancingFactor(NodeBTAVL node);
    void left_rota(NodeBTAVL &node, long pos);
    void right_rota(NodeBTAVL &node, long pos);
    Record minValue(long node);
    Record maxValue(long node);
    int size(long node);
    bool isBalanced(long node);
    void remove(long node, char value[5], long father = 0, bool sit = true);
};

#include "AVLFile.cpp"

#endif // AVL_AVL_H
