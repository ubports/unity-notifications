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

#include "notificationmodel.h"
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
    capabilities.push_back("urgency");
    capabilities.push_back("x-canonical-private-synchronous");
    capabilities.push_back("x-canonical-append");
    capabilities.push_back("x-canonical-private-icon-only");
    capabilities.push_back("x-canonical-truncation");
    return capabilities;
}

Notification* NotificationServer::buildNotification(NotificationID id, const Hints &hints) {
    Urgency urg = URGENCY_LOW;
    if(hints.find(URGENCY_HINT) != hints.end()) {
        QVariant u = hints[URGENCY_HINT].variant();
        if(!u.canConvert(QVariant::Int)) {
            printf("Invalid urgency value.\n");
        } else {
            urg = (Urgency) u.toInt();
        }
    }
    NotificationType ntype = ASYNCHRONOUS;
    if(hints.find(SYNCH_HINT) != hints.end()) {
        ntype = SYNCHRONOUS;
    } else if (hints.find(SNAP_HINT) != hints.end()) {
        ntype = SNAP;
    } else if(hints.find(INTERACTIVE_HINT) != hints.end()) {
        ntype = INTERACTIVE;
    }
    return new Notification(id, urg, ntype, this);

}

unsigned int NotificationServer::Notify (QString app_name, unsigned int replaces_id, QString app_icon,
        QString summary, QString body,
        QStringList actions, Hints hints, int expire_timeout) {
    if(replaces_id != 0) {
        // Update existing notification.
        // Not implemented yet.
        return replaces_id;
    }
    int currentId = idCounter;
    Notification *n = buildNotification(currentId, hints);
    if(!n) {
        return 0;
    }
    QSharedPointer<Notification> notification(n);
    n->setText(body);
    model.insertNotification(notification);
    idCounter++;
    if(idCounter == 0) // Spec forbids zero as return value.
        idCounter = 1;
    return currentId;
}

void NotificationServer::CloseNotification(NotificationID id, unsigned int reason) {
    emit NotificationClosed(id, reason);
}

void NotificationServer::GetServerInformation (QString &name, QString &vendor, QString &version) const {
    name = "Unity notification server";
    vendor = "Canonical";
    version = "something";
}
