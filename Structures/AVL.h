#ifndef AVL_AVL_H
#define AVL_AVL_H

#include <iostream>
#include <fstream>
#include <functional>
#include <vector>

using namespace std;

class AVL
{
public:
    struct Record
    {
        char cod_compra[8];
        char nombre[10];
        char producto[10];
        float precio;
        int cantidad;
        Record() {}
        Record(std::string _cod_compra, std::string _nombre, std::string _producto, std::string _marca, float _precio, int _cantidad);
    };

    struct NodeBT
    {
        Record data;
        long left;
        long right;
        long height;

        NodeBT()
        {
            left = right = -1;
        }

        NodeBT(Record record)
        {
            left = right = -1;
            this->data = record;
        }
    };

private:
    long pos_root;
    string filename;
    long counter = 0;

    long size();
    int sizeNode();
    void addvector(vector<NodeBT> &aux, long value);
    void rangeSearch(long root, string begin, string end, vector<NodeBT> &aux);

public:
    AVL(string filename);
    Record find(char key[8]);
    void insert(Record compra);
    vector<NodeBT> rangeSearch(string begin, string end);
    void print_cod(long temp = 0);

private:
    Record find(long nodepos, char key[5]);
    int i = 0;
    void insert(long node, Record value, bool sit, long father);
    void update(NodeBT &node, long pos);
    int height(long node);
    void balance(long node);
    pair<NodeBT, long> getNode(long pos);
    int balancingFactor(NodeBT node);
    void left_rota(NodeBT &node, long pos);
    void right_rota(NodeBT &node, long pos);
    Record minValue(long node);
    Record maxValue(long node);
    int size(long node);
    bool isBalanced(long node);
    void remove(long node, char value[5], long father = 0, bool sit = true);
};

#endif // AVL_AVL_H