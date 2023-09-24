#include "BridgeClass.h"

using namespace std;

BridgeClass::BridgeClass(QObject *parent) : QObject(parent) {
}

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


