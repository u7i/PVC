//
// Created by u7i on 2/12/21.
//

#ifndef PVC_UTILS_H
#define PVC_UTILS_H

#include <QObject>
#include <QString>

#include <QApplication>
#include <QStyle>
#include <QStyleFactory>

#include <QPixmap>

#include <QFontMetrics>

namespace PVC::App {
    /**
     * \brief Utils functions that used in qml application module ( core module doesn't depend on this class )
     */
    class Utils : public QObject {
        Q_OBJECT
    public:
        /**
         * \brief Exposing of the system standard icons to qml
         * @param w Target width of the pixmap
         * @param h Target height of the pixmap
         */
        static Q_INVOKABLE QPixmap getStandardIconAsPixmap(int pixmapId, int w, int h){
            QStyle *style = QApplication::style();

            if (!style) {
                qDebug() << "Utils: QApplication::style() is invalid.";

                QImage img(w, h, QImage::Format_RGB16);
                for (int x = 0; x < w; ++x){
                    for (int y = 0; y < h; ++y){
                        img.setPixelColor(x, y, Qt::gray);
                    }
                }
                return QPixmap::fromImage(img);
            }

            return style->standardIcon(static_cast<QStyle::StandardPixmap>(pixmapId)).pixmap(w, h);
        }

        /**
         * \brief Calculates label width with designated text and font
         * @param string Label text
         * @param font Label font
         * @param margin Additional space, by default 0px
         *
         * @return With of the label ( in px )
         */
        static Q_INVOKABLE int getLabelWidth(const QString &string, const QFont &font, int margin = 0) {
            QFontMetrics metrics(font);
            return metrics.horizontalAdvance(string) + margin;
        }
    };
}

#endif //PVC_UTILS_H
