#include "tokenSQL.h"
#include <string>
#include <vector>

vector<string> input(string ingreso){
  vector<string> vec;
  string valor = "";
  for (int i = 0; i < ingreso.size(); i++) {
    if (ingreso[i] == ';') {
      valor+=ingreso[i];
      vec.push_back(valor);
      valor = "";
    }
    else{
      valor+=ingreso[i];
    }
  }
  if (valor != "") {
    vec.push_back(valor);
  }
  return vec;
}

int main(int argc, const char* argv[]) {
  
  
  string sentence = "";
  getline(cin, sentence);

  // Cambiar:
  // input es ahora el nombre de archivo con las instruccion de la SM
  
  Scanner scanner(sentence+" ");
  Token* tk = scanner.nextToken();
  cout << "-------*-------" << endl;
  while (tk->type != Token::END && tk->type != Token::SEMICOLON) {
    cout << tk << endl;
    delete tk;
    tk =  scanner.nextToken();
    if (tk->type == Token::END) {
      cout << "-> END" << endl;
    }
    if (tk->type == Token::SEMICOLON) {
      cout << "-> SEMICOLON" << endl;
    }
  }
  cout << tk << endl;
  delete tk;

}


