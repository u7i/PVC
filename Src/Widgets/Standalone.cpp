//
// Created by u7i on 1/26/21.
//

#include "Standalone.h"

void PVC::App::Standalone::createUI() {
    this->mainLayout = new QHBoxLayout();

    this->labelLayout = new QVBoxLayout;
    this->buttonLayout = new QHBoxLayout();

    // Setup labels
    this->songTitleLabel = new QLabel();
    this->songTitleLabel->setAlignment(Qt::AlignmentFlag::AlignRight);

    this->playerNameLabel = new QLabel();
    this->playerNameLabel->setStyleSheet("font-weight: bold;");
    this->playerNameLabel->setAlignment(Qt::AlignmentFlag::AlignRight);

    // Setup buttons
    this->actionButton = new QPushButton();
    this->nextButton = new QPushButton();

    // Setup pause/play button
    this->actionButton->setFixedSize(32, 32);

    // Setup next Player button
    this->nextButton->setFixedSize(16, 32);

    // Icon won't change during runtime
    this->nextButton->setIcon(this->style()->standardIcon(QStyle::SP_ArrowRight));

    // Setup label layout
    this->labelLayout->addWidget(this->songTitleLabel);
    this->labelLayout->addWidget(this->playerNameLabel);

    this->labelLayout->setSpacing(0);

    // Set button layout
    this->buttonLayout->addWidget(this->actionButton);
    this->buttonLayout->addWidget(this->nextButton);

    this->buttonLayout->setSpacing(0);

    // Setup main layout
    this->mainLayout->addLayout(this->labelLayout);
    this->mainLayout->addLayout(this->buttonLayout);

    this->setLayout(this->mainLayout);
}

PVC::App::Standalone::Standalone(QWidget *parent) {
    this->setParent(parent);
    this->createUI();

    // Create metrics
    this->metrics = new QFontMetrics(this->songTitleLabel->font());

    // Setup logic
    this->logic = new ControllerLogic(true);

    // Connect slots
    connect(this->logic, &ControllerLogic::currentPlayerUpdated, this, &Standalone::updateWidget);

    connect(this->actionButton, &QPushButton::clicked, this->logic, &ControllerLogic::actionButtonClicked);
    connect(this->nextButton, &QPushButton::clicked, this->logic, &ControllerLogic::nextButtonClicked);

    // Init logic
    this->logic->init();

    // Setup label width
    QString str = this->logic->getLongestStringFromAvailable();
    this->songTitleLabel->setMinimumWidth(this->metrics->horizontalAdvance(str) + this->LabelMargins);
}
PVC::App::Standalone::~Standalone() = default;

void PVC::App::Standalone::updateWidget() {
    // Update labels
    this->songTitleLabel->setText(this->logic->getCurrentPlayerSongTitle());

    // Update size of label ( if needed )
    int nWidth = this->metrics->horizontalAdvance(this->songTitleLabel->text()) + this->LabelMargins;
    if (this->songTitleLabel->width() < nWidth){
        qDebug() << "Standalone: songTitleLabel size updated";
        this->songTitleLabel->setFixedWidth(nWidth);
    }

    this->playerNameLabel->setText(this->logic->getCurrentPlayerName());

    // Update buttons
    // Action button
    auto updateActionButton = [&](const QIcon &icon, bool enabled){
        this->actionButton->setIcon(icon);
        this->actionButton->setEnabled(enabled);
    };

    switch (this->logic->getActionButtonState()) {
        case ActionButtonState::DoPlay:
            updateActionButton(this->style()->standardIcon(QStyle::SP_MediaPlay), true);
            break;

        case ActionButtonState::DoPause:
            updateActionButton(this->style()->standardIcon(QStyle::SP_MediaPause), true);
            break;

        case ActionButtonState::None:
            updateActionButton(this->style()->standardIcon(QStyle::SP_MediaPlay), false);
            break;
    }

    // Next button
    this->nextButton->setEnabled(this->logic->isNextButtonEnabled());
}
