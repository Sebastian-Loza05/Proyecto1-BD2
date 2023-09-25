#include <iostream>
// #include "AvlFile.h"
#include "BPlusFile.h"
// #include "SequentialFile.h"
// #include "methods.h"
#include "parserSQL.h"

using namespace std;

int main (int argc, char *argv[]) {

  //Se pedira escoger con que metodo se trabajara(pensando en la UI).
  // int metodo;
  // do{
  //   cout<<"1.- AvlFile."<<endl;
  //   cout<<"2.- Sequential File."<<endl;
  //   cout<<"3.- Bplus File."<<endl;
  //   cout<<"Ingrese el metodo a utilizar: ";
  //   cin>>metodo;
  // }while(metodo < 1 || metodo > 3);

  string sentencia = "";
  while(true){
    cout<<"Inserte su sentencia SQL: "<<endl;
    getline(cin, sentencia);
    Scanner scanner(sentencia + " ");
    Parser parser(&scanner);
    parser.parse();
    cout<<error_message<<endl;
  }


  // if(metodo == 1)
  //   MethodSelector<AVLFile> method;
  // else if(metodo == 2)
  //   MethodSelector<SequentialFile> method;
  // else
  //   MethodSelector<BPlusFile<int>> method;



  return 0;
}
/*
#include <iostream>
#include <typeinfo>
#include <sstream>
// #include "BPlusFile.h"
// #include "SequentialFile.h"
#include "Structures/AVLFile.cpp"

using namespace std;

void ingreso(MethodSelector *method){
    // BPlusFile<long , sizeof(long)> bplus;
    ifstream archivo("datos.csv");

    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo" << endl;
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

        char key[20];
        // long key;
        char nombre[20];
        char producto[20];
        char marca[20];
        float precio;
        int cantidad;

        // key = stoi(campos[0]);

        strncpy(key, campos[0].c_str(), sizeof(key) - 1);
        nombre[sizeof(key) - 1 ]= '\0';

        strncpy(nombre, campos[1].c_str(), sizeof(nombre) - 1);
        nombre[sizeof(nombre) - 1 ]= '\0';


        strncpy(producto, campos[2].c_str(), sizeof(producto) - 1);
        producto[sizeof(producto) - 1 ]= '\0';

        strncpy(marca, campos[3].c_str(), sizeof(marca) - 1);
        marca[sizeof(marca) - 1 ]= '\0';

        // cout << campos[4] << endl;
        precio = stof(campos[4]);
        cantidad = stoi(campos[5]);

        method->display_all();
        Record record(key,nombre,producto,marca,precio,cantidad);
        // record.print();
        string asd;
        // if (counter > 96)
        //   cin >> asd;
        bool asd__ = method->add(record);
        // cout << "Asda" << endl;
        // cout << "Asda" << endl;
        // for (const string& valor: campos){
        //   cout << valor << "\t";
        // }
        // cout << endl;
        cout<<"counter: "<<counter<<endl;
        counter++;
        cout << "------" << endl;
        method->display();
        cout << "------" << endl;

        // bplus.displayTree();
        //
        cin >> asd;
        if(counter == 200000)break;
        campos.clear();
    }
    // bplus.displayTree();

    archivo.close();

    return;
}

int main (int argc, char *argv[]) {

    //Se pedira escoger con que metodo se trabajara(pensando en la UI)

    MethodSelector *method = nullptr;
    method = new AVLFile<char*>(); // STRUCT 1
    ingreso(method);

    return 0;
}



#include <GUI/QGui  Application>
#include <Gui/Gui/QQmlApplicationEngine>
#include "Gui/Gui/src/BridgeClass.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<BridgeClass>("io.qt.bridge", 1, 0, "BridgeClass");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
*/
