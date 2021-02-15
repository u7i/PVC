//
// Created by u7i on 1/29/21.
//

#ifndef PVC_PVC_PLUGIN_H
#define PVC_PVC_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <QtQml>

#include "./CustomModules/Utils.h"
#include "../Application/ControllerLogic.h"

#include "./CustomModules/QmlStyle.h"
#include "./CustomModules/QmlPixmapViewer.h"

namespace PVC::App {
    /**
     * \brief QML plugin description
     * \version 0.1
     */
    class PVC_Plugin : public QQmlExtensionPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
    public:
        void registerTypes(const char *uri) override {
            assert(uri == QLatin1String("org.u7i.PVC")); // Check for correct uri

            // Register cpp types ( = expose it to qml )
            qmlRegisterType<ControllerLogic>(uri, 0, 1, "ControllerLogic");
            qmlRegisterType<Utils>(uri, 0, 1, "ControllerUtils");

            // QmlPixmap system
            qmlRegisterType<Qml::QmlPixmapViewer>(uri, 0, 1, "PixmapViewer");

            // Expose enums
            qmlRegisterUncreatableMetaObject(
                    App::ActionButtonState::staticMetaObject,
                    uri, 0, 1,
                    "ActionButtonState",
                    "Error: can't create namespace holder");

            // Expose qt types to qml
            qmlRegisterUncreatableMetaObject(
                    Qml::QmlStyle::staticMetaObject,
                    uri,0, 1,
                    "Style",
                    "Error: can't create virtual class" // On create error
            );

            qmlRegisterUncreatableMetaObject(
                    Qml::QmlStyle::staticMetaObject,
                    uri,0, 1,
                    "Style",
                    "Error: can't create virtual class" // On create error
            );
        }
    };
}

#endif //PVC_PVC_PLUGIN_H
