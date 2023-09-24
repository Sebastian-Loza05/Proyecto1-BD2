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
