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
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NOTIFICATIONSERVER_H
#define NOTIFICATIONSERVER_H

#include"notify-backend.h"

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

#include <QDBusAbstractAdaptor>
#include <QStringList>
#include <QDBusVariant>

typedef QMap<QString, QDBusVariant> Hints;

Q_DECLARE_METATYPE(Hints)

class NotificationModel;
class Notification;

class NotificationServer : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_INTERFACE)

public:
    NotificationServer(NotificationModel &m, QObject *parent=nullptr);
    ~NotificationServer();
    void invokeAction(unsigned int id, QString action);

public Q_SLOTS:
    void CloseNotification (unsigned int id);
    void GetServerInformation (QString &name, QString &vendor, QString &version, QString &specVersion) const;
    QStringList GetCapabilities() const;
    unsigned int Notify (QString app_name, unsigned int replaces_id, QString app_icon, QString summary, QString body,
            QStringList actions, Hints hints, int expire_timeout);
    void onDataChanged(unsigned int id);

Q_SIGNALS:
    void NotificationClosed(unsigned int id, unsigned int reason);
    void ActionInvoked(unsigned int id, QString action_key);
    void dataChanged(unsigned int id);

private:
    Notification* buildNotification(NotificationID id, const Hints &hints);
    NotificationModel &model;
    unsigned int idCounter;

};

#endif
