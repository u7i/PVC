//
// Created by u7i on 2/13/21.
//

#ifndef PVC_QMLSTYLE_H
#define PVC_QMLSTYLE_H

#include <QApplication>
#include <QStyle>
#include <QPixmap>

namespace PVC::Qml{
    /**
     * \brief Class that exposing StandardPixmap enum to qml
     * \author u7i
     * \version 0.1
     */
    class QmlStyle : public QStyle{
    public:
        // Register pixmap
        Q_ENUM_NS(StandardPixmap)
    };
}

#endif //PVC_QMLSTYLE_H