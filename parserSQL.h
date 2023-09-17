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
        parseTable();
      else
        cout<< "Error esperaba los atributos"<<endl;
      return;
    }
    cout<< "Error esperaba una sentencia SQL." <<endl;
  }
  void parseExpression(){
    atributos.push_back(previous->lexema);
    if(match(Token::COLON))
      parseExpression();
    else
      parseTable();
  }
  
  void parseTable(){
    if(match(Token::FROM)){
      if(match(Token::ID)){
        if(match(Token::SEMICOLON)){
          //funcion para mostrar toda la tabla;
          return;
        }
        parseCondition();
      }
      cout<< "Se esperaba el nombre de una tabla." <<endl;
    }
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
      if(match(Token::ID)){
        if(match(Token::SEMICOLON)){
          //codigo
          return;
        }
        cout<<"Sintaxis incorrecta"<<endl;
        return;
      }
      cout<<"Sintaxis incorrecta"<<endl;
      return;
    }
    else if(match(Token::BETWEEN)){
      parse_range();
    }
    else
      cout<<"Sintaxis incorrecta"<<endl;
    return;
  }

  void parse_range(){
    if(match(Token::NUM)){
      if(match(Token::AND)){
        if(match(Token::NUM)){
          if(match(Token::SEMICOLON)){
            //codigo
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
