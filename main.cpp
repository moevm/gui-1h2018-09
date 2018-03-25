#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "todomodel.h"
#include "todolist.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<ToDoModel>("ToDo", 1, 0, "ToDoModel");
    qmlRegisterUncreatableType<ToDoList>("ToDo", 1, 0, "toDoList", QStringLiteral("CANT CREATE TODO IN QML"));

    ToDoList toDoList;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("toDoList"), &toDoList);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
