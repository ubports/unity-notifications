/*
 * Copyright (C) 2013-2016 Canonical, Ltd.
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
#include "Notification.h"

#include <unity/shell/notifications/ModelInterface.h>

#include <QDebug>
#include <QTimer>
#include <QList>
#include <QVector>
#include <QMap>
#include <QStringListModel>
#include <QQmlEngine>

using namespace unity::shell::notifications;

struct NotificationModelPrivate {
    QList<QSharedPointer<Notification> > displayedNotifications;
    QTimer timer;
    QVector<QSharedPointer<Notification> > ephemeralQueue;
    QVector<QSharedPointer<Notification> > interactiveQueue;
    QVector<QSharedPointer<Notification> > snapQueue;
    QMap<NotificationID, int> displayTimes;
};

bool notificationCompare(const QSharedPointer<Notification> &first, const QSharedPointer<Notification> &second) {
    return *first < *second;
}

NotificationModel::NotificationModel(QObject *parent) : QAbstractListModel(parent), p(new NotificationModelPrivate) {
    connect(&(p->timer), SIGNAL(timeout()), this, SLOT(timeout()));
    p->timer.setSingleShot(true);
}

NotificationModel::~NotificationModel() {
    for(int i=0; i<p->ephemeralQueue.size(); i++) {
        p->ephemeralQueue[i]->detachFromServer();
    }
    for(int i=0; i<p->interactiveQueue.size(); i++) {
        p->interactiveQueue[i]->detachFromServer();
    }
    for(int i=0; i<p->snapQueue.size(); i++) {
        p->snapQueue[i]->detachFromServer();
    }
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        p->displayedNotifications[i]->detachFromServer();
    }
}

int NotificationModel::rowCount(const QModelIndex & /* parent */) const {
    //printf("Count %d\n", p->displayedNotifications.size());
    return p->displayedNotifications.size();
}

QVariant NotificationModel::data(const QModelIndex &index, int role) const {
    //printf("Data %d.\n", index.row());
    if (!index.isValid())
        return QVariant();

    switch(role) {
        case ModelInterface::RoleType:
            return p->displayedNotifications[index.row()]->getType();

        case ModelInterface::RoleUrgency:
            return p->displayedNotifications[index.row()]->getUrgency();

        case ModelInterface::RoleId:
            return p->displayedNotifications[index.row()]->getID();

        case ModelInterface::RoleSummary:
            return p->displayedNotifications[index.row()]->getSummary();

        case ModelInterface::RoleBody:
            return p->displayedNotifications[index.row()]->getBody();

        case ModelInterface::RoleValue:
            return p->displayedNotifications[index.row()]->getValue();

        case ModelInterface::RoleIcon:
            return p->displayedNotifications[index.row()]->getIcon();

        case ModelInterface::RoleSecondaryIcon:
            return p->displayedNotifications[index.row()]->getSecondaryIcon();

        case ModelInterface::RoleActions:
            return QVariant::fromValue(p->displayedNotifications[index.row()]->getActions());

        case ModelInterface::RoleHints:
            return p->displayedNotifications[index.row()]->getHints();

        case ModelInterface::RoleNotification:
            return QVariant::fromValue(p->displayedNotifications[index.row()]);

        default:
            return QVariant();
    }
}

void NotificationModel::insertNotification(const QSharedPointer<Notification> &n) {
    if(numNotifications() >= maxNotifications)
        return; // Just ignore. Maybe we should throw()?
    int remaining = p->timer.remainingTime();
    int elapsed = p->timer.interval() - remaining;
    p->timer.stop();
    incrementDisplayTimes(elapsed);
    switch(n->getType()) {
    case Notification::Type::Ephemeral : insertEphemeral(n); break;
    case Notification::Type::Confirmation : insertConfirmation(n); break;
    case Notification::Type::Interactive : insertInteractive(n); break;
    case Notification::Type::SnapDecision : insertSnap(n); break;
    default:
        printf("Unknown notification type, I should probably throw an exception here.\n");
        break;
    }
    int timeout = nextTimeout();
    //Q_ASSERT(timeout > 0);
    p->timer.setInterval(timeout);
    p->timer.start();
}

QList<QSharedPointer<Notification>> NotificationModel::getAllNotifications() const {
    QMap<NotificationID, QSharedPointer<Notification>> notifications;
    for (const auto& notification : p->ephemeralQueue) {
        notifications[notification->getID()] = notification;
    }
    for (const auto& notification : p->interactiveQueue) {
        notifications[notification->getID()] = notification;
    }
    for (const auto& notification : p->snapQueue) {
        notifications[notification->getID()] = notification;
    }
    for (const auto& notification : p->displayedNotifications) {
        notifications[notification->getID()] = notification;
    }
    notifications.remove(0);
    return notifications.values();
}

QSharedPointer<Notification> NotificationModel::getNotification(NotificationID id) const {
    for(int i=0; i<p->ephemeralQueue.size(); i++) {
        if(p->ephemeralQueue[i]->getID() == id) {
            return p->ephemeralQueue[i];
        }
    }
    for(int i=0; i<p->interactiveQueue.size(); i++) {
        if(p->interactiveQueue[i]->getID() == id) {
            return p->interactiveQueue[i];
        }
    }
    for(int i=0; i<p->snapQueue.size(); i++) {
        if(p->snapQueue[i]->getID() == id) {
            return p->snapQueue[i];
        }
    }
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getID() == id) {
            return p->displayedNotifications[i];
        }
    }

    return QSharedPointer<Notification>();
}

QSharedPointer<Notification> NotificationModel::getNotification(const QString &summary) const {
    for(int i=0; i<p->ephemeralQueue.size(); i++) {
        if(p->ephemeralQueue[i]->getSummary() == summary) {
            return p->ephemeralQueue[i];
        }
    }
    for(int i=0; i<p->interactiveQueue.size(); i++) {
        if(p->interactiveQueue[i]->getSummary() == summary) {
            return p->interactiveQueue[i];
        }
    }
    for(int i=0; i<p->snapQueue.size(); i++) {
        if(p->snapQueue[i]->getSummary() == summary) {
            return p->snapQueue[i];
        }
    }
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getSummary() == summary) {
            return p->displayedNotifications[i];
        }
    }

    return QSharedPointer<Notification>();
}

QSharedPointer<Notification> NotificationModel::getDisplayedNotification(int index) const {
    if (index < p->displayedNotifications.size()) {
        return p->displayedNotifications[index];
    } else {
        return QSharedPointer<Notification>();
    }
}

bool NotificationModel::hasNotification(NotificationID id) const {
    return !(getNotification(id).isNull());
}

QList<QSharedPointer<Notification>> NotificationModel::removeAllNotificationsForClient(const QString& clientId)
{
    QList<QSharedPointer<Notification>> notifications;

    for(int i=0; i<p->ephemeralQueue.size();) {
        if(p->ephemeralQueue[i]->getClientId() == clientId) {
            notifications << p->ephemeralQueue.takeAt(i);
            Q_EMIT queueSizeChanged(queued());
        } else {
            ++i;
        }
    }

    for(int i=0; i<p->snapQueue.size();) {
        if(p->snapQueue[i]->getClientId() == clientId) {
            notifications << p->snapQueue.takeAt(i);
            Q_EMIT queueSizeChanged(queued());
        } else {
            ++i;
        }
    }

    for(int i=0; i<p->interactiveQueue.size();) {
        if(p->interactiveQueue[i]->getClientId() == clientId) {
            notifications << p->interactiveQueue.takeAt(i);
            Q_EMIT queueSizeChanged(queued());
        } else {
            ++i;
        }
    }

    bool needToTimeout = false;
    for(int i=0; i<p->displayedNotifications.size();) {
        if(p->displayedNotifications[i]->getClientId() == clientId) {
            notifications << deleteFromVisible(i);
            needToTimeout = true;
        } else {
            ++i;
        }
    }

    if (needToTimeout) {
        timeout(); // Simulate a timeout so visual state is updated.
    }

    return notifications;
}

void NotificationModel::removeNotification(const NotificationID id) {
    for (int i = 0; i < p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getID() == id) {
            deleteFromVisible(i);
            timeout(); // Simulate a timeout so visual state is updated.
            return;
        }
    }

    for (auto it = p->ephemeralQueue.begin(); it != p->ephemeralQueue.end(); ++it) {
        QSharedPointer<Notification> n = *it;

        if (n && n->getID() == id) {
            p->ephemeralQueue.erase(it);
            Q_EMIT queueSizeChanged(queued());
            return;
        }
     }

    for (auto it = p->snapQueue.begin(); it != p->snapQueue.end(); ++it) {
        QSharedPointer<Notification> n = *it;

        if (n && n->getID() == id) {
            p->snapQueue.erase(it);
            Q_EMIT queueSizeChanged(queued());
            return;
        }
    }

    for (auto it = p->interactiveQueue.begin(); it != p->interactiveQueue.end(); ++it) {
        QSharedPointer<Notification> n = *it;

        if (n && n->getID() == id) {
            p->interactiveQueue.erase(it);
            Q_EMIT queueSizeChanged(queued());
            return;
        }
    }

    // The ID was not found in any queue. Should it be an error case or not?
}

void NotificationModel::deleteFirst() {
    if(p->displayedNotifications.empty())
        return;
    deleteFromVisible(0);
}

QSharedPointer<Notification> NotificationModel::deleteFromVisible(int loc) {
    beginRemoveRows(QModelIndex(), loc, loc);
    QSharedPointer<Notification> n = p->displayedNotifications[loc];
    p->displayTimes.erase(p->displayTimes.find(n->getID()));
    auto notification = p->displayedNotifications.takeAt(loc);
    endRemoveRows();
    return notification;
}

void NotificationModel::timeout() {
    bool restartTimer = false;
    // We might call this function before the timer
    // has expired (e.g. because a notification was
    // manually removed)
    if(p->timer.isActive()) {
       incrementDisplayTimes(p->timer.interval() - p->timer.remainingTime());
       p->timer.stop();
    } else {
        incrementDisplayTimes(p->timer.interval());
    }
    pruneExpired();
    if(!p->displayedNotifications.empty()) {
        restartTimer = true;
    }
    // Snap decisions override everything.
    if(showingNotificationOfType(Notification::Type::SnapDecision) || !p->snapQueue.empty()) {
        if(countShowing(Notification::Type::SnapDecision) < maxSnapsShown && !p->snapQueue.empty()) {
            QSharedPointer<Notification> n = p->snapQueue.takeFirst();
            insertToVisible(n, insertionPoint(n));
            restartTimer = true;
            Q_EMIT queueSizeChanged(queued());
        }
    } else {
        restartTimer |= nonSnapTimeout();
    }
    if(restartTimer) {
        int timeout = nextTimeout();
        Q_ASSERT(timeout > 0);
        p->timer.setInterval(timeout);
        p->timer.start();
    }
}

bool NotificationModel::nonSnapTimeout() {
    bool restartTimer;
    if(!showingNotificationOfType(Notification::Type::Interactive) && !p->interactiveQueue.empty()) {
        QSharedPointer<Notification> n = p->interactiveQueue.takeFirst();
        insertToVisible(n, insertionPoint(n));
        restartTimer = true;
        Q_EMIT queueSizeChanged(queued());
    }
    if(!showingNotificationOfType(Notification::Type::Ephemeral) && !p->ephemeralQueue.empty()) {
        QSharedPointer<Notification> n = p->ephemeralQueue.takeFirst();
        insertToVisible(n, insertionPoint(n));
        restartTimer = true;
        Q_EMIT queueSizeChanged(queued());
    }
    return restartTimer;
}

void NotificationModel::pruneExpired() {
    for(int i=p->displayedNotifications.size()-1; i>=0; i--) {
        QSharedPointer<Notification> n = p->displayedNotifications[i];
        int shownTime = p->displayTimes[n->getID()];
        int totalTime = n->getDisplayTime();
        if(totalTime >= 0 && shownTime >= totalTime) {
            deleteFromVisible(i);
        }
    }
}

int NotificationModel::nextTimeout() const {
    int mintime = INT_MAX;
    if(p->displayedNotifications.empty()) {
        // What to do? It really does not make sense
        // to add a timer in this case.
        return 10000;
    }
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        QSharedPointer<Notification> n = p->displayedNotifications[i];
        int totalTime = n->getDisplayTime();
        if (totalTime >= 0) {
            int shownTime = p->displayTimes[n->getID()];
            int remainingTime = totalTime - shownTime;
            if(remainingTime < 0)
                remainingTime = 0;
            if(remainingTime < mintime)
                mintime = remainingTime;
        }
    }
    return mintime;
}

void NotificationModel::insertEphemeral(const QSharedPointer<Notification> &n) {
    Q_ASSERT(n->getType() == Notification::Type::Ephemeral);

    if(showingNotificationOfType(Notification::Type::Ephemeral)) {
        int loc = findFirst(Notification::Type::Ephemeral);
        QSharedPointer<Notification> showing = p->displayedNotifications[loc];
        if(n->getUrgency() > showing->getUrgency()) {
            insertToVisible(n, qMax(0, loc-1));
        } else {
            insertToVisible(n, loc+1);
        }
    } else {
        int loc = insertionPoint(n);
        insertToVisible(n, loc);
    }
}

void NotificationModel::insertInteractive(const QSharedPointer<Notification> &n) {
    Q_ASSERT(n->getType() == Notification::Type::Interactive);

    if(showingNotificationOfType(Notification::Type::Interactive)) {
        int loc = findFirst(Notification::Type::Interactive);
        QSharedPointer<Notification> showing = p->displayedNotifications[loc];
        if(n->getUrgency() > showing->getUrgency()) {
            insertToVisible(n, qMax(0, loc-1));
        } else {
            insertToVisible(n, loc+1);
        }
    } else {
        int loc = insertionPoint(n);
        insertToVisible(n, loc);
    }
}


void NotificationModel::insertConfirmation(const QSharedPointer<Notification> &n) {
    Q_ASSERT(n->getType() == Notification::Type::Confirmation);
    if(showingNotificationOfType(Notification::Type::Confirmation)) {
        deleteFirst(); // Synchronous is always first.
    }
    insertToVisible(n, 0);
}


void NotificationModel::insertSnap(const QSharedPointer<Notification> &n) {
    Q_ASSERT(n->getType() == Notification::Type::SnapDecision);
    int showing = countShowing(n->getType());
    if(showing >= maxSnapsShown) {
        int loc = findFirst(Notification::Type::SnapDecision);
        bool replaced = false;
        for(int i=0; i<showing; i++) {
            if(p->displayedNotifications[loc+i]->getUrgency() > n->getUrgency()) {
                QSharedPointer<Notification> lastShowing = p->displayedNotifications[loc+showing-1];
                deleteFromVisible(loc+showing-1);
                insertToVisible(n, loc+i+1);
                p->snapQueue.push_front(lastShowing);
                replaced = true;
                break;
            }
        }

        if(!replaced) {
            p->snapQueue.push_back(n);
        }
        qStableSort(p->snapQueue.begin(), p->snapQueue.end(), notificationCompare);
        Q_EMIT queueSizeChanged(queued());
    } else {
        bool inserted = false;
        int loc = findFirst(Notification::Type::SnapDecision);
        for(int i=0; i<showing; i++) {
            if(p->displayedNotifications[loc+i]->getUrgency() > n->getUrgency()) {
                insertToVisible(n, loc+i+1);
                inserted = true;
                break;
            }
        }

        if (!inserted) {
            insertToVisible(n, showingNotificationOfType(Notification::Type::Confirmation) ? 1 : 0);
        }
    }
}

int NotificationModel::insertionPoint(const QSharedPointer<Notification> &n) const {
    if(n->getType() == Notification::Type::SnapDecision) {
        int loc = findFirst(Notification::Type::SnapDecision);
        int numSnaps = countShowing(Notification::Type::SnapDecision);
        for(int i=0; i<numSnaps; i++) {
            if(p->displayedNotifications[loc+i]->getUrgency() < n->getUrgency()) {
                return loc+i;
            }
        }
        return loc+numSnaps;
    } else {
        int i=0;
        for(; i<p->displayedNotifications.size(); i++) {
            if(p->displayedNotifications[i]->getType() > n->getType()) {
                return i+1;
            }
        }
        return i;
    }
}

void NotificationModel::insertToVisible(const QSharedPointer<Notification> &n, int location) {
    if(location < 0)
        location = p->displayedNotifications.size();
    if(location > p->displayedNotifications.size()) {
        printf("Bad insert.\n");
        return;
    }
    beginInsertRows(QModelIndex(), location, location);
    p->displayedNotifications.insert(location, n);
    endInsertRows();
    p->displayTimes[n->getID()] = 0;
}

Notification* NotificationModel::getRaw(const unsigned int notificationId) const {
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getID() == notificationId) {
            Notification* n = p->displayedNotifications[i].data();
            QQmlEngine::setObjectOwnership(n, QQmlEngine::CppOwnership);
            return n;
        }
    }

    return nullptr;
}

int NotificationModel::queued() const {
    return p->ephemeralQueue.size() + p->interactiveQueue.size() + p->snapQueue.size();
}

bool NotificationModel::showingNotificationOfType(const Notification::Type type) const {
    return countShowing(type) > 0;
}

bool NotificationModel::showingNotification(const NotificationID id) const {
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getID() == id) {
            return true;
        }
    }
    return false;
}

int NotificationModel::countShowing(const Notification::Type type) const {
    int count = 0;
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getType() == type) {
            count++;
        }
    }
    return count;
}

int NotificationModel::numNotifications() const {
    return queued() + p->displayedNotifications.size();
}

void NotificationModel::incrementDisplayTimes(const int displayedTime) const {
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        p->displayTimes[p->displayedNotifications[i]->getID()] += displayedTime;
    }
}

int NotificationModel::findFirst(const Notification::Type type) const {
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getType() == type)
            return i;
    }
    return -1;
}

QHash<int, QByteArray> NotificationModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles.insert(ModelInterface::RoleType, "type");
    roles.insert(ModelInterface::RoleUrgency, "urgency");
    roles.insert(ModelInterface::RoleId, "id");
    roles.insert(ModelInterface::RoleSummary, "summary");
    roles.insert(ModelInterface::RoleBody, "body");
    roles.insert(ModelInterface::RoleValue, "value");
    roles.insert(ModelInterface::RoleIcon, "icon");
    roles.insert(ModelInterface::RoleSecondaryIcon, "secondaryIcon");
    roles.insert(ModelInterface::RoleActions, "actions");
    roles.insert(ModelInterface::RoleHints, "hints");
    roles.insert(ModelInterface::RoleNotification, "notification");

    return roles;
}

void NotificationModel::notificationUpdated(const NotificationID id) {
    if(showingNotification(id)) {
        incrementDisplayTimes(p->timer.interval() - p->timer.remainingTime());
        p->timer.stop();
        p->displayTimes[id] = 0;
        int timeout = nextTimeout();
        Q_ASSERT(timeout > 0);
        p->timer.setInterval(timeout);
        p->timer.start();
    }
}

void NotificationModel::onDataChanged(unsigned int id) {
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getID() == id) {
            Q_EMIT dataChanged(index(i, 0), index(i, 0));
            break;
        }
    }
}
