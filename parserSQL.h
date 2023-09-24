#include <sstream>
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


class Parser {
  Scanner* scanner;
  MethodSelector method;
  Token* current, * previous;
  vector<string> atributos;
  map<string, string> values;
public:
  Parser(Scanner* sc, MethodSelector &_method):scanner(sc) {
    method = _method;
    previous = current = NULL;
    return;
  }
  void parse(){
    current = scanner->nextToken();
    if(check(Token::ERR)){
      cout<< "Caracter invalido" << endl;
      return;
    }
    parseStatement();
    
    if (current->type != Token::SEMICOLON)
      cout<<"Esperaba fin de input, se encontro "<< current <<endl;
    else 
      cout<< "Fin de la ejecucion" << endl;
    if(current) delete current;
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
        cout << "Parse error, unrecognised character: " << current->lexema << endl;
        exit(0);
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
        cout<< "Error esperaba los atributos"<<endl;
      return;
    }
    else if(match(Token::CREATE)){
      if(match(Token::TABLE)){
        Token* temp = previous; 
        if(match(Token::ID)){
          parseTable(temp);
          return;
        }
        cout<<"Se esperaba un nombre de tabla"<<endl;
        return;
      }
      cout<<"Error de sintaxis"<<endl;
      return;
    }
    else if(match(Token::INSERT)){
      if(match(Token::INTO)){
        if(match(Token::ID)){
          if(match(Token::VALUES)){
            bool res = parseValuesSentence();
            return;
          }
          cout<<"Esperaba la sentencia VALUES"<<endl;
          return;
        }
        cout<<"Esperaba el nombre de la tabla"<<endl;
        return;
      }
      cout<<"Espera INTO en despues del INSERT."<<endl;
      return;
    }
    else if(match(Token::DELETE)){
      parseTableDelete();
      return;
    }
    cout<< "Error esperaba una sentencia SQL." <<endl;
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
        cout<<"Esperaba ;"<<endl;
        return false;
      }
      cout<<"Esperaba ), se encontro "<<current->lexema<<endl;
      return false;
    }
    cout<<"Se esperaba (, se encontro "<<current<<endl;
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
        cout<<"Esperaba \", se cencontro: "<<current->lexema<<endl;
        return false;
      }
      cout<<"Se esperaba un dato"<<endl;
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
    cout<<"Sintaxis incorrecta"<<endl;
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
      cout<< "Error de sintaxis." <<endl;
      return;
    }
    cout<<"Error de sintaxis"<<endl;
  }
  void parseTableDelete(){
    if(match(Token::FROM)){
      if(match(Token::ID)){
        bool r = parseConditionDelete();
        return;
      }
      cout<<"Se esperaba el nombre de la tabla, se obtuvo "<<current<<endl;
      return;
    }
    cout<<"Se esperaba la sentencia FROM, se obtuvo "<<current<<endl;
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
            cout<<"Sintaxis incorrecta, se esperaba ;"<<endl;
            return false;
          }
          return false;
        }
        cout<<"Se esperaba la un '=' se encontro "<<current<<endl;
      }
      cout<< "Se esperaba un id" <<endl;
      return false;
    }
    cout<< "Se esperaba un WHERE" <<endl;
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
        cout<<"Se esperaba \""<<endl;
        return;
      }
      cout<<"Se esperaba el nombre del archivo"<<endl;
      return;
    }
    cout<<"Se esperaba \""<<endl;
    return;
  }

  void parseIndex(string filename){
    if(match(Token::USING)){
      if(match(Token::INDEX)){
        parseIndexType();
        return;
      }
      cout<<"Sintaxis incorrecta"<<endl;
      return;
    }
    cout<<"Sintaxis incorrecta"<<endl;
    return;
  }
  
  void parseIndexType(){
    MethodSelector *method = new MethodSelector();
    if(match(Token::BPLUS)){
      if(match(Token::SEMICOLON)){
        this->method = BPlusFile<int, sizeof(int)>();
        cout<<"Se crea tabla con index bplus"<<endl;
        
        return;
      }
      cout<<"Se esperaba un ;"<<endl;
      return;
    }
    else if(match(Token::AVL)){
      if(match(Token::SEMICOLON)){
        this->method = AVLFile<int>();
        cout<<"Se crea tabla con index avl"<<endl;
        return;
      }
      cout<<"Se esperaba un ;"<<endl;
      return;
    }
    else if(match(Token::SEQUENTIAL)){
      if(match(Token::SEMICOLON)){
        this->method = SequentialFile<int>();
        cout<<"Se crea tabla con index sequential"<<endl;
      }
      cout<<"Se esperaba un ;"<<endl;
      return;
    }
    cout<<"Se esperaba index para hashear"<<endl;
    return;
  }

  void parseCondition(){
    if (match(Token::WHERE)){
      if(match(Token::ID)){
        parseCondition2();
        return;
      }
      cout<< "Se esperaba un id" <<endl;
      return;
    }
    cout<< "Se esperaba un WHERE" <<endl;
    return;
  }

  void parseCondition2(){
    if(match(Token::EQUAL)){
      bool v = parseEqual();
      if(v){
        if(match(Token::SEMICOLON)){
          cout<<"Busqueda unitaria"<<endl;
          atributos.clear();
          return;
        }
        cout<<"Sintaxis incorrecta"<<endl;
        return;
      }
      return;
    }
    else if(match(Token::BETWEEN)){
      cout<<"Between"<<endl;
      parse_range();
    }
    else
      cout<<"Sintaxis incorrecta"<<endl;
    return;
  }

  bool parseEqual(){
    if(match(Token::NUM)){
      return true;
    }
    else if(match(Token::QUOTE)){
      if(match(Token::ID)){
        if(!match(Token::QUOTE)){
          cout<<"Error de sintaxis, esperaba un \""<<endl;
          return false;
        }
        return true;
      }
      cout<<"Sintaxis incorrecta"<<endl;
      return false;
    }
    cout<<"Sintaxis incorrecta"<<endl;
    return false;
  }

  void parse_range(){
    if(match(Token::NUM)){
      cout<<"numero: "<<previous->lexema<<endl;
      if(match(Token::AND)){
        if(match(Token::NUM)){
          if(match(Token::SEMICOLON)){
            cout<<"Busqueda por rango"<<endl;
            return;
          }
          cout<<"Sintaxis Incorrecta"<<endl;
          return;
        }
        cout<<"Sintaxis Incorrecta"<<endl;
        return;
      }
      cout<<"Sintaxis Incorrecta"<<endl;
      return;
    }
    cout<<"Sintaxis Incorrecta"<<endl;
    return;
  }
};
