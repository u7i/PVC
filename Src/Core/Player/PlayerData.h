//
// Created by u7i on 2/7/21.
//

#ifndef PVC_PLAYERDATA_H
#define PVC_PLAYERDATA_H

#include "../DBusUtils.h"

namespace PVC{
    /**
     * \brief Namespace that encapsulating PlaybackStatus enum
     */
    namespace PlaybackStatus {
        /**
         * \brief Player playback statuses
         */
        enum Value {
            Playing = 0,
            Paused = 1,
            Stopped = 2,
            None = -1
        };
    }

    /**
     * \brief MPRIS player properties
     */
    struct PlayerData{
        // Flags
        bool bCanPlay = true;
        bool bCanPause = true;

        QString serviceName;
        QString playerName;

        QString title;
        QString artist;

        QUrl URL;

        PlaybackStatus::Value status = PlaybackStatus::None;
        //QIcon Icon; TODO: Add icon support

        PlayerData() = default;
        PlayerData(const QString &serviceName){
            this->serviceName = serviceName;
        }
        PlayerData(bool canPlay, bool canPause,
                   const QString &serviceName, const QString &playerName,
                   const QString &title, const QString &artist, const QUrl &URL,
                   PlaybackStatus::Value status){

            this->bCanPlay = canPlay;
            this->bCanPause = canPause;

            this->serviceName = serviceName;
            this->playerName = playerName;

            this->title = title;
            this->artist = artist;

            this->URL = URL;

            this->status = status;
        }

        // API:
        /**
         * \brief Check does player is active ( PlaybackStatus == Playing | Paused )
         */
        bool isActive() const{
            return this->status != PVC::PlaybackStatus::Stopped && this->status != PVC::PlaybackStatus::None;
        }

        // Operators
        bool operator== (const PlayerData &rhs) const{
            return rhs.serviceName == this->serviceName;
        }
        bool operator== (const PlayerData *rhs) const{
            return rhs && *this == *rhs;
        }

        bool operator!= (const PlayerData &rhs) const{
            return rhs.serviceName != this->serviceName;
        }
        bool operator!= (const PlayerData *rhs) const{
            return rhs && *this != *rhs;
        }
    };
}

#endif //PVC_PLAYERDATA_H
