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

#define DBUS_SERVICE_NAME "com.canonical.notificationproto" // org.freedesktop.Notifications
#define DBUS_INTERFACE "com.canonical.notificationproto" // "org.freedesktop.Notifications
#define DBUS_PATH "/com/canonical/notificationproto" // "/org/freedesktop.Notifications"

class NotificationServer : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_INTERFACE)
/*
    Q_CLASSINFO("D-Bus Introspection", ""
            "  <interface name=\"com.canonical.notificationproto\">\n"
            "    <method name=\"GetCapabilities\"/>\n"
            "  </interface>\n"
            )
            */
public:
    NotificationServer(QObject *parent=nullptr);
    ~NotificationServer();

public slots:
    QStringList GetCapabilities() const;
    int Notify (QString app_name, int replaces_id, QString app_icon, QString summary, QString body,
            QStringList actions, QMap<QString, QString>hints, int expire_timeout);
    void CloseNotification (int id);
    void GetServerInformation (QString &name, QString &vendor, QString &version) const;

signals:

    void NotificationClosed(int id, int reason);
    void ActionInvoked(int id, QString action_key);

};

#endif
