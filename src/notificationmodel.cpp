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
    QList<Notification*> displayedNotifications;
    QTimer timer;
    QVector<Notification*> asyncQueue;
};

NotificationModel::NotificationModel(QObject *parent) : QAbstractListModel(parent) {
    p = new NotificationModelPrivate;
    connect(&(p->timer), SIGNAL(timeout()), this, SLOT(timeout()));
}

NotificationModel::~NotificationModel() {
    for(int i=0; i<p->displayedNotifications.size(); i++)
        delete p->displayedNotifications[i];
    p->displayedNotifications.clear();
    delete p;
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

void NotificationModel::insertNotification(Notification *n) {
    switch(n->getType()) {
    case ASYNCHRONOUS : insertAsync(n); break;
    case SYNCHRONOUS : insertSync(n); break;
    default:
        printf("Insert not implemented for this type.\n");
        delete n;
    }
}

void NotificationModel::deleteFirst() {
    if(p->displayedNotifications.empty())
        return;
    int loc = 0;
    QModelIndex deletePoint = QAbstractItemModel::createIndex(loc, 0);
    beginRemoveRows(deletePoint, loc, loc);
    delete p->displayedNotifications[loc];
    p->displayedNotifications.pop_front();
    endRemoveRows();
}

void NotificationModel::timeout() {
    bool restartTimer = false;
    p->timer.stop();
    deleteFirst();

    if(p->displayedNotifications.empty()) {
        if(!p->asyncQueue.empty()) {
            Notification *n = p->asyncQueue[0];
            p->asyncQueue.pop_front();
            insertToVisible(n);
            restartTimer = true;
            emit queueSizeChanged(queued());
        }
    } else {
        restartTimer = true;
    }
    if(restartTimer) {
        p->timer.setInterval(nextTimeout());
        p->timer.start();
    }
}

int NotificationModel::nextTimeout() const {
    return 5000;
}

void NotificationModel::insertAsync(Notification *n) {
    Q_ASSERT(n->getType() == ASYNCHRONOUS);
    if(p->displayedNotifications.size() == 0) {
        insertToVisible(n);
    } else {
        p->asyncQueue.push_back(n);
        emit queueSizeChanged(queued());
    }
}

void NotificationModel::insertSync(Notification *n) {
    printf("Fooo\n");
    if(showingNotificationOfType(SYNCHRONOUS)) {
        deleteFirst(); // Synchronous is always first.
    }
    insertToVisible(n, 0);
}

void NotificationModel::insertToVisible(Notification *n, int location) {
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
    return p->asyncQueue.size();
}

bool NotificationModel::showingNotificationOfType(const NotificationType type) const {
    for(int i=0; i<p->displayedNotifications.size(); i++) {
        if(p->displayedNotifications[i]->getType() == type) {
            return true;
        }
    }
    return false;
}
