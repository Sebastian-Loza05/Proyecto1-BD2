#include "BridgeClass.h"

BridgeClass::BridgeClass(QObject *parent) : QObject(parent) { }

void BridgeClass::methodChosen(const QString& method)
{
    m_method = method;
}

void BridgeClass::runQuery(const QString& query)
{
    m_query = query;
    qDebug() << "Metodo: " << m_method;
    qDebug() << "Consulta: " << m_query;
    emit dataChanged();
}

QVector<QVector<QString>> BridgeClass::readFirstThreeLines(const QString& filename) {
    QVector<QVector<QString>> result;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file for reading";
        return result;
    }

    QTextStream in(&file);
    int count = 0;
    while (!in.atEnd() && count < 3) {
        QString line = in.readLine().replace("\"", "").replace(";", "");

        QStringList fields = line.split(",");
        // asumiendo que ',' es el delimitador
        result.append(fields.toVector());
        count++;
    }

    file.close();
    return result;
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
