#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include "BPlusFile.h"
#include "SequentialFile.h"
#include "Structures/methods.h"
#include "Structures/AVLFile.h"
#include "tokenSQL.h"
#include <utility>
#include <vector>
#include <map>
#include "Structures/methods.h"
using namespace std;

inline MethodSelector* method = new MethodSelector();
inline vector<Record> records;
inline string error_message;

class Parser {
  Scanner* scanner;
  Token* current, * previous;
  vector<string> atributos;
  map<string, string> values;
public:
  Parser(Scanner* sc):scanner(sc) {
    previous = current = NULL;
    return;
  }
  void parse(){
    current = scanner->nextToken();
    if(check(Token::ERR)){
      error_message = "Caracter invalido";
      return;
    }
    parseStatement();
    
    // if (current->type != Token::SEMICOLON)
    //   error_message = "Esperaba fin de input.";
    // else 
    //   cout<< "Fin de la ejecucion" << endl;
    // if(current) delete current;
    return;
  }

private:
  void mostrar(){
    cout<<"Atributos: "<<endl;
    for (auto const &v: atributos){
      cout<<v<<", ";
    }
    cout<<endl;
  }
  
  void mostrarValues(){
    for(auto it : values){
      cout<<it.first<<": "<<it.second<<endl;
    }
  }

  bool match(Token::Type ttype) {
    if (check(ttype)) {
      advance();
      return true;
    }
    return false;
  }

  bool check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
  }


  bool advance() {
    if (!isAtEnd()) {
      Token* temp =current;
      if (previous) delete previous;
      current = scanner->nextToken();
      previous = temp;
      if (check(Token::ERR)) {
        error_message = "Parse error, unrecognised character";
        return false;
      }
      return true;
    }
    return false;
  } 

  bool isAtEnd() {
    return (current->type == Token::END);
  } 

  
  void parseStatement(){
    if(match(Token::SELECT)){
      if(match(Token::ID))
        parseExpression();
      else if(match(Token::ALL))
        parseTable(nullptr);
      else
        error_message =  "Error esperaba los atributos";
      return;
    }
    else if(match(Token::CREATE)){
      if(match(Token::TABLE)){
        Token* temp = previous; 
        if(match(Token::ID)){
          parseTable(temp);
          return;
        }
        error_message = "Se esperaba un nombre de tabla";
        return;
      }
      error_message = "Error de sintaxis";
      return;
    }
    else if(match(Token::INSERT)){
      if(match(Token::INTO)){
        if(match(Token::ID)){
          if(match(Token::VALUES)){
            bool res = parseValuesSentence();
            return;
          }
          error_message = "Esperaba la sentencia VALUES";
          return;
        }
        error_message = "Esperaba el nombre de la tabla";
        return;
      }
      error_message = "Espera INTO en despues del INSERT.";
      return;
    }
    else if(match(Token::DELETE)){
      parseTableDelete();
      return;
    }
    error_message = "Error esperaba una sentencia SQL.";
  }
  bool parseValuesSentence(){
    if(match(Token::LPARENT)){
      bool res = parseValuesList();
      if(!res) return false;
      if(match(Token::RPARENT)){
        if(match(Token::SEMICOLON)){
          //Insertar
          cout<<"Insertar en la tabla values: "<<endl;
          mostrarValues();
          return true;
        }
        error_message = "Esperaba ;";
        return false;
      }
      error_message = "Esperaba )";
      return false;
    }
    error_message = "Se esperaba (.";
    return false;
  }

  bool parseValuesList(){
    bool res = parseValues();
    while(match(Token::COLON)){
       res = parseValues();
    }
    return res;
  }

  bool parseValues(){
    if(match(Token::QUOTE)){
      if(match(Token::ID)){
        string value = previous->lexema;
        if(match(Token::QUOTE)){
          values.insert(make_pair(value, "string"));
          return true;
        }
        error_message = "Esperaba \"";
        return false;
      }
      error_message = "Se esperaba un dato";
      return false;
    }
    else if(match(Token::NUM)){
      values.insert(make_pair(previous->lexema, "int"));
      return true;
    }
    else if(match(Token::FLOAT)){
      values.insert(make_pair(previous->lexema, "float"));
      return true;
    }
    else if(match(Token::TRUE)){
      values.insert(make_pair("true", "bool"));
      return true;
    }
    else if(match(Token::FALSE)){
      values.insert(make_pair("false", "bool"));
      return true;
    }
    error_message = "Sintaxis incorrecta";
    return false;
  }


  void parseExpression(){
    atributos.push_back(previous->lexema);
    if(match(Token::COLON)){
      advance();
      parseExpression();
    }
    else
      parseTable(nullptr);
  }
  
  void parseTable(Token* temp){
    if(match(Token::FROM)){
      if(match(Token::ID)){
        if(match(Token::SEMICOLON)){
          cout<<"Mostrar tabla"<<endl;
          records = method->load();
          atributos.clear();
          return;
        }
        parseCondition();
        return;
      }
      else if(temp!=nullptr && match(Token::FILE)){
        parseFile();
        return;
      }
      error_message = "Error de sintaxis.";
      return;
    }
    error_message = "Error de sintaxis";
  }
  void parseTableDelete(){
    if(match(Token::FROM)){
      if(match(Token::ID)){
        bool r = parseConditionDelete();
        return;
      }
      error_message  = "Se esperaba el nombre de la tabla.";
      return;
    }
    error_message = "Se esperaba la sentencia FROM, se obtuvo ";
  }

  bool parseConditionDelete(){
    if (match(Token::WHERE)){
      if(match(Token::ID)){
        if(match(Token::EQUAL)){
          bool r = parseEqual();
          if(r){
            if(match(Token::SEMICOLON)){
              cout<<" Eliminacion"<<endl;
              return true;
            }
            error_message = "Sintaxis incorrecta, se esperaba ;";
            return false;
          }
          return false;
        }
        error_message = "Se esperaba la un '=' se encontro ";
      }
      error_message = "Se esperaba un id" ;
      return false;
    }
    error_message = "Se esperaba un aHERE";
    return false;
  }

  void parseFile(){
    string filename = "";
    if(match(Token::QUOTE)){
      if(match(Token::FILENAME)){
        filename = previous->lexema;
        if(match(Token::QUOTE)){
          parseIndex(filename);
          return;
        }
        error_message = "Se esperaba \"";
        return;
      }
      error_message = "Se esperaba el nombre del archivo";
      return;
    }
    error_message = "Se esperaba '";
    return;
  }

  void parseIndex(string filename){
    if(match(Token::USING)){
      if(match(Token::INDEX)){
        parseIndexType(filename);
        return;
      }
      error_message = "Sintaxis incorrecta";
      return;
    }
    error_message = "Sintaxis incorrecta";
    return;
  }
  
  void parseIndexType(string filename){
    if(match(Token::BPLUS)){
      if(match(Token::SEMICOLON)){
        method = new BPlusFile<int, sizeof(int)>();
        insertCsv(filename);
        cout<<"Se crea tabla con index bplus"<<endl;
        
        return;
      }
      error_message = "Se esperaba un ;";
      return;
    }
    else if(match(Token::AVL)){
      if(match(Token::SEMICOLON)){
        method = new AVLFile<int>();
        insertCsv(filename);
        cout<<"Se crea tabla con index avl"<<endl;
        return;
      }
      error_message ="Se esperaba un ;";
      return;
    }
    else if(match(Token::SEQUENTIAL)){
      if(match(Token::SEMICOLON)){
        method = new SequentialFile<int>();
        insertCsv(filename);
        cout<<"Se crea tabla con index sequential"<<endl;
        return;
      }
      error_message = "Se esperaba un ;";
      return;
    }
    error_message = "Se esperaba index para hashear";
    return;
  }

  void parseCondition(){
    if (match(Token::WHERE)){
      if(match(Token::ID)){
        parseCondition2();
        return;
      }
      error_message = "Se esperaba un id";
      return;
    }
    error_message = "Se esperaba un WHERE";
    return;
  }

  void parseCondition2(){
    if(match(Token::EQUAL)){
      bool v = parseEqual();
      if(v){
        int value = stoi(previous->lexema);
        if(match(Token::SEMICOLON)){
          records.clear();
          pair<Record,bool> result = method->search(value);
          cout<<"Busqueda unitaria"<<endl;
          if (result.second) {
            records.push_back(result.first);
          }
          else{
            error_message = "No existe un elemento con la llave buscada";
          }
          atributos.clear();
          return;
        }
        error_message = "Sintaxis incorrecta";
        return;
      }
      
      return;
    }
    else if(match(Token::BETWEEN)){
      cout<<"Between"<<endl;
      parse_range();
    }
    else
      error_message = "Sintaxis incorrecta";
    return;
  }

  bool parseEqual(){
    if(match(Token::NUM) | match(Token::ID) ){
      return true;
    }
    else if(match(Token::QUOTE)){
      if(match(Token::ID)){
        if(!match(Token::QUOTE)){
          error_message = "Error de sintaxis, esperaba un \"";
          return false;
        }
        return true;
      }
      error_message = "Sintaxis incorrecta";
      return false;
    }
    error_message  = "Sintaxis incorrecta";
    return false;
  }

  void parse_range(){
    if(match(Token::NUM)){
      int begin = stoi(previous->lexema);
      // cout<"<"numero: "<<previous->lexema<<endl;
      if(match(Token::AND)){
        if(match(Token::NUM)){
          int end = stoi(previous->lexema);
          if(match(Token::SEMICOLON)){
            
            cout<<"Busqueda por rango"<<endl;
            records = method->rangeSearch(begin, end);
            return;
          }
          error_message = "Sintaxis Incorrecta";
          return;
        }
        error_message = "Sintaxis Incorrecta";
        return;
      }
      error_message = "Sintaxis Incorrecta";
      return;
    }
    error_message = "Sintaxis Incorrecta";
    return;
  }

  void insertCsv(string ruta){
    cout << ruta << endl;
    ifstream archivo(ruta);
    cout<<"ruta: "<<ruta<<endl;
    if (!archivo.is_open()) {
      error_message = "La ruta del archivo es incorrecta.";
      return;
    }

    string linea;
    vector<string> campos;
    istringstream lineaStream(linea);
    int counter = 0;
    while (getline(archivo, linea)) {
      istringstream lineaStream(linea);
      string campo;

      while (getline(lineaStream, campo, ',')) {
        campos.push_back(campo);
      }

      //char key[20];
      int key;
      char nombre[20];
      char producto[20];
      char marca[20];
      float precio;
      int cantidad;

      key = stoi(campos[0]);

      //strncpy(key, campos[0].c_str(), sizeof(key) - 1);
      //nombre[sizeof(key) - 1 ]= '\0';

      strncpy(nombre, campos[1].c_str(), sizeof(nombre) - 1);
      nombre[sizeof(nombre) - 1 ]= '\0';


      strncpy(producto, campos[2].c_str(), sizeof(producto) - 1);
      producto[sizeof(producto) - 1 ]= '\0';

      strncpy(marca, campos[3].c_str(), sizeof(marca) - 1);
      marca[sizeof(marca) - 1 ]= '\0';

      // cout << campos[4] << endl;
      precio = stof(campos[4]);
      cantidad = stoi(campos[5]);
      cout << "--------" << endl;
      Record record(key,nombre,producto,marca,precio,cantidad);
      bool asd__ = method->add(record);
      cout << "---*----" << endl;
      method->display_all();
      if(counter == 2000)
        break;
      counter ++;
      campos.clear();
    }
    method->display_all();
  }
};

