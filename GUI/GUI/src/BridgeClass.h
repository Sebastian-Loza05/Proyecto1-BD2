#ifndef BRIDGECLASS_H
#define BRIDGECLASS_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QVariant>
#include <QRegularExpression>
#include "../../../tokenSQL.h"
#include "../../../parserSQL.h"
#include "../../../Structures/methods.h"

using namespace std;

class BridgeClass : public QObject
{
    Q_OBJECT

public:
    explicit BridgeClass(QObject *parent = nullptr);
    Q_INVOKABLE QStringList getThreeLines(const QString& filename);
signals:
    void dataChanged();

public slots:
    void runQuery(const QString& query);
    std::vector<std::string> input(const string ingreso);

private:
    QString m_method;
    QString m_query;
    QVector<QVector<QString>> readFirstThreeLines(const QString& filename);
    MethodSelector* method_global;
};




#endif // BRIDGECLASS_H
