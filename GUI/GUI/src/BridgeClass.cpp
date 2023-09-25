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
            this->global_error_message = error_message;
            qDebug()<<global_error_message;
            this->method_global=method;
            this->global_records=records;
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

QString BridgeClass::readRecords() const {
    /*QString result;

    const int nombreMaxLen = 15; // Define una longitud máxima para cada campo
    const int productoMaxLen = 15;
    const int marcaMaxLen = 15;

    qDebug() << "Recorriendo Records";

    for (const Record &rec : global_records) {
        QString nombrePadded = QString(rec.nombre).leftJustified(nombreMaxLen, ' '); // Rellena con espacios a la derecha
        QString productoPadded = QString(rec.producto).leftJustified(productoMaxLen, ' ');
        QString marcaPadded = QString(rec.marca).leftJustified(marcaMaxLen, ' ');

        result += QString::number(rec.key) + "\t" +
                  nombrePadded + "\t" +
                  productoPadded + "\t" +
                  marcaPadded + "\t" +
                  QString::number(rec.precio) + "\t" +
                  QString::number(rec.cantidad) + "\n";
    }

    qDebug() << "Se recorrio los records";

    return result;
*/

    QString result;

    int maxKeyLen = 0;
    int maxNombreLen = 0;
    int maxProductoLen = 0;
    int maxMarcaLen = 0;
    int maxPrecioLen = 0;
    int maxCantidadLen = 0;

    // Encontrar longitudes máximas
    for (const Record &rec : global_records) {
        maxKeyLen = qMax(maxKeyLen, QString::number(rec.key).length());
        maxNombreLen = qMax(maxNombreLen, QString(rec.nombre).length());
        maxProductoLen = qMax(maxProductoLen, QString(rec.producto).length());
        maxMarcaLen = qMax(maxMarcaLen, QString(rec.marca).length());
        maxPrecioLen = qMax(maxPrecioLen, QString::number(rec.precio).length());
        maxCantidadLen = qMax(maxCantidadLen, QString::number(rec.cantidad).length());
    }

    // Construir la cadena con un tabulado correcto
    for (const Record &rec : global_records) {
        QString keyPadded = QString::number(rec.key).rightJustified(maxKeyLen, ' ');
        QString nombrePadded = QString(rec.nombre).leftJustified(maxNombreLen, ' ');
        QString productoPadded = QString(rec.producto).leftJustified(maxProductoLen, ' ');
        QString marcaPadded = QString(rec.marca).leftJustified(maxMarcaLen, ' ');
        QString precioPadded = QString::number(rec.precio).rightJustified(maxPrecioLen, ' ');
        QString cantidadPadded = QString::number(rec.cantidad).rightJustified(maxCantidadLen, ' ');

        result += keyPadded + "\t" +
                  nombrePadded + "\t" +
                  productoPadded + "\t" +
                  marcaPadded + "\t" +
                  precioPadded + "\t" +
                  cantidadPadded + "\n";
    }

    return result;
}

