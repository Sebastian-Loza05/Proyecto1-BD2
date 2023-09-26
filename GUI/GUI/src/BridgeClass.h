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
    Q_INVOKABLE bool verifyLogin(const QString& username, const QString& password);
    Q_INVOKABLE QString getErrorMessage() const;
    Q_INVOKABLE QVariantList readRecords() const;
signals:
    void errorOccurred();
    void dataChanged();

public slots:
    void runQuery(const QString& query);
    std::vector<std::string> input(const string ingreso);


private:
    QString m_method;
    QString m_query;
    MethodSelector<Record>* method_global;
    string global_error_message;
    vector<Record> global_records1;
    vector<Record2> global_records2;
    pair<string,string> tabla_global;
};




#endif // BRIDGECLASS_H
