#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "datamodel.h"
#include "datalist.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<DataModel>("Data", 1, 0, "DataModel");
    qmlRegisterUncreatableType<DataList>("Data", 1, 0, "dataList", QStringLiteral("CANT CREATE TODO IN QML"));

    DataList dataList;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("dataList"), &dataList);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
