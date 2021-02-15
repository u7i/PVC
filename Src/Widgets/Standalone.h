//
// Created by u7i on 1/26/21.
//

#ifndef PVC_STANDALONE_H
#define PVC_STANDALONE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStyle>

#include "../Application/ControllerLogic.h"

#include <QDebug>

namespace PVC::App {
    /**
     * \brief Widget that allows to control all media players in system
     * \author u7i
     * \version 0.1
     */
    class Standalone : public QWidget {
        Q_OBJECT

        const int LabelMargins = 30;
        void createUI();

    public:
        explicit Standalone(QWidget *parent = nullptr);
        ~Standalone() override;

    private slots:
        /**
         * \brief Connected to logic->currentPlayerUpdated
         */
        void updateWidget();

    protected:
        ControllerLogic *logic;
        QFontMetrics *metrics;

        // Widgets
        QHBoxLayout *mainLayout;

        QVBoxLayout *labelLayout;
        QHBoxLayout *buttonLayout;

        QLabel *songTitleLabel;
        QLabel *playerNameLabel;

        QPushButton *actionButton;
        QPushButton *nextButton;
    };
}

#endif //PVC_STANDALONE_H