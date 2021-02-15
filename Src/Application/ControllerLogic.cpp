//
// Created by u7i on 2/9/21.
//

#include "ControllerLogic.h"

PVC::App::ControllerLogic::ControllerLogic(bool lateInit) {
    if (!lateInit) this->init();
}

void PVC::App::ControllerLogic::init() {
    qDebug() << "ControllerLogic: Start initializing"; // TODO: Debug
    this->manager = new PlayerManager();
    connect(this->manager, &PlayerManager::onPlayerChanged, this, &ControllerLogic::onChangedSlot);

    // Force select player
    this->selectPlayerFromAvailable();
}

PVC::Player *PVC::App::ControllerLogic::getCurrentPlayer() const {
    if (this->currentIndex == -1) return nullptr;
    return this->getAvailablePlayerByIndex(this->currentIndex);
}
QString PVC::App::ControllerLogic::getCurrentPlayerSongTitle() const {
    if (this->currentIndex == -1) return Constants::NonePlayerSongTitle;
    return this->getAvailablePlayerByIndex(this->currentIndex)->getData().title;
}
QString PVC::App::ControllerLogic::getCurrentPlayerName() const {
    if (this->currentIndex == -1) return Constants::NonePlayerName;
    return this->getAvailablePlayerByIndex(this->currentIndex)->getData().playerName;
}

int PVC::App::ControllerLogic::getActionButtonState() const {
    Player *current = this->getCurrentPlayer();
    if (!current) return ActionButtonState::None;

    switch (this->getCurrentPlayer()->getData().status) {
        case PlaybackStatus::Playing:
            return ActionButtonState::DoPause;

        case PlaybackStatus::Paused:
            return ActionButtonState::DoPlay;

        default:
            return ActionButtonState::None;
    }
}
bool PVC::App::ControllerLogic::isNextButtonEnabled() const {
    return this->manager->getAvailablePlayers().size() > 1;
}

QString PVC::App::ControllerLogic::getAvailablePlayersLongestSongTitle() const {
    QString title;
    for (const Player *player : this->manager->getAvailablePlayers()){
        if (player->getData().title.length() > title.length()){
            title = player->getData().title;
        }
    }

    if (!title.isEmpty()) return title;
    return Constants::NonePlayerSongTitle;
}
QString PVC::App::ControllerLogic::getAvailablePlayersLongestPlayerName() const {
    QString playerName;
    for (const Player *player : this->manager->getAvailablePlayers()){
        if (player->getData().playerName.length() > playerName.length()){
            playerName = player->getData().playerName;
        }
    }

    if (!playerName.isEmpty()) return playerName;
    return Constants::NonePlayerName;
}

QString PVC::App::ControllerLogic::getLongestStringFromAvailable() const {
    QString songTitle = this->getAvailablePlayersLongestSongTitle();
    QString playerName = this->getAvailablePlayersLongestPlayerName();

    return songTitle.length() > playerName.length() ? songTitle : playerName;
}

// Public slots
void PVC::App::ControllerLogic::actionButtonClicked() {
    switch (this->getActionButtonState()) {
        case ActionButtonState::DoPlay:
            this->getCurrentPlayer()->play();
            break;

        case ActionButtonState::DoPause:
            this->getCurrentPlayer()->pause();
            break;

        case ActionButtonState::None:
            break;
    }
}
void PVC::App::ControllerLogic::nextButtonClicked() {
    // Loop impl
    if (this->currentIndex >= this->manager->getAvailablePlayers().size() - 1) {
        // Checks
        if (this->manager->getAvailablePlayers().isEmpty()) {
            this->selectNonePlayer();
            return;
        }

        this->currentIndex = 0;
    }else{
        this->currentIndex++;
    }

    this->currentServiceName = this->getAvailablePlayerByIndex(this->currentIndex)->getData().serviceName;
    this->currentPlayerUpdated();
}

void PVC::App::ControllerLogic::selectNonePlayer() {
    this->currentIndex = -1;
    this->currentServiceName = "";

    this->currentPlayerUpdated();
}

// Private slots
void PVC::App::ControllerLogic::onChangedSlot(Player *player, PlayerChangeType::Value type) {
    // list isn't broken, so let's just send update signal and do nothing more
    if (!type.isAvailableListChanged) {
        if (player && player->getData().serviceName == this->currentServiceName) {
            this->currentPlayerUpdated();
        }

        return;
    }

    for (int c = 0; c < this->manager->getAvailablePlayers().size(); ++c){
        if (this->getAvailablePlayerByIndex(c)->getData().serviceName == this->currentServiceName){
            this->currentIndex = c; // Just update index, service name is stay the same
            this->currentPlayerUpdated();
            return;
        }
    }

    // If current player is no longer available - select new
    this->selectPlayerFromAvailable();
}

PVC::Player *PVC::App::ControllerLogic::getAvailablePlayerByIndex(int index) const {
    return this->manager->getAvailablePlayers()[index];
}

void PVC::App::ControllerLogic::selectPlayerFromAvailable() {
    auto select = [&](int index, Player *player){
        this->currentIndex = index;
        this->currentServiceName = player->getData().serviceName;

        this->currentPlayerUpdated();
    };

    // Set priority to playing player, but if no players playing - we select the last
    int lowPriority = -1;

    for (int c = this->manager->getAvailablePlayers().size() - 1; c >= 0; --c) {
        Player *player = this->getAvailablePlayerByIndex(c);

        if (player->getData().status != PlaybackStatus::Stopped) {
            if (player->getData().status == PlaybackStatus::Playing) { // Immediately select player
                select(c, player);
                return;
            }

            lowPriority = c;
        }
    }

    // Try to select low-priority player
    if (lowPriority != -1){
        select(lowPriority, this->getAvailablePlayerByIndex(lowPriority));
        return;
    }

    // If we found nothing - set none player ( = set currentIndex to -1 )
    this->selectNonePlayer();
} 