#ifndef BRIDGECLASS_H
#define BRIDGECLASS_H

#include <QObject>
#include <QString>
#include <QDebug>

class BridgeClass : public QObject
{
    Q_OBJECT
public:
    explicit BridgeClass(QObject *parent = nullptr);

signals:
    void dataChanged();

public slots:
    void methodChosen(const QString& method);
    void runQuery(const QString& query);

private:
    QString m_method;
    QString m_query;
};

#endif // BRIDGECLASS_H
