#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "database.h"
#include "ipstack.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    qmlRegisterSingletonType<Database>("Database",0,1,"Database", Database::singletonProvider);
    qmlRegisterSingletonType<Utils>("Utils",0,1,"Utils", Utils::singletonProvider);
    qmlRegisterSingletonType<IpStack>("IpStack",0,1,"IpStack", IpStack::singletonProvider);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/test-project/qml/Main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
