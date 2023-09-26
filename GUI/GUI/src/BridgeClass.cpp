#include "BridgeClass.h"
using namespace std;



BridgeClass::BridgeClass(QObject *parent) : QObject(parent) {
}

void BridgeClass::runQuery(const QString& query)
{
    this->global_error_message = "";
    if (query.isEmpty()) {
        error_message = "Sintaxis Incorrecta :(";
        this->global_error_message = error_message;
        qDebug()<<global_error_message;
        return;
    } else {
        qDebug()<<"Antes: "<<query;
        std::string m_query = query.toStdString();

        m_query.erase(std::remove(m_query.begin(), m_query.end(), '\\'), m_query.end());

        qDebug() <<"Query: "<< m_query;
        vector<string> querys = input(m_query);
        for(int i=0;i<querys.size();i++){
            Scanner scanner(querys[i]);
            Parser parser(&scanner);
            parser.parse();
            this->global_records1=records;
            this->global_records2=records1;
            this->tabla_global=tabla;
            qDebug()<<tabla_global.first;
            qDebug()<<tabla_global.second;
            this->global_error_message = error_message;
            qDebug()<<global_error_message;
            this->method_global=method;

        }
        if(records.empty()) {

        }
        readRecords();
        emit dataChanged();
    }

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
    QFile file("/home/luisd/UTEC/ciclo_6/BDII/proyecto/Proyecto1-BD2/login.txt");
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

QString BridgeClass::getErrorMessage() const {
    return QString::fromStdString(global_error_message);
}

// QString BridgeClass::readRecords() const {
//     QString result;
//
//     const int nombreMaxLen = 15; // Define una longitud máxima para cada campo
//     const int productoMaxLen = 15;
//     const int marcaMaxLen = 15;
//     const int profesionMaxLen =15;
//     const int generoMaxLen = 2;
//     // Creación del header con la misma justificación
//     if(this->tabla_global.second=="record1"){
//         qDebug()<<"IF";
//         QString header = QString("CODIGO\t") +
//                          QString("NOMBRE").leftJustified(nombreMaxLen, ' ') + "\t" +
//                          QString("PRODUCTO").leftJustified(productoMaxLen, ' ') + "\t" +
//                          QString("MARCA").leftJustified(marcaMaxLen, ' ') + "\t" +
//                          QString("PRECIO\t") +
//                          QString("CANTIDAD\n");
//
//         // Añadir el header al resultado
//         result += header;
//
//         for (const Record &rec : global_records1) {
//             qDebug()<<"Leyendo records";
//             QString nombrePadded = QString(rec.nombre).leftJustified(nombreMaxLen, ' '); // Rellena con espacios a la derecha
//             QString productoPadded = QString(rec.producto).leftJustified(productoMaxLen, ' ');
//             QString marcaPadded = QString(rec.marca).leftJustified(marcaMaxLen, ' ');
//
//             result += QString::number(rec.key) + "\t" +
//                       nombrePadded + "\t" +
//                       productoPadded + "\t" +
//                       marcaPadded + "\t" +
//                       QString::number(rec.precio) + "\t" +
//                       QString::number(rec.cantidad) + "\n";
//         }
//     }else{
//         qDebug()<<"ELSE";
//         QString header = QString("CODIGO\t") +
//                          QString("NOMBRE").leftJustified(nombreMaxLen, ' ') + "\t" +
//                          QString("GENERO").leftJustified(productoMaxLen, ' ') + "\t" +
//                          QString("PROFESION").leftJustified(marcaMaxLen, ' ') + "\t" +
//                          QString("EDAD\t") +
//                          QString("SUELDO\n");
//
//         // Añadir el header al resultado
//         result += header;
//
//         for (const Record2 &rec : global_records2) {
//             qDebug()<<"Leyendo records";
//             QString nombrePadded = QString(rec.key).leftJustified(nombreMaxLen, ' '); // Rellena con espacios a la derecha
//             QString profesionPadded = QString(rec.profesion).leftJustified(profesionMaxLen, ' ');
//             QString generoPadded = QString(rec.genero).leftJustified(generoMaxLen, ' ');
//
//             result += nombrePadded + "\t" +
//                       profesionPadded + "\t" +
//                       generoPadded + "\t" +
//                       QString::number(rec.edad) + "\t" +
//                       QString::number(rec.sueldo) + "\n";
//         }
//     }
//
//
//     qDebug() << "Se recorrio los records";
//
//     return result;
// }
QVariantList BridgeClass::readRecords() const {
    QVariantList recordsList;
    QString encabezado;

    if(this->tabla_global.second == "record1") {
        encabezado  = "1";
        for (const Record &rec : global_records1) {
            QVariantMap recordMap;
            recordMap.insert("headerType", encabezado);
            recordMap.insert("codigo", rec.key);
            recordMap.insert("nombre", rec.nombre);
            recordMap.insert("producto", rec.producto);
            recordMap.insert("marca", rec.marca);
            recordMap.insert("precio", rec.precio);
            recordMap.insert("cantidad", rec.cantidad);
            recordsList.append(recordMap);
        }
    } else {
        encabezado= "2";
        for (const Record2 &rec : global_records2) {
            QVariantMap recordMap;
            recordMap.insert("headerType", encabezado);
            recordMap.insert("codigo", rec.key);
            recordMap.insert("genero", rec.genero);
            recordMap.insert("profesion", rec.profesion);
            recordMap.insert("edad", rec.edad);
            recordMap.insert("sueldo", rec.sueldo);
            recordsList.append(recordMap);
        }
    }

    return recordsList;
}
