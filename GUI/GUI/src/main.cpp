#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "BridgeClass.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    BridgeClass bridge;
    engine.rootContext()->setContextProperty("bridge", &bridge);

    engine.load(QUrl(QStringLiteral("qrc:/Main/main.qml")));

    return app.exec();
}
