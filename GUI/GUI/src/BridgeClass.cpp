#include "BridgeClass.h"
using namespace std;

struct record {
    QString codigo;
    QString nombre;
    QString producto;
    QString marca;
    double precio;
    int cantidad;
};

BridgeClass::BridgeClass(QObject *parent) : QObject(parent) {
    records.push_back({ "64528587", "Carla", "Reloj de", "Casio", 12.1, 3 });
    records.push_back({ "75893210", "Luis", "Teléfono", "Samsung", 550.5, 5 });
    records.push_back({ "47859603", "Jorge", "Teclado", "Logitech", 45.3, 10 });
    records.push_back({ "23984756", "Sofia", "Audífonos", "Sony", 80.7, 7 });
    records.push_back({ "58749213", "Juan", "Cámara", "Nikon", 400.8, 3 });

}

QStringList BridgeClass::getRecords() {
    QString codigo = "codigo";
    QString nombre = "nombre";
    QString producto = "produto";
    QString marca = "marca";
    QString precio = "precio";
    QString cantidad = "cantidad";
    QStringList result;

    result.append(codigo + "\t" + nombre + "\t" + producto + "\t" + marca + "\t" +precio + "\t" +cantidad);
    for (const record &rec : records) {
        result.append(rec.codigo + "\t" + rec.nombre + "\t" + rec.producto + "\t" + rec.marca + "\t" + QString::number(rec.precio) + "\t" + QString::number(rec.cantidad));
    }
    return result;
}

//BridgeClass::BridgeClass(QObject *parent) : QObject(parent) {
//}

void BridgeClass::runQuery(const QString& query)
{

    qDebug()<<"Antes: "<<query;
    std::string m_query = query.toStdString();

    m_query.erase(std::remove(m_query.begin(), m_query.end(), '\\'), m_query.end());

    qDebug() <<"Query: "<< m_query;
    vector<string> querys = input(m_query);
    for(int i=0;i<querys.size();i++){
        Scanner scanner(querys[i]);
        Parser parser(&scanner);
        parser.parse();
        cout<<"Error: "<<error_message<<endl;
        this->method_global=method;
    }

    emit dataChanged();
}


QStringList BridgeClass::getThreeLines(const QString& filename) {
    QStringList result;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file for reading";
        return result;
    }

    QTextStream in(&file);
    int count = 0;
    static QRegularExpression regex("[,;]");
    while (!in.atEnd() && count < 3) {
        QString line = in.readLine();
        qDebug() << "Original Line:" << line;  // Aquí agregamos un debug para ver la línea original
        QStringList fields = line.split(regex);
        QString processedLine = fields.join("\t");
        qDebug() << "Processed Line:" << processedLine;  // Aquí vemos la línea procesada
        result.append(processedLine);
        count++;
    }

    file.close();
    return result;
}

std::vector<std::string> BridgeClass::input(const string ingreso){
    vector<string> vec;
    string valor = "";
    for (int i = 0; i < ingreso.size(); i++) {
        if (ingreso[i] == ';') {
            valor+=ingreso[i];
            valor+=' ';
            // cout << valor << endl;
            vec.push_back(valor);
            valor = "";
        }
        else{
            valor+=ingreso[i];
        }
    }
    if (valor != "") {
        valor+=' ';
        vec.push_back(valor);
    }
    return vec;
};

bool BridgeClass::verifyLogin(const QString& username, const QString& password) {
    QFile file("/home/sebastian/Documents/utec/Ciclo6/BD2/Proyecto1-BD2/login.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << "Leído del archivo:" << line; // Agregar esta línea
        QStringList parts = line.split(';');
        if(parts.count() == 2 && parts[0].trimmed() == username && parts[1].trimmed() == password) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}
