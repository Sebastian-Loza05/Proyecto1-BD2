#include "AVLFile.h"
// #include "AVL.h"

using namespace std;


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
Record AVLFile<T>::find(T key){
    Record temp = find(pos_root, key);
    return temp;
}


template<typename T>
vector<NodeBTAVL> AVLFile<T>::rangeSearch(string begin, string end){
    vector<NodeBTAVL> v;
    rangeSearch(pos_root, begin, end, v);
    return v;
}






template<typename T>
Record AVLFile<T>::find(long nodepos, T key){
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

