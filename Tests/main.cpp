#include <QApplication>
#include <QPushButton>

#include <QQuickView>
#include <QQmlEngine>

#include "../Src/Widgets/Standalone.h"
#include <iostream>

int main(int argc, char *argv[]) {
    // Setup flags
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    /*QQuickView View;
    View.setSource(QUrl("qrc:/Test.qml"));
    View.show(); */

    //Standalone *standalone = new Standalone();
    //standalone->show();

    auto *s = new PVC::App::Standalone();
    s->show();

    return QApplication::exec();
}
