#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>
#include <map>

using namespace std;

/*
  Autores:
  Sebastian Loza
  Luis Golac
*/

// Incluir estructura de datos que permita la busqueda de las palabras reservadas

class Token {
public:
  enum Type { PUSH, JMPEQ, JMPGT, JMPGE, JMPLT, JMPLE, SKIP, POP, DUP, SWAP, ADD, SUB, MUL, DIV, STORE, LOAD, GOTO, ID, LABEL, NUM, EOL, CADENA, ERR, END};
  static const char* token_names[24]; 
  Type type;
  string lexema;
  Token(Type type):type(type) { lexema = ""; }
  Token(Type, char c);
  Token(Type type, const string source):type(type) {
    lexema = source;
  }

};

const char* Token::token_names[24] = { "PUSH", "JMPEQ", "JMPGT", "JMPLT", "JMPGE", "JMPLE", "SKIP", "POP", "DUP", "SWAP", "ADD", "SUB", "MUL", "DIV", "STORE", "LOAD", "GOTO", "ID", "LABEL", "NUM", "EOL", "CADENA", "ERR", "END"};


// Modificar
std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
  if (tok.lexema.empty())
    return outs << Token::token_names[tok.type];
  else
    return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
  return outs << *tok;
}

class PalabrasReservadas {
  map<string, Token::Type> palabras;

public:
  PalabrasReservadas(){
    palabras.insert(make_pair("push", Token::PUSH));
    palabras.insert(make_pair("jmpeq", Token::JMPEQ));
    palabras.insert(make_pair("jmpgt", Token::JMPGT));
    palabras.insert(make_pair("jmpge", Token::JMPGE));
    palabras.insert(make_pair("jmplt", Token::JMPLT));
    palabras.insert(make_pair("jmple", Token::JMPLE));
    palabras.insert(make_pair("skip", Token::SKIP));
    palabras.insert(make_pair("pop", Token::POP));
    palabras.insert(make_pair("dup", Token::DUP));
    palabras.insert(make_pair("swap", Token::SWAP));
    palabras.insert(make_pair("add", Token::ADD));
    palabras.insert(make_pair("sub", Token::SUB));
    palabras.insert(make_pair("mul", Token::MUL));
    palabras.insert(make_pair("div", Token::DIV));
    palabras.insert(make_pair("store", Token::STORE));
    palabras.insert(make_pair("load", Token::LOAD));  
    palabras.insert(make_pair("goto", Token::GOTO));  
  }

  Token::Type search(string lexema){
    auto it = palabras.find(lexema);
    if(it != palabras.end()) return it->second;
    return Token::CADENA;
  }
};


class Scanner {
public:
  Scanner(string input);
  Token* nextToken();
  ~Scanner();  
private:
  string input;
  PalabrasReservadas palabras;
  int first, current;
  int state;
  bool isNumber(const string& s);
  char nextChar();
  void rollBack();
  void startLexema();
  void incrStartLexema();
  string getLexema();
};


Scanner::Scanner(string _input):input(_input),first(0),current(0) { }

Token* Scanner::nextToken() {
  Token* token;
  char c;
  c = nextChar();
  while (c == ' ' || c == '\t' ) c = nextChar();
  if(c == '\n'){
    c = nextChar();
    while(c == '\n')
      c = nextChar();
    rollBack();
    return new Token(Token::EOL);
  }

  startLexema();
  if (isdigit(c) || isalpha(c)) {
    c = nextChar();
    while (isdigit(c) || isalpha(c)) {
      c = nextChar();
    }
    if(c == ':'){
      rollBack();
      string a = getLexema();
      c = nextChar();
      return new Token(Token::LABEL, a);
    }

    rollBack();
    string lexema = getLexema();
    if(current == input.length()){
      current-=2;
      lexema = getLexema();
      current++;
    }
    Token::Type ktype = palabras.search(lexema);
    if(ktype != Token::CADENA){
      return new Token(ktype);
    }

    if(isNumber(getLexema()))
      return new Token(Token::NUM, getLexema());

    return new Token(Token::ID, getLexema());
      
  } else if (c == '\0') {
    return new Token(Token::END);
  } else {
    return new Token(Token::ERR,getLexema());
  }
  return NULL;
}

Scanner::~Scanner() { }

bool Scanner::isNumber(const string& s){
  for (char c : s) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}

char Scanner::nextChar() {
  int c = input[current];
  if (c != '\0') current++;
  return c;
}

void Scanner::rollBack() { // retract
  if (input[current] != '\0')
    current--;
}

void Scanner::startLexema() {
  first = current-1;  
  return;
}

void Scanner::incrStartLexema() {
  first++;
}


string Scanner::getLexema() {
  return input.substr(first,current-first);
}





// ---------------------------------------------------

int main(int argc, const char* argv[]) {
  
  if (argc != 2) {
    cout << "File missing" << endl;
    exit(1);
  }
  std::ifstream t(argv[1]);
  std::stringstream buffer;
  buffer << t.rdbuf();


  // Cambiar:
  // input es ahora el nombre de archivo con las instruccion de la SM
  

  Scanner scanner(buffer.str());
  Token* tk = scanner.nextToken();
  while (tk->type != Token::END) {
    cout << tk << endl;
    delete tk;
    tk =  scanner.nextToken();
  }
  cout << tk << endl;
  delete tk;

}


