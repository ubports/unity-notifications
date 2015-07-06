/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NOTIFICATIONSERVER_H
#define NOTIFICATIONSERVER_H

#include "notify-backend.h"

/* http://www.galago-project.org/specs/notification/0.9/x408.html#commands
 *
 * The functions to implement:
 *
 * STRING_ARRAY org.freedesktop.Notifications.GetCapabilities (void);
 * UINT32 org.freedesktop.Notifications.Notify (STRING app_name, UINT32 replaces_id,
 *   STRING app_icon, STRING summary, STRING body, ARRAY actions, DICT hints, INT32 expire_timeout);
 * void org.freedesktop.Notifications.CloseNotification (UINT32 id);
 * void org.freedesktop.Notifications.GetServerInformation (out STRING name, out STRING vendor, out STRING version);
 *
 * Signals:
 * org.freedesktop.Notifications.NotificationClosed (UINT32 id, UINT32 reason);
 * org.freedesktop.Notifications.ActionInvoked (UINT32 id, STRING action_key);
 *
 */

#include <DBusTypes.h>

#include <QDBusAbstractAdaptor>
#include <QStringList>
#include <QDBusVariant>
#include <QDBusConnection>
#include <QDBusContext>
#include <QDBusServiceWatcher>

class NotificationModel;
class Notification;
class NotificationsAdaptor;

class NotificationServer : public QObject, protected QDBusContext {

    Q_OBJECT

    friend NotificationsAdaptor;

public:
    NotificationServer(const QDBusConnection& connection, NotificationModel &m, QObject *parent=nullptr);
    ~NotificationServer();
    void invokeAction(unsigned int id, const QString &action);
    void forceCloseNotification (unsigned int id);

public Q_SLOTS:
    void CloseNotification (unsigned int id);
    QString GetServerInformation (QString &vendor, QString &version, QString &specVersion) const;
    QStringList GetCapabilities() const;
    NotificationDataList GetNotifications(const QString &app_name);
    unsigned int Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, const QVariantMap &hints, int expire_timeout);
    void onDataChanged(unsigned int id);
    void onCompleted(unsigned int id);

private Q_SLOTS:
    void serviceUnregistered(const QString &service);

Q_SIGNALS:
    void NotificationClosed(unsigned int id, unsigned int reason);
    void ActionInvoked(unsigned int id, const QString &action_key);
    void dataChanged(unsigned int id);

private:
    void incrementCounter();
    QString messageSender();
    bool isCmdLine();

    QSharedPointer<Notification> buildNotification(NotificationID id, const QVariantMap &hints);
    NotificationModel &model;
    unsigned int idCounter;
    QDBusConnection m_connection;
    QDBusServiceWatcher m_watcher;

};

#endif
