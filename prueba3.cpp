#include "tokenSQL.h"

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
  cout << tk << endl;
  cout << "-------*-------" << endl;
  while (tk->type != Token::END) {
    cout << tk << endl;
    delete tk;
    tk =  scanner.nextToken();
  }
  cout << tk << endl;
  delete tk;

}


