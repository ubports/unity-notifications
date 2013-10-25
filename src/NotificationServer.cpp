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

#include "NotificationModel.h"
#include "NotificationServer.h"
#include "Notification.h"
#include <QDBusMetaType>
#include <QSharedPointer>

NotificationServer::NotificationServer(NotificationModel &m, QObject *parent) :
    QDBusAbstractAdaptor(parent), model(m), idCounter(1) {
    qDBusRegisterMetaType<Hints>();

    connect(this, SIGNAL(dataChanged(unsigned int)), &m, SLOT(onDataChanged(unsigned int)));
}

NotificationServer::~NotificationServer() {

}

void NotificationServer::invokeAction(unsigned int id, const QString &action) {
    Q_EMIT ActionInvoked(id, action);
}


#define INTERACTIVE_HINT "x-canonical-switch-to-application"




QStringList NotificationServer::GetCapabilities() const {
    QStringList capabilities;
    capabilities.push_back("actions");
    capabilities.push_back("body");
    capabilities.push_back("body-markup");
    capabilities.push_back("icon-static");
    capabilities.push_back("image/svg+xml");
    capabilities.push_back(URGENCY_HINT);
    capabilities.push_back(SYNCH_HINT);
    capabilities.push_back(APPEND_HINT);
    capabilities.push_back(ICON_ONLY_HINT);
    capabilities.push_back(BUTTON_TINT_HINT);
    capabilities.push_back(TRUNCATION_HINT);
    capabilities.push_back(SNAP_HINT);
    capabilities.push_back(SECONDARY_ICON_HINT);
    capabilities.push_back(MENU_MODEL_HINT);
    capabilities.push_back(INTERACTIVE_HINT);
    capabilities.push_back(FULLSCREEN_HINT);

    return capabilities;
}

Notification* NotificationServer::buildNotification(NotificationID id, const Hints &hints) {
    int expireTimeout = 0;
    Notification::Urgency urg = Notification::Urgency::Low;
    if(hints.find(URGENCY_HINT) != hints.end()) {
        QVariant u = hints[URGENCY_HINT].variant();
        if(!u.canConvert(QVariant::Int)) {
            fprintf(stderr, "Invalid urgency value.\n");
        } else {
            urg = (Notification::Urgency) u.toInt();
        }
    }
    Notification::Type ntype = Notification::Type::Ephemeral;
    expireTimeout = 5000;
    if(hints.find(SYNCH_HINT) != hints.end()) {
        expireTimeout = 3000;
        ntype = Notification::Type::Confirmation;
    } else if (hints.find(SNAP_HINT) != hints.end()) {
        expireTimeout = 60000;
        ntype = Notification::Type::SnapDecision;
    } else if(hints.find(INTERACTIVE_HINT) != hints.end()) {
        ntype = Notification::Type::Interactive;
        expireTimeout = 5000;
    }

    Notification* n = new Notification(id, expireTimeout, urg, ntype, this);
    connect(n, SIGNAL(dataChanged(unsigned int)), this, SLOT(onDataChanged(unsigned int)));
    connect(n, SIGNAL(completed(unsigned int)), this, SLOT(onCompleted(unsigned int)));

    return n;
}

unsigned int NotificationServer::Notify (const QString &app_name, unsigned int replaces_id, const QString &app_icon,
        const QString &summary, const QString &body,
        const QStringList &actions, const Hints &hints, int expire_timeout) {
    const unsigned int FAILURE = 0; // Is this correct?
    const int minActions = 4;
    const int maxActions = 12;
    //QImage icon(app_icon);
    int currentId = idCounter;
    QSharedPointer<Notification> notification;
    if(replaces_id != 0) {
        if(!model.hasNotification(replaces_id)) {
            fprintf(stderr, "Tried to change non-existing notification %d.\n", replaces_id);
            return FAILURE;
        }
        currentId = replaces_id;
        notification = model.getNotification(replaces_id);
        // Appending text is a special case. This is the new update-behaviour
        // for the append-hint and expects the client-app to use libnotify's
        // call notify_notification_update()
        if (hints.find(APPEND_HINT) != hints.end()) {
            QString newBody = QString(notification->getBody() + "\n" + body);
            notification->setBody(newBody);
            model.notificationUpdated(currentId);
            return notification->getID();
        }
        // Otherwise we let the code below update the fields.
    } else {

        // support append-hint also for a new notification using just the
        // summary-text as "identifier"... this is legacy behaviour and
        // only kept for compatibility-reasons... developer-docs have to
        // be updated and extended with a remark that this will be dropped
        // by the next cycle
        if (hints.find(APPEND_HINT) != hints.end()) {
            notification = model.getNotification(summary);
            if (notification) {
                QString newBody = QString(notification->getBody() + "\n" + body);
                notification->setBody(newBody);
                model.notificationUpdated(notification->getID());
                return notification->getID();
            }
        }

        Notification *n = buildNotification(currentId, hints);
        if(!n) {
            fprintf(stderr, "Could not build notification object.\n");
            return FAILURE;
        }
        notification.reset(n);
        idCounter++;
        if(idCounter == 0) // Spec forbids zero as return value.
            idCounter = 1;
    }

    if(notification->getType() == Notification::Type::Interactive) {
        int numActions = actions.size();
        if(numActions != 2) {
            fprintf(stderr, "Wrong number of actions for an interactive notification. Has %d, requires %d.\n", numActions, 2);
            return FAILURE;
        }
        notification->setActions(actions);
    }

    // Do this first because it can fail. In case we are updating an
    // existing notification exiting now means all old state is
    // preserved.
    if(notification->getType() == Notification::Type::SnapDecision) {
        int numActions = actions.size();
        if(numActions < minActions) {
            if (hints.find(MENU_MODEL_HINT) == hints.end()) {
                fprintf(stderr, "Too few strings for Snap Decisions. Has %d, requires %d.\n", numActions, minActions);
                return FAILURE;
            }
        }
        if(numActions > maxActions) {
            fprintf(stderr, "Too many strings for Snap Decisions. Has %d, maximum %d.\n", numActions, maxActions);
            return FAILURE;
        }
        if(numActions % 2 != 0) {
            fprintf(stderr, "Number of actions must be even, not odd.\n");
            return FAILURE;
        }
        notification->setActions(actions);
    }
    notification->setBody(body);
    notification->setIcon(app_icon);
    notification->setSummary(summary);

    QVariantMap notifyHints;
    for (Hints::const_iterator iter = hints.constBegin(), end = hints.constEnd(); iter != end; ++iter) {
        notifyHints[iter.key()] = iter.value().variant();
    }
    notification->setHints(notifyHints);

    QVariant secondaryIcon = hints[SECONDARY_ICON_HINT].variant();
    notification->setSecondaryIcon(secondaryIcon.toString());

    if(replaces_id == 0) {
        model.insertNotification(notification);
    }
    return currentId;
}

void NotificationServer::CloseNotification (unsigned int id) {
    Q_EMIT NotificationClosed(id, 1);
    model.removeNotification(id);
}

void NotificationServer::GetServerInformation (QString &name, QString &vendor, QString &version, QString &specVersion) const {
    name = "Unity notification server";
    vendor = "Canonical Ltd";
    version = "1.2";
    specVersion = "1.1";
}

void NotificationServer::onDataChanged(unsigned int id) {
    Q_EMIT dataChanged(id);
}

void NotificationServer::onCompleted(unsigned int id) {
    CloseNotification(id);
}
