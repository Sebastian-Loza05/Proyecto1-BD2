#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct Variable {
  string name;
  string type;
};

string generateStructCode(const std::vector<Variable>& variables) {
  string code = "";
  code += "#pragma once\n";
  code += "#include <iostream>\nusing namespace std;\n";
  code += "\nstruct Record1 {\n";
  for (const Variable& var : variables) {
    code += "    " + var.type + " " + var.name + ";\n";
  }
  code += "};\n";
  return code;
}

void crearStruct(){
  vector<Variable> variables;
  while(true){
    Variable var;
    cout<<"Ingrese el nombre de la variable o fin para terminar: ";
    cin>>var.name;
    if (var.name == "fin")
      break;

    cout<<"Ingrese el tipo de la variable o fin para terminar: ";
    cin>>var.type;
    variables.push_back(var);
  } 

  string strucCode = generateStructCode(variables);
  fstream header("structs.h", ios::out | ios::trunc);
  if(header.is_open()){
    header<<strucCode;
  }
}
