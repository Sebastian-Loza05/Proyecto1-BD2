#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include "AVL.h"

using namespace std;


AVL::AVL(string filename){
    this->filename = filename;
    pos_root = -1;
    fstream file(filename, ios::out | ios::in | ios::binary);
    if (!file.is_open()){
        file.open(filename, ios::out | ios::in | ios::binary | ios::trunc);
    }
    file.close();
}

long AVL::size(){
    ifstream file(filename, ios::binary);
    file.seekg(0, ios::end);
    long nBytes = file.tellg();
    file.close();
    return nBytes;
}

int AVL::sizeNode(){
    return 45;
}

void AVL::addvector(vector<NodeBT> &aux, long value){
    NodeBT temp = getNode(value).first;
    aux.emplace_back(temp);
}

void AVL::rangeSearch(long root, string begin, string end, vector<NodeBT> &aux){
    if (root == -1){
        return;
    }
    if (begin < getNode(root).first.data.cod_compra){
        rangeSearch(getNode(root).first.left, begin, end, aux);
    }
    if (begin <= getNode(root).first.data.cod_compra && end >= getNode(root).first.data.cod_compra){
        addvector(aux, root);
    }
    if (end > getNode(root).first.data.cod_compra){
        rangeSearch(getNode(root).first.right, begin, end, aux);
    }
}

AVL::Record AVL::find(char key[8]){
    Record temp = find(pos_root, key);
    return temp;
}

void AVL::insert(Record compra){
    insert(this->pos_root, compra, true, -1);
}

vector<AVL::NodeBT> AVL::rangeSearch(string begin, string end){
    vector<NodeBT> v;
    rangeSearch(pos_root, begin, end, v);
    return v;
}

void AVL::print_cod(long temp){
    if (pos_root == -1){
        return;
    }
    ifstream file(filename, ios::binary);
    NodeBT aux;
    file.seekg(temp * sizeof(NodeBT));
    file.read(reinterpret_cast<char *>(&aux), sizeof(NodeBT));
    print_cod(aux.left);
    cout << aux.data.cod_compra << endl;
    print_cod(aux.right);
}

AVL::Record AVL::find(long nodepos, char key[5]){
    ifstream file(filename, ios::binary);
    if (nodepos == -1) {
        throw out_of_range("No se encontro");
    } else{
        NodeBT temp;
        file.seekg(nodepos * sizeof(NodeBT));
        file.read(reinterpret_cast<char *>(&temp), sizeof(NodeBT));

        if (key < temp.data.cod_compra){
            file.close();
            return find(temp.left, key);
        } else if (key > temp.data.cod_compra){
            file.close();
            return find(temp.right, key);
        }
        else
            file.close();
        return temp.data;
    }
}

void AVL::insert(long node, Record value, bool sit, long father){
    fstream file(filename, ios::out | ios::in | ios::ate);
    if (node == -1)
    {
        NodeBT temp(value);
        file.seekp(0, ios::end);
        file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBT));
        if (father > -1){
            file.close();
            NodeBT aux = getNode(father).first;
            file.open(filename, ios::out | ios::in | ios::binary);
            if (sit) {
                aux.left = counter;
            }else {aux.right = counter;}
            file.seekp(father * sizeof(NodeBT));
            file.write(reinterpret_cast<char *>(&aux), sizeof(NodeBT));
        }
        counter++;
        file.close();
        pos_root = 0;
        return;
    }

    NodeBT temp;
    file.seekg(node * sizeof(NodeBT));
    file.read(reinterpret_cast<char *>(&temp), sizeof(NodeBT));

    if (temp.data.cod_compra > value.cod_compra){
        file.close();
        insert(temp.left, value, true, node);
    } else if (value.cod_compra >= temp.data.cod_compra){
        file.close();
        insert(temp.right, value, false, node);
    }
    file.close();
    update(temp, node);
    balance(node);
}

void AVL::update(NodeBT &node, long pos){
    int leftHeight = height(node.left);
    int rightHeight = height(node.right);
    node.height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    ofstream file(filename, ios::out | ios::in | ios::binary);
    file.seekp(pos * sizeof(NodeBT));
    file.write(reinterpret_cast<char *>(&node), sizeof(NodeBT));
}

int AVL::height(long node){
    return node == -1 ? 0 : getNode(node).first.height;
}

void AVL::balance(long node){
    if (node == -1){
        return;
    }
    pair<NodeBT, long> temp = getNode(node);
    int balanceFactor = balancingFactor(temp.first);

    if (balanceFactor > 1){
        pair<NodeBT, long> temp2 = getNode(temp.first.left);
        if (balancingFactor(temp2.first) < 0)
        {
            left_rota(temp2.first, temp2.second);
        }
        right_rota(temp.first, temp2.second);
    } else if (balanceFactor < -1){
        pair<NodeBT, long> temp2 = getNode(temp.first.right);
        if (balancingFactor(temp2.first) > 0){
            right_rota(temp2.first, temp2.second);
        }
        left_rota(temp.first, temp2.second);
    }
}

pair<AVL::NodeBT, long> AVL::getNode(long pos){
    if (pos == -1) {
        return pair<NodeBT, long>(NodeBT(), 1);
    }
    NodeBT temp;
    ifstream file(filename, ios::binary);
    file.seekg(pos * sizeof(NodeBT));
    file.read(reinterpret_cast<char *>(&temp), sizeof(NodeBT));
    file.close();
    return pair<NodeBT, long>(temp, pos);
}

int AVL::balancingFactor(NodeBT node){
    if (node.left && node.right){
        return getNode(node.left).first.height - getNode(node.right).first.height;
    } else if (node.left && node.right == -1){
        return getNode(node.left).first.height;
    } else if (node.left == -1 && node.right){
        return getNode(-node.right).first.height;
    }
}

void AVL::left_rota(NodeBT &node, long pos){
    pair<NodeBT, long> temp = getNode(node.right);
    node.right = temp.first.left;
    temp.first.left = pos;
    pair<NodeBT, long> temp2 = getNode(node.left);
    node = temp.first;
    fstream file(filename, ios::out | ios::in | ios::ate);
    file.seekp(pos * sizeof(NodeBT));
    file.write(reinterpret_cast<char *>(&node), sizeof(NodeBT));
    file.seekp(temp2.second * sizeof(NodeBT));
    file.write(reinterpret_cast<char *>(&temp2.first), sizeof(NodeBT));
    update(temp2.first, temp2.second);
    update(node, pos);
    file.close();
}

void AVL::right_rota(NodeBT &node, long pos){
    pair<NodeBT, long> temp = getNode(node.left);
    node.left = temp.first.right;
    temp.first.right = pos;
    pair<NodeBT, long> temp2 = getNode(node.right);
    node = temp.first;
    fstream file(filename, ios::out | ios::in | ios::ate);
    file.seekp(pos * sizeof(NodeBT));
    file.write(reinterpret_cast<char *>(&node), sizeof(NodeBT));
    file.seekp(temp2.second * sizeof(NodeBT));
    file.write(reinterpret_cast<char *>(&temp2.first), sizeof(NodeBT));
    update(temp2.first, temp2.second);
    update(node, pos);
    file.close();
}

AVL::Record AVL::minValue(long node){
    ifstream file(filename, ios::binary);
    NodeBT temp = getNode(node).first;
    if (temp.left == -1){
        file.close();
        return temp.data;
    }
    return minValue(temp.left);
}

AVL::Record AVL::maxValue(long node){
    ifstream file(filename, ios::binary);
    NodeBT temp = getNode(node).first;
    if (temp.right == -1){
        file.close();
        return temp.data;
    }
    file.close();
    return maxValue(temp.right);
}

int AVL::size(long node){
    return node == -1 ? 0 : size(getNode(node).first.left) + 1 + size(getNode(node).first.right);
}

bool AVL::isBalanced(long node){
    if (node == -1){
        return true;
    }
    int bf = balancingFactor(getNode(node).first);
    if (bf > 1 || bf < -1){
        return false;
    }
    return isBalanced(getNode(node).first.left) && isBalanced(getNode(node).first.right);
}

void AVL::remove(long node, char value[5], long father, bool sit)
{
    if (node == -1)
        return;
    if (value < getNode(node).first.data.cod_compra){
        remove(getNode(node).first.left, value, node);
        balance(node);
    }
    else if (value > getNode(node).first.data.cod_compra){
        remove(getNode(node).first.right, value, node);
        balance(node);
    }
    else if (getNode(node).first.left > -1 && getNode(node).first.right > -1){
        fstream file(filename, ios::out | ios::in | ios::ate);
        NodeBT temp = getNode(node).first;
        Record aux = minValue(temp.right);
        temp.data = aux;
        file.seekp(node * sizeof(NodeBT));
        file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBT));
        balance(node);
        remove(temp.right, temp.data.cod_compra, node);
    }
    else {
        if (getNode(node).first.right > -1){
            fstream file(filename, ios::out | ios::in | ios::ate);
            NodeBT temp = getNode(father).first;
            temp.right = getNode(node).first.right;
            file.seekp(father * sizeof(NodeBT));
            file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBT));
            balance(father);
        }
        else if (getNode(node).first.left > -1){
            fstream file(filename, ios::out | ios::in | ios::ate);
            NodeBT temp = getNode(father).first;
            temp.left = getNode(node).first.left;
            file.seekp(father * sizeof(NodeBT));
            file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBT));
            balance(father);
        }
        else{
            fstream file(filename, ios::out | ios::in | ios::ate);
            NodeBT temp = getNode(father).first;
            if (node == temp.right){
                temp.right = -1;
                file.seekp(father * sizeof(NodeBT));
                file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBT));
                balance(father);
            }
            else if (node == temp.left){
                temp.left = -1;
                file.seekp(father * sizeof(NodeBT));
                file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBT));
                balance(father);
            }
            else{
                temp.right = temp.left = -1;
                file.seekp(father * sizeof(NodeBT));
                file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBT));
                balance(father);
            }
        }
    }
}
