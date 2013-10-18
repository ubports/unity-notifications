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

#include "NotificationClient.h"
#include "NotificationServer.h"
#include <QStringList>
#include <QDBusReply>

NotificationClient::NotificationClient(QObject *parent) : QObject(parent),
    service(DBUS_SERVICE_NAME, DBUS_PATH, DBUS_INTERFACE) {

}

NotificationClient::~NotificationClient() {

}

NotificationID NotificationClient::sendNotification(Notification::Type ntype, Notification::Urgency urg, const QString &summary, const QString &body) {
    QString app_name("client test");
    unsigned int replaces_id = 0;
    QString app_icon("/usr/share/icons/unity-icon-theme/search/16/search_field.png");
    QStringList actions;
    QMap<QString, QVariant> hints;
    hints["urgency"] = (char)urg;

    if(ntype == Notification::Type::Confirmation) {
        hints[SYNCH_HINT] = "yes";
    }
    if(ntype == Notification::Type::SnapDecision) {
        QStringList snaps;
        snaps.push_back("Ok");
        snaps.push_back("ok_id");
        snaps.push_back("Cancel");
        snaps.push_back("cancel_id");
        hints[SNAP_HINT] = snaps;
    }
    if(ntype == Notification::Type::Interactive) {
        hints[INTERACTIVE_HINT] = "targetapp";
    }
    int timeout = 5000;
    QDBusReply<unsigned int> result = service.call("Notify",
            app_name, replaces_id, app_icon, summary, body, actions, hints, timeout);
    if(!result.isValid()) {
        return (NotificationID) -1;
    }
    return result.value();
}

NotificationID NotificationClient::appendText(NotificationID id, const QString &text) {
    QString app_name("append");
    QString app_icon;
    QString summary;
    QStringList actions;
    QMap<QString, QVariant> hints;
    hints[APPEND_HINT] = QVariant(true);
    int timeout = 5000;
    QDBusReply<unsigned int> result = service.call("Notify",
            app_name, id, app_icon, summary, text, actions, hints, timeout);
    if(!result.isValid()) {
        return (NotificationID) -1;
    }
    return result.value();

}

void NotificationClient::NotificationClosed(NotificationID id, unsigned int reason) {
    Q_EMIT closed(id, reason);
    QString msg("Got NotificationClosed signal for notification ");
    msg += QString::number(id, 10);
    msg += ".\n";
    Q_EMIT eventHappened(msg);
}

void NotificationClient::ActionInvoked(NotificationID id, const QString &key) {
    Q_EMIT invoked(id, key);
    QString msg("Got ActionInvoked signal for notification ");
    msg += QString::number(id, 10);
    msg += " event \"";
    msg += key;
    msg += "\".\n";
    Q_EMIT eventHappened(msg);
}
