//
// Created by u7i on 2/12/21.
//

#ifndef PVC_QMLPIXMAPVIEWER_H
#define PVC_QMLPIXMAPVIEWER_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QPixmap>

namespace PVC::Qml{
    /**
     * \brief Pixmap drawer for qml
     *
     * \author u7i
     * \version 0.1
     */
    class QmlPixmapViewer : public QQuickPaintedItem {
        Q_OBJECT

        QPixmap m_pixmap;
        QImage m_disabled;

        void genDisabledImage(){
            // Generate grayscale
            this->m_disabled = this->m_pixmap.toImage();

            QColor c;
            int gray;

            for (int x = 0; x < this->m_disabled.width(); ++x) {
                for (int y = 0; y < this->m_disabled.height(); ++y) {
                    c = this->m_disabled.pixelColor(x, y);

                    //gray = (c.red() * 0.3 + c.green() * 0.59 + c.blue() * 0.11);
                    gray = (c.red() + c.green() + c.blue()) / 4.5; // Result approx equal to Qt implementation

                    this->m_disabled.setPixelColor(x, y, QColor(gray, gray, gray, c.alpha()));
                }
            }
        }
    public:
        Q_PROPERTY(QPixmap pixmap MEMBER m_pixmap WRITE setPixmap)
        Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)

        QmlPixmapViewer() = default;

        virtual void paint(QPainter *painter) override{
            if (this->isEnabled()) painter->drawPixmap(0, 0, this->m_pixmap);
            else painter->drawImage(0, 0, this->m_disabled);
        }

        // API:
        /**
         * \brief Set new pixmap
         * @param pixmap New pixmap
         */
        void setPixmap(const QPixmap &pixmap){
            this->m_pixmap = pixmap;
            this->genDisabledImage();
        }

        /**
         * \brief Get current pixmap
         */
        QPixmap getPixmap() const{
            return this->m_pixmap;
        }

        /**
         * \brief Change enable state and force update widget
         */
        void setEnabled(bool isEnabled){
            QQuickPaintedItem::setEnabled(isEnabled);
            this->update();
        }
    };
}

#endif //PVC_QMLPIXMAPVIEWER_H
