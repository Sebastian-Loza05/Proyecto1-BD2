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







