//
// Created by u7i on 1/29/21.
//

#ifndef PVC_PVC_PLUGIN_H
#define PVC_PVC_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <QtQml>
#include <Widgets/Standalone.h>

/**
 * \brief QML plugin description
 * \version 0.1
 */
class PVC_Plugin : public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri) override{
        assert(uri == QLatin1String("org.u7i.PVC")); // Check for correct uri

        // Register cpp types ( expose it to qml )
        //qmlRegisterType<Standalone>(uri, 0, 1, "PlaybackWidget"); TODO: Register logic type
    }
};

#endif //PVC_PVC_PLUGIN_H
