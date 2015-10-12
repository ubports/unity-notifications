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
#include <NotificationsAdaptor.h>
#include <QDBusMetaType>
#include <QDebug>
#include <QDBusConnectionInterface>
#include <QSharedPointer>

static const char* LOCAL_OWNER = "local";

static bool isAuthorised(const QString& clientId, QSharedPointer<Notification> notification)
{
    return (clientId == LOCAL_OWNER) || (notification->getClientId() == clientId);
}

NotificationServer::NotificationServer(const QDBusConnection& connection, NotificationModel &m, QObject *parent) :
    QObject(parent), model(m), idCounter(0), m_connection(connection) {

    DBusTypes::registerNotificationMetaTypes();

    // Memory managed by Qt
    new NotificationsAdaptor(this);

    m_watcher.setConnection(m_connection);
    m_watcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
    connect(&m_watcher, &QDBusServiceWatcher::serviceUnregistered, this, &NotificationServer::serviceUnregistered);

    connect(this, SIGNAL(dataChanged(unsigned int)), &m, SLOT(onDataChanged(unsigned int)));

    if(!m_connection.registerObject(DBUS_PATH, this)) {
        qWarning() << "Could not register to DBus object.";
    }

    QDBusConnectionInterface *iface = m_connection.interface();
    auto reply = iface->registerService(DBUS_SERVICE_NAME, QDBusConnectionInterface::ReplaceExistingService,
                                        QDBusConnectionInterface::DontAllowReplacement);
    if(!reply.isValid() || reply.value() != QDBusConnectionInterface::ServiceRegistered) {
        qWarning() << "Notification DBus name already taken.";
    }
}

NotificationServer::~NotificationServer() {
    m_connection.unregisterService(DBUS_SERVICE_NAME);
}

void NotificationServer::invokeAction(unsigned int id, const QString &action) {
    Q_EMIT ActionInvoked(id, action);
}

QStringList NotificationServer::GetCapabilities() const {
    QStringList capabilities;
    capabilities.push_back("actions");
    capabilities.push_back("body");
    capabilities.push_back("body-markup");
    capabilities.push_back("icon-static");
    capabilities.push_back("image/svg+xml");
    capabilities.push_back(VALUE_HINT);
    capabilities.push_back(VALUE_TINT_HINT);
    capabilities.push_back(URGENCY_HINT);
    capabilities.push_back(SOUND_HINT);
    capabilities.push_back(SUPPRESS_SOUND_HINT);
    capabilities.push_back(SYNCH_HINT);
    capabilities.push_back(ICON_ONLY_HINT);
    capabilities.push_back(AFFIRMATIVE_TINT_HINT);
    capabilities.push_back(REJECTION_TINT_HINT);
    capabilities.push_back(TRUNCATION_HINT);
    capabilities.push_back(SNAP_HINT);
    capabilities.push_back(SECONDARY_ICON_HINT);
    capabilities.push_back(NON_SHAPED_ICON_HINT);
    capabilities.push_back(MENU_MODEL_HINT);
    capabilities.push_back(INTERACTIVE_HINT);
    capabilities.push_back(TIMEOUT_HINT);
    capabilities.push_back(SWIPE_HINT);

    return capabilities;
}

NotificationDataList NotificationServer::GetNotifications(const QString &app_name) {
    NotificationDataList results;
    for (auto notification: model.getAllNotifications()) {
        NotificationData data;
        data.appName = app_name;
        data.id = notification->getID();
        data.appIcon = notification->getIcon();
        data.summary = notification->getSummary();
        data.body = notification->getBody();
        data.actions = notification->getActions()->getRawActions();
        data.hints = notification->getHints();
        data.expireTimeout = notification->getDisplayTime();

        results << data;
    }

    return results;
}

QSharedPointer<Notification> NotificationServer::buildNotification(NotificationID id, const QVariantMap &hints) {
    int expireTimeout = 0;
    Notification::Urgency urg = Notification::Urgency::Low;
    if(hints.find(URGENCY_HINT) != hints.end()) {
        QVariant u = hints[URGENCY_HINT];
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
        QVariant u = hints[TIMEOUT_HINT];
        if(!u.canConvert(QVariant::Int)) {
            expireTimeout = 60000;
        } else {
            expireTimeout = u.toInt();
        }

        ntype = Notification::Type::SnapDecision;
    } else if(hints.find(INTERACTIVE_HINT) != hints.end()) {
        ntype = Notification::Type::Interactive;
        expireTimeout = 5000;
    }

    QSharedPointer<Notification> n(new Notification(id, expireTimeout, urg, ntype, this));
    connect(n.data(), SIGNAL(dataChanged(unsigned int)), this, SLOT(onDataChanged(unsigned int)));
    connect(n.data(), SIGNAL(completed(unsigned int)), this, SLOT(onCompleted(unsigned int)));

    return n;
}

void NotificationServer::incrementCounter() {
    idCounter++;
    // Spec forbids zero as return value.
    if(idCounter == 0) {
        idCounter = 1;
}
}

QString NotificationServer::messageSender() {
    QString sender(LOCAL_OWNER);
        if (calledFromDBus()) {
                sender = message().service();
        }
        return sender;
}

bool NotificationServer::isCmdLine() {
    if (!calledFromDBus()) {
        return false;
    }
    QString sender = message().service();
    uint pid = connection().interface()->servicePid(sender);
    QString path = QDir(QString("/proc/%1/exe").arg(pid)).canonicalPath();
    return (path == "/usr/bin/notify-send");
}

void NotificationServer::serviceUnregistered(const QString &clientId) {
    m_watcher.removeWatchedService(clientId);
    auto notifications = model.removeAllNotificationsForClient(clientId);
    for (auto notification: notifications) {
        Q_EMIT NotificationClosed(notification->getID(), 1);
    }
}

unsigned int NotificationServer::Notify(const QString &app_name, uint replaces_id,
                           const QString &app_icon, const QString &summary,
                           const QString &body, const QStringList &actions,
                           const QVariantMap &hints, int expire_timeout) {
    const unsigned int FAILURE = 0;
    const int minActions = 4;
    const int maxActions = 14;
    //QImage icon(app_icon);
    int currentId = 0;

    bool newNotification = true;
    QString clientId = messageSender();

    QSharedPointer<Notification> notification;
    if (replaces_id != 0) {
        if (model.hasNotification(replaces_id)) {
            newNotification = false;
            notification = model.getNotification(replaces_id);
            if (!isAuthorised(clientId, notification)) {
                auto message =
                        QString::fromUtf8(
                                "Client '%1' tried to update notification %2, which it does not own.").arg(
                                clientId).arg(replaces_id);
                qWarning() << message;
                sendErrorReply(QDBusError::InvalidArgs, message);
                return FAILURE;
            }
        } else {
            notification = buildNotification(replaces_id, hints);
        }

        currentId = replaces_id;
    } else {
        incrementCounter();
        while (model.hasNotification(idCounter)) {
            incrementCounter();
        }
        notification = buildNotification(idCounter, hints);

        currentId = idCounter;
    }

    Q_ASSERT(notification);
    if(notification->getType() == Notification::Type::Interactive) {
        int numActions = actions.size();
        if(numActions != 2) {
            sendErrorReply(
                    QDBusError::InvalidArgs,
                    QString::fromUtf8(
                            "Wrong number of actions for an interactive notification. Has %1, requires 2.").arg(
                            numActions));
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
                sendErrorReply(
                        QDBusError::InvalidArgs,
                        QString::fromUtf8(
                                "Too few strings for Snap Decisions. Has %1, requires %2.").arg(
                                numActions).arg(minActions));
                return FAILURE;
            }
        }
        if(numActions > maxActions) {
            sendErrorReply(
                    QDBusError::InvalidArgs,
                    QString::fromUtf8(
                            "Too many strings for Snap Decisions. Has %1, maximum %2.").arg(
                            numActions).arg(maxActions));
            return FAILURE;
        }
        if(numActions % 2 != 0) {
            sendErrorReply(
                    QDBusError::InvalidArgs,
                    QString::fromUtf8(
                            "Number of actions must be even, not odd."));
            return FAILURE;
        }
        notification->setActions(actions);
    }
    notification->setBody(body);
    notification->setIcon(app_icon);
    notification->setSummary(summary);

    QVariantMap notifyHints;
    for (auto iter = hints.constBegin(), end = hints.constEnd(); iter != end; ++iter) {
        notifyHints[iter.key()] = iter.value();
    }
    notification->setHints(notifyHints);

    QVariant secondaryIcon = hints[SECONDARY_ICON_HINT];
    notification->setSecondaryIcon(secondaryIcon.toString());

    QVariant value = hints[VALUE_HINT];
    notification->setValue(value.toInt());

    notification->setClientId(clientId);

    if (newNotification) {
        // Don't clean up after the command line client closes
        if (!isCmdLine()) {
            m_watcher.addWatchedService(clientId);
        }
        model.insertNotification(notification);
    } else {
        model.notificationUpdated(currentId);
    }
    return currentId;
}

void NotificationServer::CloseNotification (unsigned int id) {

    // Sanity checking the public bus method's arguments:
    // if this was called from the bus for a notification which doesn't
    // exist or isn't owned by the caller, then reply with an error
    if (calledFromDBus()) {
        auto notification = model.getNotification(id);
        const auto clientId = messageSender();
        if (!notification || !isAuthorised(clientId, notification)) {
            auto err = QString::fromUtf8(
                "Client '%1' tried to close notification %2, which it does not own or does not exist.")
                .arg(clientId)
                .arg(id);
            qWarning() << err;
            sendErrorReply(QDBusError::InvalidArgs, err);
            return;
        }
    }

    forceCloseNotification(id);
}

void NotificationServer::forceCloseNotification (unsigned int id) {

    model.removeNotification(id);

    Q_EMIT NotificationClosed(id, 1);
}


QString NotificationServer::GetServerInformation (QString &vendor, QString &version, QString &specVersion) const {
    vendor = "Canonical Ltd";
    version = "1.2";
    specVersion = "1.1";
    return "Unity notification server";
}

void NotificationServer::onDataChanged(unsigned int id) {
    Q_EMIT dataChanged(id);
}

void NotificationServer::onCompleted(unsigned int id) {
    CloseNotification(id);
}
