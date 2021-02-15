//
// Created by u7i on 1/26/21.
//

#ifndef PVC_PLAYER_H
#define PVC_PLAYER_H

#include <QString>
#include <QFileInfo>
#include <QUrl>

#include "PlayerData.h"
#include "../DBusUtils.h"

namespace PVC{
    /**
     * \brief Short player class
     * \version 0.1
     *
     * \author u7i
     */
    class Player : public QObject {
        Q_OBJECT

        PlayerData data = PlayerData();
        QDBusConnection connection;

        QSharedPointer<QDBusInterface> resolveIFace;
        QSharedPointer<QDBusInterface> playerInterface;

        /**
         * \brief Parse MPRIS service to Player object
         * If Player was failed to create, it status will be set to NONE
         *
         * \author u7i
         *
         * @param service Name of MPRIS service ( with full path ex: org.mpris.Player2.vlc )
         * @param connection DBus connection
         * @param Output Parsed player
         */
        void parsePlayer() {
            // Reset current data
            this->data = PlayerData(this->data.serviceName);

            // Get current playback status
            QVariant V = DBusUtils::getProperty("PlaybackStatus",
                                                Constants::MPRIS_PlayerInterface,
                                                *this->resolveIFace);

            QString StatusStr = V.toString();

            if (StatusStr == "Playing") this->data.status = PlaybackStatus::Playing;
            else if (StatusStr == "Paused") this->data.status = PlaybackStatus::Paused;
            else if (StatusStr == "Stopped") this->data.status = PlaybackStatus::Stopped;
            else this->data.status = PlaybackStatus::None;

            // Parse metadata props:
            // A little bit long string - here we content QVariant -> QDBusVariant -> QDBusArgument.
            // We can't extract QDBusArgument directly from Metadata.arguments
            // Detailed here:
            // https://stackoverflow.com/questions/20206376/how-do-i-extract-the-returned-data-from-qdbusmessage-in-a-qt-dbus-call
            QDBusArgument propsArg = DBusUtils::getProperty("Metadata",
                                                            Constants::MPRIS_PlayerInterface,
                                                            *this->resolveIFace).value<QDBusArgument>();

            QMap<QString, QVariant> props;

            propsArg >> props;

            // Get title ( in most cases it's user-friendly string )
            if (props.contains("xesam:title")){
                this->data.title = props["xesam:title"].toString();
            }

            // Get artist
            if (props.contains("xesam:artist")){
                const QStringList &list = props["xesam:artist"].toStringList();

                if (!list.isEmpty()) {
                    this->data.artist = list[0];
                }
            }

            // Get filename
            if (props.contains("xesam:url")){
                this->data.URL = props["xesam:url"].toString();
            }

            if (this->data.title.isEmpty()){
                // Try to set title from filename:
                // Check does URL file valid and exists
                if (this->data.URL.isValid()) {
                    QFileInfo URLInfo(this->data.URL.toLocalFile());

                    if (URLInfo.exists() && URLInfo.isFile()){
                        this->data.title = URLInfo.fileName().split(".")[0];
                    }
                }
            }

            // Get flags
            this->data.bCanPlay = DBusUtils::getProperty("CanPlay", Constants::MPRIS_PlayerInterface,
                                                         *this->resolveIFace).toBool();

            this->data.bCanPause = DBusUtils::getProperty("CanPause", Constants::MPRIS_PlayerInterface,
                                                          *this->resolveIFace).toBool();

            // Friendly player name ( for example VLC Media Player )
            this->data.playerName = DBusUtils::getProperty("Identity", Constants::MPRIS_Service,
                                                           *this->resolveIFace).toString();
        }

    public:
        Player(const Player &player)
                     : connection(player.connection),
                     resolveIFace(new QDBusInterface(__dbus_interface_copy_params((*(player.resolveIFace)),
                                                                                  connection))),
                     playerInterface(new QDBusInterface(__dbus_interface_copy_params( (*(player.playerInterface)),
                                                                                      connection)))
        {
            this->data = player.data;
            this->postInited(*this);
        }
        Player(const QString &serviceName, const QDBusConnection &connection)
                     : resolveIFace(new QDBusInterface(DBusUtils::getPropertyResolverIFace(serviceName, Constants::MPRIS_Path,
                                                                                           connection))),
                       playerInterface(new QDBusInterface(serviceName, Constants::MPRIS_Path, Constants::MPRIS_PlayerInterface,
                                                          connection)),
                       connection(connection){

            this->data.serviceName = serviceName;

            // Bind player
            this->connection.connect(this->data.serviceName, PVC::Constants::MPRIS_Path,
                                PVC::Constants::DBus_PropertiesInterface, "PropertiesChanged",
                                this, SLOT(onChangedSlot(QString, QVariantMap, QStringList)));

            // Init PlayerData
            this->parsePlayer();
            this->postInited(*this);
        }

        // API:
        /**
         * \brief Get player data
         */
        [[nodiscard]] const PlayerData &getData() const{
            return this->data;
        }

        /**
         * \brief Try to resume/start player
         * \return Result of the operation
         */
        bool play(){
            if (!this->data.bCanPlay){
                qDebug() << "Player: Can't pause player that doesn't allow to play it -" << this->data.serviceName;
                return false;
            }
            if (this->data.status == PlaybackStatus::Playing){
                qDebug() << "Player: Can't play player that already playing -" << this->data.serviceName;
                return false;
            }

            QDBusMessage Msg = DBusUtils::callFunction("Play", *this->playerInterface);
            if (Msg.type() == QDBusMessage::ErrorMessage){
                qDebug() << "Player: Error when tried to play player" << Msg.errorName() << Msg.errorMessage();
                return false;
            }

            qDebug() << "Player: Play" << this->data.serviceName;
            return true;
        }

        /**
         * \brief Try to pause player
         * \return Result of the operation
         */
        bool pause(){
            if (!this->data.bCanPause){
                qDebug() << "Player: Can't pause player that haven't pause feature -" << this->data.serviceName;
                return false;
            }
            if (this->data.status != PlaybackStatus::Playing){
                qDebug() << "Player: Can't pause player that isn't playing -" << this->data.serviceName;
                return false;
            }

            QDBusMessage Msg = DBusUtils::callFunction("Pause", *this->playerInterface);
            if (Msg.type() == QDBusMessage::ErrorMessage){
                qDebug() << "Player: Error when tried to pause player" << Msg.errorName() << Msg.errorMessage();
                return false;
            }

            qDebug() << "Player: Pause" << this->data.serviceName;
            return true;
        }

        // Operators
        Player &operator=(const Player &rhs){
            this->data = rhs.data;
            this->connection = rhs.connection;

            this->resolveIFace = rhs.playerInterface;
            this->playerInterface = rhs.playerInterface;

            return *this;
        }

    signals:
        void postInited(Player &self);
        void onChanged(Player &self);

    private slots:
        /**
         * \brief Slot that receive MPRIS property change event
         * @param interfaceName
         * @param changedProps
         * @param invalidatedProps
         */
        void onChangedSlot(QString interfaceName, QVariantMap changedProps, QStringList invalidatedProps) {
            // Update
            this->parsePlayer();
            this->onChanged(*this); // Fire signal
        };
    };
}

#endif //PVC_PLAYER_H
