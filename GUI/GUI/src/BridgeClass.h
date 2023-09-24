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

class BridgeClass : public QObject
{
    Q_OBJECT
public:
    explicit BridgeClass(QObject *parent = nullptr);
    Q_INVOKABLE QStringList getThreeLines(const QString& filename);
signals:
    void dataChanged();

public slots:
    void methodChosen(const QString& method);
    void runQuery(const QString& query);

private:
    QString m_method;
    QString m_query;
    QVector<QVector<QString>> readFirstThreeLines(const QString& filename);
};

#endif // BRIDGECLASS_H
