//
// Created by u7i on 1/28/21.
//

#ifndef PVC_CONSTANTS_H
#define PVC_CONSTANTS_H

#include <QString>

/**
 * \brief All global constants that used in the project. Contains DBus interfaces, none player title/name
 */
namespace PVC::Constants{
    inline QString MPRIS_Path = "/org/mpris/MediaPlayer2";
    inline QString MPRIS_Service = "org.mpris.MediaPlayer2";
    inline QString MPRIS_PlayerInterface = "org.mpris.MediaPlayer2.Player";

    inline QString DBus_Service = "org.freedesktop.DBus";
    inline QString DBus_Interface = "org.freedesktop.DBus";
    inline QString DBus_PropertiesInterface = "org.freedesktop.DBus.Properties";
    inline QString DBus_Path = "/org/freedesktop/DBus";

    inline QString NonePlayerSongTitle = "No media was found";
    inline QString NonePlayerName = "PVC";
}

#endif //PVC_CONSTANTS_H