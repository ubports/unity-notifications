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
#include "notification.h"
#include<QTimer>
#include<QList>
#include<QVector>
#include<QMap>

struct NotificationModelPrivate {
    QList<QSharedPointer<Notification> > displayedNotifications;
    QTimer timer;
    QVector<QSharedPointer<Notification> > asyncQueue;
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
}

int NotificationModel::rowCount(const QModelIndex &parent) const {
    //printf("Count %d\n", displayedNotifications.size());
    return p->displayedNotifications.size();
}

QVariant NotificationModel::data(const QModelIndex &parent, int role) const {
    //printf("Data %d.\n", parent.row());
    if (!parent.isValid())
            return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();
    return QVariant(p->displayedNotifications[parent.row()]->getBody());
}

Q_INVOKABLE QString NotificationModel::tempHackGetData() const {
    return p->displayedNotifications[0]->getBody();
}

void NotificationModel::insertNotification(QSharedPointer<Notification> n) {
    if(numNotifications() >= maxNotifications)
        return; // Just ignore. Maybe we should throw()?
    int remaining = p->timer.remainingTime();
    int elapsed = p->timer.interval() - remaining;
    p->timer.stop();
    incrementDisplayTimes(elapsed);
    switch(n->getType()) {
    case Notification::Type::Ephemeral : insertAsync(n); break;
    case Notification::Type::Confirmation : insertSync(n); break;
    case Notification::Type::Interactive : insertInteractive(n); break;
    case Notification::Type::SnapDecision : insertSnap(n); break;
    default:
        printf("Unknown notification type, I should probably throw an exception here.\n");
        break;
    }
    int timeout = nextTimeout();
    Q_ASSERT(timeout > 0);
    p->timer.setInterval(timeout);
    p->timer.start();
}

void NotificationModel::removeNotification(const NotificationID id) {
    for(int i=0; i<p->asyncQueue.size(); i++) {
        if(p->asyncQueue[i]->getID() == id) {
            p->asyncQueue.erase(p->asyncQueue.begin() + i);
            emit queueSizeChanged(queued());
            return;
        }
    }

    for(int i=0; i<p->snapQueue.size(); i++) {
        if(p->snapQueue[i]->getID() == id) {
            p->snapQueue.erase(p->snapQueue.begin() + i);
            emit queueSizeChanged(queued());
            return;
        }
    }

    for(int i=0; i<p->interactiveQueue.size(); i++) {
        if(p->interactiveQueue[i]->getID() == id) {
            p->interactiveQueue.erase(p->interactiveQueue.begin() + i);
            emit queueSizeChanged(queued());
            return;
        }
    }

    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getID() == id) {
            deleteFromVisible(i);
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

void NotificationModel::deleteFromVisible(int loc) {
    QModelIndex deletePoint = QAbstractItemModel::createIndex(loc, 0);
    beginRemoveRows(deletePoint, loc, loc);
    QSharedPointer<Notification> n = p->displayedNotifications[loc];
    p->displayTimes.erase(p->displayTimes.find(n->getID()));
    p->displayedNotifications.erase(p->displayedNotifications.begin() + loc);
    endRemoveRows();
}

void NotificationModel::timeout() {
    bool restartTimer = false;
    incrementDisplayTimes(p->timer.interval());
    pruneExpired();
    if(!p->displayedNotifications.empty()) {
        restartTimer = true;
    }
    // Snap decisions override everything.
    if(showingNotificationOfType(Notification::Type::SnapDecision) || !p->snapQueue.empty()) {
        if(countShowing(Notification::Type::SnapDecision) < maxSnapsShown && !p->snapQueue.empty()) {
            QSharedPointer<Notification> n = p->snapQueue[0];
            p->snapQueue.pop_front();
            insertToVisible(n, insertionPoint(n));
            restartTimer = true;
            emit queueSizeChanged(queued());
        }
    } else {
        restartTimer = nonSnapTimeout();
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
        QSharedPointer<Notification> n = p->interactiveQueue[0];
        p->interactiveQueue.pop_front();
        insertToVisible(n, insertionPoint(n));
        restartTimer = true;
        emit queueSizeChanged(queued());
    }
    if(!showingNotificationOfType(Notification::Type::Ephemeral) && !p->asyncQueue.empty()) {
        QSharedPointer<Notification> n = p->asyncQueue[0];
        p->asyncQueue.pop_front();
        insertToVisible(n, insertionPoint(n));
        restartTimer = true;
        emit queueSizeChanged(queued());
    }
    return restartTimer;
}
void NotificationModel::pruneExpired() {
    for(int i=p->displayedNotifications.size()-1; i>=0; i--) {
        QSharedPointer<Notification> n = p->displayedNotifications[i];
        int shownTime = p->displayTimes[n->getID()];
        if(shownTime >= n->getDisplayTime()) {
            deleteFromVisible(i);
        }
    }
}

void NotificationModel::removeNonSnap() {
    for(int i=p->displayedNotifications.size()-1; i>=0; i--) {
        QSharedPointer<Notification> n = p->displayedNotifications[i];
        switch(n->getType()) {
        case Notification::Type::SnapDecision : break;
        case Notification::Type::Confirmation : deleteFromVisible(i); break;
        case Notification::Type::Ephemeral : deleteFromVisible(i); p->asyncQueue.push_front(n); queueSizeChanged(queued()); break;
        case Notification::Type::Interactive : deleteFromVisible(i); p->interactiveQueue.push_front(n); queueSizeChanged(queued()); break;
        case Notification::Type::PlaceHolder : break;
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
        int shownTime = p->displayTimes[n->getID()];
        int remainingTime = totalTime - shownTime;
        if(remainingTime < 0)
            remainingTime = 0;
        if(remainingTime < mintime)
            mintime = remainingTime;
    }
    return mintime;
}

void NotificationModel::insertAsync(QSharedPointer<Notification> n) {
    Q_ASSERT(n->getType() == Notification::Type::Ephemeral);

    if(showingNotificationOfType(Notification::Type::SnapDecision)) {
        p->asyncQueue.push_back(n);
        qStableSort(p->asyncQueue.begin(), p->asyncQueue.end(), notificationCompare);
        emit queueSizeChanged(queued());
    } else if(showingNotificationOfType(Notification::Type::Ephemeral)) {
        int loc = findFirst(Notification::Type::Ephemeral);
        QSharedPointer<Notification> showing = p->displayedNotifications[loc];
        if(n->getUrgency() > showing->getUrgency()) {
            deleteFromVisible(loc);
            insertToVisible(n, loc);
            p->asyncQueue.push_front(showing);
        } else {
            p->asyncQueue.push_back(n);
        }
        qStableSort(p->asyncQueue.begin(), p->asyncQueue.end(), notificationCompare);
        emit queueSizeChanged(queued());
    } else {
        insertToVisible(n);
    }
}

void NotificationModel::insertInteractive(QSharedPointer<Notification> n) {
    Q_ASSERT(n->getType() == Notification::Type::Interactive);

    if(showingNotificationOfType(Notification::Type::SnapDecision)) {
        p->interactiveQueue.push_back(n);
        qStableSort(p->interactiveQueue.begin(), p->interactiveQueue.end(), notificationCompare);
        emit queueSizeChanged(queued());
    } else if(showingNotificationOfType(Notification::Type::Interactive)) {
        int loc = findFirst(Notification::Type::Interactive);
        QSharedPointer<Notification> showing = p->displayedNotifications[loc];
        if(n->getUrgency() > showing->getUrgency()) {
            deleteFromVisible(loc);
            insertToVisible(n, loc);
            p->interactiveQueue.push_front(showing);
        } else {
            p->interactiveQueue.push_back(n);
        }
        qStableSort(p->interactiveQueue.begin(), p->interactiveQueue.end(), notificationCompare);
        emit queueSizeChanged(queued());
    } else {
        int loc = insertionPoint(n);
        insertToVisible(n, loc);
    }

}


void NotificationModel::insertSync(QSharedPointer<Notification> n) {
    Q_ASSERT(n->getType() == Notification::Type::Confirmation);
    if(showingNotificationOfType(Notification::Type::Confirmation)) {
        deleteFirst(); // Synchronous is always first.
    }
    insertToVisible(n, 0);
}


void NotificationModel::insertSnap(QSharedPointer<Notification> n) {
    Q_ASSERT(n->getType() == Notification::Type::SnapDecision);
    removeNonSnap();
    int showing = countShowing(n->getType());
    if(showing >= maxSnapsShown) {
        int loc = findFirst(Notification::Type::SnapDecision);
        bool replaced = false;
        for(int i=0; i<showing; i++) {
            if(p->displayedNotifications[loc+i]->getUrgency() < n->getUrgency()) {
                QSharedPointer<Notification> lastShowing = p->displayedNotifications[loc+showing-1];
                deleteFromVisible(loc+showing-1);
                insertToVisible(n, loc+i);
                p->snapQueue.push_front(lastShowing);
                replaced = true;
                break;
            }
        }
        if(!replaced) {
            p->snapQueue.push_back(n);
        }
        qStableSort(p->snapQueue.begin(), p->snapQueue.end(), notificationCompare);
        emit queueSizeChanged(queued());
    } else {
        int loc = insertionPoint(n);
        insertToVisible(n, loc);
    }
}

int NotificationModel::insertionPoint(const QSharedPointer<Notification> n) const {
    int i=0;
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
        for(; i<p->displayedNotifications.size(); i++) {
            if(p->displayedNotifications[i]->getType() > n->getType()) {
                break;
            }
        }
        return i;
    }
}

void NotificationModel::insertToVisible(QSharedPointer<Notification> n, int location) {
    if(location < 0)
        location = p->displayedNotifications.size();
    if(location > p->displayedNotifications.size()) {
        printf("Bad insert.\n");
        return;
    }
    QModelIndex insertionPoint = QAbstractItemModel::createIndex(location, 0);
    beginInsertRows(insertionPoint, location, location);
    p->displayedNotifications.insert(location, n);
    endInsertRows();
    p->displayTimes[n->getID()] = 0;
}

int NotificationModel::queued() const {
    return p->asyncQueue.size() + p->interactiveQueue.size() + p->snapQueue.size();
}

bool NotificationModel::showingNotificationOfType(const Notification::Type type) const {
    return countShowing(type) > 0;
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
