#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include "tokenSQL.h"
#include <vector>
using namespace std;


class Parser {
  Scanner* scanner;
  Token* current, * previous;
  vector<string> atributos;
public:
  Parser(Scanner* sc):scanner(sc) {
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
        if(match(Token::ID))
          parseTable(temp);
      }
    }
    cout<< "Error esperaba una sentencia SQL." <<endl;
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
