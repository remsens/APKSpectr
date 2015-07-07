#include <QGuiApplication>
#include <QApplication>
#include <QString>
#include "qapkengine.h"



using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApkEngine engine;
#ifdef QT_DEBUG
    QString qml_path = "../../";
#else
    QString qml_path = "./";
#endif

    engine.load(qml_path + "MainWin.qml");
    return app.exec();
}

