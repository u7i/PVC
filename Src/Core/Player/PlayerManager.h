//
// Created by u7i on 2/4/21.
//

#ifndef PVC_PLAYERMANAGER_H
#define PVC_PLAYERMANAGER_H

#include <QObject>
#include <QtDBus>

#include "Player.h"

namespace PVC{
    /**
     * \brief Namespace that encapsulate player change type. Used in the PlayerManager signal
     */
    namespace PlayerChangeType{
        /**
         * \brief Types of player change
         */
        enum Enum{
            Added = 0, ///< Player that was discovered by the manager
            Removed = 1, ///< Player was destroyed
            AddedToAvailable = 2, ///< Player added to available players list
            RemovedFromAvailable = 3, ///< Player was removed from available players list
            Changed = 4 ///< Player changed, but there are no changes in lists
        };

        /**
         * \brief PlayerChangeType structure
         * Contains type of change and isAvailableListChanged flag
         */
        struct Value{
            Enum type;
            bool isAvailableListChanged = false;

            Value(Enum t, bool isAvailableListChanged){
                this->type = t;
                this->isAvailableListChanged = isAvailableListChanged;
            }

            operator Enum() const{
                return this->type;
            }
        };
    }

    /**
     * \brief Widget that provides API for simple access to all MPRIS players in the system
     * \author u7i
     * \version 0.1
     */
    class PlayerManager : public QObject {
        Q_OBJECT

        QList<Player *> allPlayers;
        QList<Player *> availablePlayers;

        bool bIsInited = false;
    public:
        PlayerManager(bool init = true);
        ~PlayerManager() override = default;

        /**
         * \brief Init or reInit manager
         * \return Init result. Equal to isInited()
         */
        bool init();

        /**
         * \brief Checks does manager was inited successful
         * @return
         */
        [[nodiscard]] bool isInited() const;

        // Getters
        /**
         * \brief Get all players that presented in the manager
         * @return the list of players
         */
        [[nodiscard]] const QList<Player *> &getAllPlayers() const;

        /**
         * \brief Get all available to control players
         * @return the list of players
         */
        [[nodiscard]] const QList<Player *> &getAvailablePlayers() const;

        /**
        * \brief Get amount of players
        * \param onlyAvailable Return amount of controllable players
        */
        [[nodiscard]] int getPlayersAmount(bool onlyAvailable) const;
    signals:
        /**
         * \brief Signal that called when one of the players was changed
         *
         * @param player Pointer to player, <b> can be nullptr </b>
         * @param isAvailableListChanged Does available player list changed
         * @param type Type of the changing
         */
        void onPlayerChanged(PVC::Player *player, PlayerChangeType::Value type);

    private slots:
        void onPlayerChangedSlot(PVC::Player &player);
        void onDBusNameOwnerChanged(QString name, QString oldOwner, QString newOwner);

    protected:
        // DBus
        QDBusConnection *bus = nullptr;

        // Try to parse new MPRIS Player from DBus service
        void addNewPlayer(const QString &service);
        void removePlayer(int index, bool checkAvailableList);

        static bool isPlayerAvailable(const PlayerData &playerData);
    };
}

#endif //PVC_PLAYERMANAGER_H
