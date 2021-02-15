#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include "../Src/Widgets/Standalone.h"

int main(int argc, char *argv[]) {
    // Setup flags
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);

    QQuickView view;
    view.setSource(QUrl("qrc:/Test.qml"));
    view.show();

    // UnComment tot use standalone
    //auto *standalone = new PVC::App::Standalone();
    //standalone->show();

    return QApplication::exec();
}
