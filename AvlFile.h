//Es la base del AvlFile, falta balanceo al insertar y el remove y tambien verificar y balancear en el archivo.
//
//Cambia tambien a que ahora el nombre del archivo que se usa sea un nombre por defecto y no que se pase por el constructor.
#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <iostream>
using namespace std;

struct Record
{
  int cod;
	char nombre[12];
	int ciclo;
	long left, right;

  void setData() {
    cout << "Codigo:";
    cin >> cod;
    cout << "Nombre: ";
    cin >> nombre;
    cout << "Ciclo: ";
    cin >> ciclo;
    left = -1;
    right = -1;
  }

  void showData() {
    cout << "\nCodigo: " << cod;
    cout << "\nNombre: " << nombre;
    cout << "\nCiclo : " << ciclo;
    cout << "\nLeft : " << left;
    cout << "\nRight : " << right <<endl;
  }
};

class AVLFile
{
private:
    string filename;
    long pos_root;
public:
  AVLFile(){}
  AVLFile(string filename){
    this->pos_root = 0;
    this->filename = filename;
    ofstream file(this->filename, ios::binary | ios::app);
    if(!file.is_open()) throw "No se pudo abrir";
    file.seekp(0, ios::end);
    if( file.tellp() != 0) pos_root = 1;
    file.close();
	}
	
    
  Record find(int key){
    if (pos_root == 0) {
      cout << "No existe el registro buscado" << endl;
      throw "Posicion invalida";
    }
    ifstream file(this->filename, ios::binary);

    return find(pos_root, key, file);
  }
    
  void insert(Record record){
    ofstream file_e(this->filename, ios::binary | ios::app);
    ifstream file_l(this->filename, ios::binary);
    if(!file_l.is_open()) throw "No se pudo abrir";
    if(!file_e.is_open()) throw "No se pudo abrir";
    file_l.seekg(0, ios::end);
    int tam = file_l.tellg();
    tam /= sizeof(record);
		insert(pos_root, record, file_e ,file_l, tam);
    file_e.close();
    file_l.close();
	}
	
  vector<Record> inorder(){
    ifstream file_l(this->filename, ios::binary);
    if(!file_l.is_open()) throw "No se pudo abrir";
    vector<Record> display;
		inorder(pos_root, display, file_l);
    return display;
	}

  void show_data(){
    ifstream file(this->filename, ios::binary);
    if(!file.is_open()) throw ("No se pudo abrir el archivo");
    Record record;
    while (file.peek() != EOF) {
      record = Record();
      file.read(reinterpret_cast<char* >(&record), sizeof(record));
      record.showData();
    }
    file.close();
  }

private:
  Record find(long pos_node, int key, ifstream &file){
    if (pos_node == -1) {
      cout << "No existe el registro buscado" << endl;
      throw "Posicion invalida";
    }

    file.seekg(sizeof(Record)*(pos_node-1), ios::beg); 
    Record record = Record();
    file.read(reinterpret_cast<char*>(&record), sizeof(Record));

    if (record.cod == key) 
      return record;
    else if (key < record.cod)
      return find(record.left, key,file);
    else
      return find(record.right, key,file);
  }    

  void insert(long pos_node, Record record, ofstream &file_e, ifstream &file_l, long tam){
    Record matri = Record();
    if(pos_node !=  0){
      long pos = pos_node-1;
      file_l.seekg( pos*sizeof(record), ios::beg);
      file_l.read((char*)(&matri), sizeof(matri));
    }else{
      this->pos_root = 1;
    }

    if (pos_node == -1 || pos_node == 0){
      file_e.seekp(0, ios::end);
      file_e.write((char*)&record, sizeof(record));
      return;
    }
    else if(record.cod > matri.cod){
      long new_pos_node = matri.right;
      if (new_pos_node == -1){
        fstream file(this->filename, ios::binary | ios::in | ios::out);
        file.seekp( (pos_node*sizeof(record)) - sizeof(long), ios::beg);
        long tam_n = tam + 1;
        file.write(reinterpret_cast<const char*>(&tam_n), sizeof(long));
        file.close();
      }
      insert(new_pos_node, record, file_e, file_l, tam);
    }
    else if(record.cod < matri.cod) {
      long new_pos_node = matri.left;
      if (new_pos_node == -1){
        fstream file(this->filename, ios::binary | ios::in | ios::out);
        file.seekp( (pos_node*sizeof(record)) - (sizeof(long)*2), ios::beg);
        long tam_n = tam + 1;
        file.write(reinterpret_cast<const char*>(&tam_n), sizeof(long));
        file.close();
      }
      insert(new_pos_node, record, file_e, file_l, tam);
    }
    else{
      cout<<"Esta llave ya existe"<<endl;
      return;
    }
	}
	
	void inorder(long pos_node, vector<Record>& display, ifstream &file_l){
    if(pos_node == -1 || pos_node == 0) return;
    
    long pos = --pos_node;
    file_l.seekg(pos*sizeof(Record), ios::beg);
    Record record = Record();
    file_l.read((char*)&record, sizeof(record));
    
    inorder(record.left, display, file_l);
    display.push_back(record);
    inorder(record.right, display, file_l);
	}
};
