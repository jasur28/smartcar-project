#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "BluetoothManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;



    BluetoothManager btManager;
    engine.rootContext()->setContextProperty("btManager", &btManager);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SmartCar", "Main");

    return app.exec();
}

