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

#include "notificationserver.h"
#include "notification.h"
#include <QDBusMetaType>
#include <QSharedPointer>

NotificationServer::NotificationServer(NotificationModel &m, QObject *parent) :
    QDBusAbstractAdaptor(parent), model(m), idCounter(1) {
    qDBusRegisterMetaType<Hints>();
}

NotificationServer::~NotificationServer() {

}

QStringList NotificationServer::GetCapabilities() const {
    QStringList capabilities;
    capabilities.push_back("actions");
    capabilities.push_back("body");
    capabilities.push_back("body-markup");
    capabilities.push_back("icon-static");
    capabilities.push_back("image/svg+xml");
    capabilities.push_back("x-canonical-private-synchronous");
    capabilities.push_back("x-canonical-append");
    capabilities.push_back("x-canonical-private-icon-only");
    capabilities.push_back("x-canonical-truncation");
    return capabilities;
}

unsigned int NotificationServer::Notify (QString app_name, unsigned int replaces_id, QString app_icon,
        QString summary, QString body,
        QStringList actions, /*Hints hints,*/ int expire_timeout) {
    Urgency urg = URGENCY_LOW;
    NotificationType ntype = ASYNCHRONOUS;
    QSharedPointer<Notification> n(new Notification(idCounter, urg, body, ntype));
    return idCounter++;
}

void NotificationServer::CloseNotification(unsigned int id, unsigned int reason) {
    emit NotificationClosed(id, reason);
}

void NotificationServer::GetServerInformation (QString &name, QString &vendor, QString &version) const {
    name = "Unity notification server";
    vendor = "Canonical";
    version = "something";
}
