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

void NotificationServer::invokeAction(unsigned int id, QString action) {
    emit ActionInvoked(id, action);
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
    capabilities.push_back(APPEND_HINT);
    capabilities.push_back("x-canonical-private-icon-only");
    capabilities.push_back("x-canonical-truncation");
    return capabilities;
}

Notification* NotificationServer::buildNotification(NotificationID id, const Hints &hints, int expireTimeout) {
    Notification::Urgency urg = Notification::Urgency::Low;
    if(hints.find(URGENCY_HINT) != hints.end()) {
        QVariant u = hints[URGENCY_HINT].variant();
        if(!u.canConvert(QVariant::Int)) {
            printf("Invalid urgency value.\n");
        } else {
            urg = (Notification::Urgency) u.toInt();
        }
    }
    Notification::Type ntype = Notification::Type::Ephemeral;
    if(hints.find(SYNCH_HINT) != hints.end()) {
        ntype = Notification::Type::Confirmation;
    } else if (hints.find(SNAP_HINT) != hints.end()) {
        ntype = Notification::Type::SnapDecision;
    } else if(hints.find(INTERACTIVE_HINT) != hints.end()) {
        ntype = Notification::Type::Interactive;
    }
    return new Notification(id, expireTimeout, urg, ntype, this);

}

unsigned int NotificationServer::Notify (QString app_name, unsigned int replaces_id, QString app_icon,
        QString summary, QString body,
        QStringList actions, Hints hints, int expire_timeout) {
    const unsigned int FAILURE = (unsigned int)-1; // Is this correct?
    const int minActions = 4;
    const int maxActions = 12;
    //QImage icon(app_icon);
    int currentId = idCounter;
    QSharedPointer<Notification> notification;
    if(replaces_id != 0) {
        if(!model.hasNotification(replaces_id))
            return FAILURE;
        currentId = replaces_id;
        notification = model.getNotification(replaces_id);
        // Appending text is a special case.
        if (hints.find(APPEND_HINT) != hints.end()) {
            notification->setBody(notification->getBody() + body);
            return notification->getID();
        }
    } else {
        Notification *n = buildNotification(currentId, hints, expire_timeout);
        if(!n) {
            return FAILURE;
        }
        notification.reset(n);
        idCounter++;
        if(idCounter == 0) // Spec forbids zero as return value.
            idCounter = 1;
    }

    // Do this first because it can fail. In case we are updating an
    // existing notification exiting now means all old state is
    // preserved.
    if(notification->getType() == Notification::Type::SnapDecision) {
        QVariant snapActions = hints[SNAP_HINT].variant();
        if(!snapActions.canConvert<QStringList>()) {
            printf("Malformed snap decisions list.\n");
            return FAILURE;
        }
        QStringList actionList = snapActions.toStringList();
        int numActions = actionList.size();
        if(numActions < minActions) {
            printf("Too few strings for Snap Decisions. Has %d, requires %d.\n", numActions, minActions);
            return FAILURE;
        }
        if(numActions > maxActions) {
            printf("Too many strings for Snap Decisions. Has %d, maximum %d.\n", numActions, maxActions);
            return FAILURE;
        }
        notification->setActions(actionList);
    } else {
        notification->setActions(actions);
    }
    notification->setBody(body);
    notification->setIcon(app_icon);
    notification->setSummary(summary);
    if(replaces_id == 0) {
        model.insertNotification(notification);
    }
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
