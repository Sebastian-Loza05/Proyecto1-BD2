#include "AVLFile.h"
// #include "AVL.h"

using namespace std;

template<typename T>
AVLFile<T>::AVLFile(){
  this->filename = "AVLdata.dat";
  ofstream file(this->filename, ios::binary | ios::app);
  file.seekp(0, ios::end);

  int tam = file.tellp();
  if (tam == 0) {
    this->pos_root = -1;
  }
  else {
    this->pos_root = 0;
  }
  file.close();
}

template<typename T>
long AVLFile<T>::size(){
    ifstream file(filename, ios::binary);
    file.seekg(0, ios::end);
    long nBytes = file.tellg();
    file.close();
    return nBytes;
}

template<typename T>
int AVLFile<T>::sizeNode(){
    return 45;
}
template<typename T>
void AVLFile<T>::addvector(vector<NodeBTAVL> &aux, long value){
    NodeBTAVL temp = getNode(value).first;
    aux.emplace_back(temp);
}
template<typename T>
void AVLFile<T>::rangeSearch(long root, string begin, string end, vector<NodeBTAVL> &aux){
    if (root == -1){
        return;
    }
    if (begin < getNode(root).first.data.key){
        rangeSearch(getNode(root).first.left, begin, end, aux);
    }
    if ( begin <= getNode(root).first.data.key && end >= getNode(root).first.data.key ){
        addvector(aux, root);
    }
    if (end > getNode(root).first.data.key){
        rangeSearch(getNode(root).first.right, begin, end, aux);
    }
}
template<typename T>
Record AVLFile<T>::find(char key[8]){
    Record temp = find(pos_root, key);
    return temp;
}
// template<typename T>
// bool AVLFile<T>::add(Record record){
//   bool res = false;
//   
//   fstream file(filename, ios::out | ios::in | ios::ate);
//   if (!file.is_open()) return false;
//
//   if (this->pos_root == -1) {
//     NodeBTAVL temp(record);
//
//     file.seekp(0, ios::end);
//     file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
//     file.close();
//     this->pos_root = 0;
//     return true;
//   }
//
//   res = add(this->pos_root, record, true, -1, file);
//
//   return res;
// }

template<typename T>
bool AVLFile<T>::add(Record compra){
  add(this->pos_root, compra, true, -1);
  return true;
}

// template<typename T>
// bool AVLFile<T>::add(long node, Record value, bool sit, long father, fstream &file){
//   if (node == -1){
//     file.seekp(0, ios::end);
//     file.write(reinterpret_cast<char *>(&value), sizeof(NodeBTAVL));
//
//       if (father > -1){
//           file.close();
//           NodeBTAVL aux = getNode(father).first;
//           file.open(filename, ios::out | ios::in | ios::binary);
//           if (sit) {
//               aux.left = counter;
//           }else {aux.right = counter;}
//           file.seekp(father * sizeof(NodeBTAVL));
//           file.write(reinterpret_cast<char *>(&aux), sizeof(NodeBTAVL));
//       }
//       counter++;
//       file.close();
//       pos_root = 0;
//       return true;
//   }
//
//   NodeBTAVL temp;
//   file.seekg(node * sizeof(NodeBTAVL));
//   file.read(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
//
//   if ( menor(value.key, temp.data.key) ){
//   // if (temp.data.key > value.key){ // value.key < temp.data.key
//     add(temp.left, value, true, node, file);
//   } 
//   else if ( menor_igual(temp.data.key, value.key) ) {
//   // else if (value.key >= temp.data.key){ // temp.data.key <= value.key
//     add(temp.right, value, false, node, file);
//   }
//   file.close();
//   update(temp, node);
//   balance(node);
//   return true;
// }

template<typename T>
bool AVLFile<T>::add(long node, Record value, bool sit, long father){
    fstream file(filename, ios::out | ios::in | ios::ate);
    if (node == -1)
    {
        NodeBTAVL temp(value);
        file.seekp(0, ios::end);
        file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
        if (father > -1){
            file.close();
            NodeBTAVL aux = getNode(father).first;
            file.open(filename, ios::out | ios::in | ios::binary);
            if (sit) {
                aux.left = counter;
            }else {aux.right = counter;}
            file.seekp(father * sizeof(NodeBTAVL));
            file.write(reinterpret_cast<char *>(&aux), sizeof(NodeBTAVL));
        }
        counter++;
        file.close();
        pos_root = 0;
        return true;
    }

    NodeBTAVL temp;
    file.seekg(node * sizeof(NodeBTAVL));
    file.read(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));

    if (temp.data.key > value.key){
        file.close();
        add(temp.left, value, true, node);
    } else if (value.key>= temp.data.key){
        file.close();
        add(temp.right, value, false, node);
    }
    file.close();
    update(temp, node);
    balance(node);
  return true;
}

template<typename T>
vector<NodeBTAVL> AVLFile<T>::rangeSearch(string begin, string end){
    vector<NodeBTAVL> v;
    rangeSearch(pos_root, begin, end, v);
    return v;
}

template<typename  T>
void AVLFile<T>::display(long pos, ifstream &data){
  if (pos == -1) {
    return;
  }
  NodeBTAVL temp;
  data.seekg(pos * sizeof(NodeBTAVL));
  data.read(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));

  display(temp.left, data);
  cout << temp.data.key << endl;
  display(temp.right, data);
}

template<typename T>
void AVLFile<T>::display(){
  ifstream data(filename, ios::binary);
  display(pos_root, data);
}


template<typename T>
Record AVLFile<T>::find(long nodepos, char key[5]){
    ifstream file(filename, ios::binary);
    if (nodepos == -1) {
        throw out_of_range("No se encontro");
    } else{
        NodeBTAVL temp;
        file.seekg(nodepos * sizeof(NodeBTAVL));
        file.read(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));

        if (key < temp.data.key){
            file.close();
            return find(temp.left, key);
        } else if (key > temp.data.key){
            file.close();
            return find(temp.right, key);
        }
        else
            file.close();
        return temp.data;
    }
}

template<typename T>
void AVLFile<T>::update(NodeBTAVL &node, long pos){
    int leftHeight = height(node.left);
    int rightHeight = height(node.right);
    node.height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    ofstream file(filename, ios::out | ios::in | ios::binary);
    file.seekp(pos * sizeof(NodeBTAVL));
    file.write(reinterpret_cast<char *>(&node), sizeof(NodeBTAVL));
}
template<typename T>
int AVLFile<T>::height(long node){
    return node == -1 ? 0 : getNode(node).first.height;
}
template<typename T>
void AVLFile<T>::balance(long node){
    if (node == -1){
        return;
    }
    pair<NodeBTAVL, long> temp = getNode(node);
    int balanceFactor = balancingFactor(temp.first);

    if (balanceFactor > 1){
        pair<NodeBTAVL, long> temp2 = getNode(temp.first.left);
        if (balancingFactor(temp2.first) < 0)
        {
            left_rota(temp2.first, temp2.second);
        }
        right_rota(temp.first, temp2.second);
    } else if (balanceFactor < -1){
        pair<NodeBTAVL, long> temp2 = getNode(temp.first.right);
        if (balancingFactor(temp2.first) > 0){
            right_rota(temp2.first, temp2.second);
        }
        left_rota(temp.first, temp2.second);
    }
}
template<typename T>
pair<NodeBTAVL, long> AVLFile<T>::getNode(long pos){
    if (pos == -1) {
        return pair<NodeBTAVL, long>(NodeBTAVL(), 1);
    }
    NodeBTAVL temp;
    ifstream file(filename, ios::binary);
    file.seekg(pos * sizeof(NodeBTAVL));
    file.read(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
    file.close();
    return pair<NodeBTAVL, long>(temp, pos);
}
template<typename T>
int AVLFile<T>::balancingFactor(NodeBTAVL node){
    if (node.left && node.right){
        return getNode(node.left).first.height - getNode(node.right).first.height;
    } else if (node.left && node.right == -1){
        return getNode(node.left).first.height;
    } else if (node.left == -1 && node.right){
        return getNode(-node.right).first.height;
    }
  return 0;
}
template<typename T>
void AVLFile<T>::left_rota(NodeBTAVL &node, long pos){
    pair<NodeBTAVL, long> temp = getNode(node.right);
    node.right = temp.first.left;
    temp.first.left = pos;
    pair<NodeBTAVL, long> temp2 = getNode(node.left);
    node = temp.first;
    fstream file(filename, ios::out | ios::in | ios::ate);
    file.seekp(pos * sizeof(NodeBTAVL));
    file.write(reinterpret_cast<char *>(&node), sizeof(NodeBTAVL));
    file.seekp(temp2.second * sizeof(NodeBTAVL));
    file.write(reinterpret_cast<char *>(&temp2.first), sizeof(NodeBTAVL));
    update(temp2.first, temp2.second);
    update(node, pos);
    file.close();
}
template<typename T>
void AVLFile<T>::right_rota(NodeBTAVL &node, long pos){
    pair<NodeBTAVL, long> temp = getNode(node.left);
    node.left = temp.first.right;
    temp.first.right = pos;
    pair<NodeBTAVL, long> temp2 = getNode(node.right);
    node = temp.first;
    fstream file(filename, ios::out | ios::in | ios::ate);
    file.seekp(pos * sizeof(NodeBTAVL));
    file.write(reinterpret_cast<char *>(&node), sizeof(NodeBTAVL));
    file.seekp(temp2.second * sizeof(NodeBTAVL));
    file.write(reinterpret_cast<char *>(&temp2.first), sizeof(NodeBTAVL));
    update(temp2.first, temp2.second);
    update(node, pos);
    file.close();
}
template<typename T>
Record AVLFile<T>::minValue(long node){
    ifstream file(filename, ios::binary);
    NodeBTAVL temp = getNode(node).first;
    if (temp.left == -1){
        file.close();
        return temp.data;
    }
    return minValue(temp.left);
}
template<typename T>
Record AVLFile<T>::maxValue(long node){
    ifstream file(filename, ios::binary);
    NodeBTAVL temp = getNode(node).first;
    if (temp.right == -1){
        file.close();
        return temp.data;
    }
    file.close();
    return maxValue(temp.right);
}
template<typename T>
int AVLFile<T>::size(long node){
    return node == -1 ? 0 : size(getNode(node).first.left) + 1 + size(getNode(node).first.right);
}
template<typename T>
bool AVLFile<T>::isBalanced(long node){
    if (node == -1){
        return true;
    }
    int bf = balancingFactor(getNode(node).first);
    if (bf > 1 || bf < -1){
        return false;
    }
    return isBalanced(getNode(node).first.left) && isBalanced(getNode(node).first.right);
}
template<typename T>
void AVLFile<T>::remove(long node, char value[5], long father, bool sit)
{
    if (node == -1)
        return;
    if (value < getNode(node).first.data.key){
        remove(getNode(node).first.left, value, node);
        balance(node);
    }
    else if (value > getNode(node).first.data.key){
        remove(getNode(node).first.right, value, node);
        balance(node);
    }
    else if (getNode(node).first.left > -1 && getNode(node).first.right > -1){
        fstream file(filename, ios::out | ios::in | ios::ate);
        NodeBTAVL temp = getNode(node).first;
        Record aux = minValue(temp.right);
        temp.data = aux;
        file.seekp(node * sizeof(NodeBTAVL));
        file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
        balance(node);
        remove(temp.right, temp.data.key, node);
    }
    else {
        if (getNode(node).first.right > -1){
            fstream file(filename, ios::out | ios::in | ios::ate);
            NodeBTAVL temp = getNode(father).first;
            temp.right = getNode(node).first.right;
            file.seekp(father * sizeof(NodeBTAVL));
            file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
            balance(father);
        }
        else if (getNode(node).first.left > -1){
            fstream file(filename, ios::out | ios::in | ios::ate);
            NodeBTAVL temp = getNode(father).first;
            temp.left = getNode(node).first.left;
            file.seekp(father * sizeof(NodeBTAVL));
            file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
            balance(father);
        }
        else{
            fstream file(filename, ios::out | ios::in | ios::ate);
            NodeBTAVL temp = getNode(father).first;
            if (node == temp.right){
                temp.right = -1;
                file.seekp(father * sizeof(NodeBTAVL));
                file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
                balance(father);
            }
            else if (node == temp.left){
                temp.left = -1;
                file.seekp(father * sizeof(NodeBTAVL));
                file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
                balance(father);
            }
            else{
                temp.right = temp.left = -1;
                file.seekp(father * sizeof(NodeBTAVL));
                file.write(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
                balance(father);
            }
        }
    }
}

template<typename  T>
void AVLFile<T>::display_all(){
  ifstream data(this->filename, ios::binary);
  data.seekg(0, ios::end);
  int tam = data.tellg();
  tam /= sizeof(NodeBTAVL);
  data.seekg(0, ios::beg);
  NodeBTAVL temp;
  for (int i = 0; i < tam; i++) {
    data.read(reinterpret_cast<char *>(&temp), sizeof(NodeBTAVL));
    temp.print();
  }
  data.close();
  return;
}
