//
// Created by u7i on 2/4/21.
//

#include "PlayerManager.h"

PVC::PlayerManager::PlayerManager(bool init){
    if (init) this->init();
}

bool PVC::PlayerManager::init() {
    // Clean-up
    this->allPlayers.clear();

    // Init DBus
    this->bus = new QDBusConnection(QDBusConnection::sessionBus());
    if (!this->bus->isConnected()) {
        qDebug() << "PlayerManager: Cannot connect to the DBus session bus";
        return (this->bIsInited = false);
    }

    // Get list of players
    QList<QString> servicesList = PVC::DBusUtils::getObjectsByServiceName(PVC::Constants::MPRIS_Service, *this->bus);
    for (const QString &service : servicesList) {
        this->addNewPlayer(service);
    }

    // Bind service owner change ( creating or deleting of the service )
    this->bus->connect(PVC::Constants::DBus_Service, PVC::Constants::DBus_Path,
                      PVC::Constants::DBus_Interface, "NameOwnerChanged",
                      this, SLOT(onDBusNameOwnerChanged(QString, QString, QString)));

    // Debug messages
    qDebug() << "PlayerManager: Found" << this->allPlayers.size() << "players";
    qDebug() << "PlayerManager: Possible to control" << this->availablePlayers.size() << "players";

    return (this->bIsInited = true);
}
bool PVC::PlayerManager::isInited() const {
    return this->bIsInited;
}

const QList<PVC::Player *> &PVC::PlayerManager::getAllPlayers() const {
    return this->allPlayers;
}
const QList<PVC::Player *> &PVC::PlayerManager::getAvailablePlayers() const {
    return this->availablePlayers;
}

int PVC::PlayerManager::getPlayersAmount(bool onlyAvailable) const {
    return onlyAvailable ? this->availablePlayers.size() : this->allPlayers.size();
}

// Private slots
void PVC::PlayerManager::onDBusNameOwnerChanged(QString name, QString oldOwner, QString newOwner) {
    if (name.contains(PVC::Constants::MPRIS_Service)){
        int i = -1;
        for (int c = 0; c < this->allPlayers.size(); ++c) {
            if (this->allPlayers[c]->getData().serviceName == name) {
                i = c;
                break;
            }
        }

        // Try to add new player
        if (i == -1 && !newOwner.isEmpty()) {
            qDebug() << "PlayerManager: Found new MPRIS service" << name << "NewOwner" << newOwner << "OldOwner" << oldOwner;
            this->addNewPlayer(name);
        } else if (i != -1) {
            // Remove
            this->removePlayer(i, false);
        }
    }
}
void PVC::PlayerManager::onPlayerChangedSlot(PVC::Player &player) {
    int i = -1;
    for (int c = 0; c < this->availablePlayers.size(); ++c){
        if (this->availablePlayers[c]->getData() == player.getData()){
            i = c;
            break;
        }
    }

    // Send debug message
    qDebug() << "PlayerManager: Player changed" << player.getData().serviceName;

    // Do action, and call signal after it
    if (i == -1 && isPlayerAvailable(player.getData())){
        this->availablePlayers.append(&player);
        this->onPlayerChanged(&player,PlayerChangeType::Value(PlayerChangeType::AddedToAvailable,
                                                      true));
    }
    else if (i != -1 && !isPlayerAvailable(player.getData())){
        this->availablePlayers.erase(this->availablePlayers.begin() + i);
        this->onPlayerChanged(&player,PlayerChangeType::Value(PlayerChangeType::RemovedFromAvailable,
                                                      true));
    }else{
        this->onPlayerChanged(&player,PlayerChangeType::Value(PlayerChangeType::Changed,
                                                              false));
    }
}

// Utils
void PVC::PlayerManager::addNewPlayer(const QString &service) {
    PVC::Player *player = new PVC::Player(service, *this->bus);

    // Check for status
    if (player->getData().status == PVC::PlaybackStatus::None){
        qDebug() << "PlayerManager: Invalid player found" << player->getData().serviceName;
        return;
    }

    this->allPlayers.append(player);

    // Add to available list
    bool isAvailable = isPlayerAvailable(player->getData());
    if (isAvailable) {
        this->availablePlayers.append(player);
    }

    // Connect player slots
    connect(player, &Player::onChanged,this, &PlayerManager::onPlayerChangedSlot);

    // Send signal
    this->onPlayerChanged(player,PlayerChangeType::Value(PlayerChangeType::Added, isAvailable));
}
void PVC::PlayerManager::removePlayer(int index, bool checkAvailableList) {
    // Remove from general players list
    QString serviceName = this->allPlayers[index]->getData().serviceName;
    this->allPlayers.erase(this->allPlayers.begin() + index);

    // Check available players list
    bool wasAvailable = false;
    if (checkAvailableList) {
        for (int c = 0; c < this->availablePlayers.size(); ++c) {
            if (this->availablePlayers[c]->getData().serviceName == serviceName) {
                this->availablePlayers.erase(this->availablePlayers.begin() + c);

                wasAvailable = true;
                break;
            }
        }
    }

    // Info msg
    qDebug() << "PlayerManager:" << (wasAvailable ? "Player destroyed and was removed from available list" : "Player destroyed")
             << serviceName;

    // Send signal
    this->onPlayerChanged(nullptr, PlayerChangeType::Value(PlayerChangeType::Removed, wasAvailable));
}

bool PVC::PlayerManager::isPlayerAvailable(const PVC::PlayerData &playerData) {
    return playerData.isActive() && !playerData.title.isEmpty();
}