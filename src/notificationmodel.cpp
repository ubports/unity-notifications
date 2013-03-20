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
#include "notification.hpp"
#include<QTimer>
#include<QList>
#include<QVector>

struct NotificationModelPrivate {
    QList<QSharedPointer<Notification> > displayedNotifications;
    QTimer timer;
    QVector<QSharedPointer<Notification> > asyncQueue;
    QVector<QSharedPointer<Notification> > interactiveQueue;
};

NotificationModel::NotificationModel(QObject *parent) : QAbstractListModel(parent), p(new NotificationModelPrivate) {
    connect(&(p->timer), SIGNAL(timeout()), this, SLOT(timeout()));
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
    return QVariant(p->displayedNotifications[parent.row()]->getText());
}

void NotificationModel::insertNotification(QSharedPointer<Notification> n) {
    switch(n->getType()) {
    case ASYNCHRONOUS : insertAsync(n); break;
    case SYNCHRONOUS : insertSync(n); break;
    case INTERACTIVE : insertInteractive(n); break;
    default:
        printf("Insert not implemented for this type.\n");
    }
}

void NotificationModel::deleteFirst() {
    if(p->displayedNotifications.empty())
        return;
    int loc = 0;
    QModelIndex deletePoint = QAbstractItemModel::createIndex(loc, 0);
    beginRemoveRows(deletePoint, loc, loc);
    p->displayedNotifications.pop_front();
    endRemoveRows();
}

void NotificationModel::timeout() {
    bool restartTimer = false;
    p->timer.stop();
    deleteFirst();
    if(p->displayedNotifications.size() != 0) {
        restartTimer = true;
    }

    if(!showingNotificationOfType(INTERACTIVE) && !p->interactiveQueue.empty()) {
        QSharedPointer<Notification> n = p->interactiveQueue[0];
        p->interactiveQueue.pop_front();
        insertToVisible(n, insertPoint(n));
        restartTimer = true;
        emit queueSizeChanged(queued());
    }
    if(!showingNotificationOfType(ASYNCHRONOUS) && !p->asyncQueue.empty()) {
        QSharedPointer<Notification> n = p->asyncQueue[0];
        p->asyncQueue.pop_front();
        insertToVisible(n, insertPoint(n));
        restartTimer = true;
        emit queueSizeChanged(queued());
    }
    if(restartTimer) {
        p->timer.setInterval(nextTimeout());
        p->timer.start();
    }
}

int NotificationModel::nextTimeout() const {
    return 5000;
}

void NotificationModel::insertAsync(QSharedPointer<Notification> n) {
    Q_ASSERT(n->getType() == ASYNCHRONOUS);
    if(showingNotificationOfType(ASYNCHRONOUS) == 0) {
        insertToVisible(n);
    } else {
        p->asyncQueue.push_back(n);
        emit queueSizeChanged(queued());
    }
}

void NotificationModel::insertInteractive(QSharedPointer<Notification> n) {
    Q_ASSERT(n->getType() == INTERACTIVE);
    if(showingNotificationOfType(INTERACTIVE)) {
        p->interactiveQueue.push_back(n);
        emit queueSizeChanged(queued());
    } else {
        int loc = insertPoint(n);
        insertToVisible(n, loc);
    }

}


void NotificationModel::insertSync(QSharedPointer<Notification> n) {
    Q_ASSERT(n->getType() == SYNCHRONOUS);
    if(showingNotificationOfType(SYNCHRONOUS)) {
        deleteFirst(); // Synchronous is always first.
    }
    insertToVisible(n, 0);
}

int NotificationModel::insertPoint(const QSharedPointer<Notification> n) const {
    int i=0;
    for(; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getType() > n->getType()) {
            break;
        }
    }
    return i;
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
    p->timer.stop();
    p->timer.setInterval(nextTimeout());
    p->timer.start();
}

int NotificationModel::queued() const {
    return p->asyncQueue.size() + p->interactiveQueue.size();
}

bool NotificationModel::showingNotificationOfType(const NotificationType type) const {
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getType() == type) {
            return true;
        }
    }
    return false;
}
