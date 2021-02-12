//
// Created by u7i on 2/9/21.
//

#ifndef PVC_CONTROLLERLOGIC_H
#define PVC_CONTROLLERLOGIC_H

#include "../Core/Player/PlayerManager.h"

namespace PVC::App {
    /**
     * \brief "Action" button states
     */
    namespace ActionButtonState{
        enum Value {
            DoPlay = 0, ///< Current player is currently in pause state. Play it after button will be clicked
            DoPause = 1, ///< Current player is currently playing. Pause it onclick
            None = -1 ///< There are no current player
        };
    }

    /**
     * \brief Base logic for player controller application
     * \author u7i
     * \version 0.1
     *
     * Also can work as the backend of the qml widget
     */
    class ControllerLogic : public QObject {
        Q_OBJECT
    public:
        ControllerLogic(bool lateInit = false);
        ~ControllerLogic() override = default;

        /**
         * \brief Late init support
         */
        void init();

        /**
         * \brief Return nullptr if there are no players in system
         * @return Player. <b> Warning, can return nullptr </b>
         */
        [[nodiscard]] PVC::Player *getCurrentPlayer() const;

        /**
         * \brief Get current player song title
         * If current player is none returns NonePlayerSongTitle string ( Constants.h )
         */
        [[nodiscard]] QString getCurrentPlayerSongTitle() const;

        /**
         * \brief Get current player user-friendly name
         * If current player is none returns NonePlayerName string ( Constants.h )
         */
        [[nodiscard]] QString getCurrentPlayerName() const;

        /**
         * \brief Get action button state
         * @return Return state of play button
         */
        [[nodiscard]] ActionButtonState::Value getActionButtonState() const;

        /**
         * \brief Get enable state of the next button
         * @return Is the next button enabled
         */
        [[nodiscard]] bool isNextButtonEnabled() const;

        /**
         * \brief Get the longest ( at the current moment of the time ) title from the available players list
         * @return The longest available player song title
         */
        [[nodiscard]] QString getAvailablePlayersLongestSongTitle() const;

        /**
         * \brief Get the longest ( at the current moment of the time ) player name from the available players list
         * @return The longest player name ( from the available players list )
         */
        [[nodiscard]] QString getAvailablePlayersLongestPlayerName() const;
    signals:
        /**
         * \brief Signal that fire when current player changed
         */
        void onCurrentPlayerUpdated();

    public slots:
        /**
         * \brief Pause/Play current player
         */
        void actionButtonClicked();

        /**
         * \brief Select next available player
         */
        void nextButtonClicked();

        /**
         * \brief Change current player to none
         */
        void selectNonePlayer();

    private slots:
        void onChangedSlot(Player *player, PlayerChangeType::Value type);

    protected:
        PlayerManager *manager = nullptr;

        /** Belongs to manager->availablePlayers list */
        int currentIndex = -1;
        QString currentServiceName = "";

        // Utils
        [[nodiscard]] Player *getAvailablePlayerByIndex(int index) const;
        void selectPlayerFromAvailable();
    };
}

#endif //PVC_CONTROLLERLOGIC_H
