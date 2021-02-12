//
// Created by u7i on 1/26/21.
//

#ifndef PVC_DBUSUTILS_H
#define PVC_DBUSUTILS_H

#include <QtDBus>
#include "Constants.h"

namespace PVC{
    /**
     * \brief Gen copy params for QDBusInterface ( try to emulate copy constructor )
     */
#define __dbus_interface_copy_params(source, connection) source.service(), source.path(), source.interface(), connection

    /**
     * \brief Utils for QDBus
     * \author u7i
     * \version 0.1
     */
    class DBusUtils{
    public:
        /**
         * \brief Recursive get all dbus objects that contains service name
         *
         * @param serviceName Name of dbus service
         * @param connection DBus connection. For example sessionBus()
         * @return List with found services name
        */
        static QList<QString> getObjectsByServiceName(const QString &serviceName, QDBusConnection &connection){
            QList<QString> list;

            // Get all dbus services
            QDBusInterface interface("org.freedesktop.DBus", "/org/freedesktop/DBus","org.freedesktop.DBus",
                                     connection);

            QList<QString> temp = interface.call("ListNames").arguments().at(0).toStringList();

            for (const QString &str : temp){
                if (str.contains(serviceName)){
                    list.push_back(str);
                }
            }
            return list;
        }

        /**
         * \brief Call DBUS interface method
         *
         * @param functionName Name of the method
         * @param IFace Interface
         * @return Result of method execution
         */
        static QDBusMessage callFunction(const QString &functionName, QDBusInterface &IFace){
            return IFace.call(functionName);
        }

        // Interface generators
        /**
         * \brief Get interface that allows to get DBUS object properties
         *
         * @param service Name of service
         * @param path Path to object
         * @param connection DBus connection
         * @return Interface
         */
        static QDBusInterface getPropertyResolverIFace(const QString &service, const QString &path,
                                                      const QDBusConnection &connection) {

            return QDBusInterface(service, path, Constants::DBus_PropertiesInterface, connection);
        }

        /**
         * \brief Get property from DBus object
         *
         * @param propertyName Name of the property
         * @param resolverInterface . You can get it by getPropertyResolverIFace()
         * @return
         */
        static QVariant getProperty(const QString &propertyName, const QString &propertyInterface,
                                        QDBusInterface &resolverInterface){

            QDBusMessage Msg = resolverInterface.call("Get", propertyInterface, propertyName);
            return Msg.arguments()[0].value<QDBusVariant>().variant();
        }
    };
}

#endif //PVC_DBUSUTILS_H