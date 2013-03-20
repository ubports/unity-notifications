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

struct NotificationModelPrivate {
    QList<Notification*> notifications;
    QTimer timer;
};

NotificationModel::NotificationModel(QObject *parent) : QAbstractListModel(parent) {
    p = new NotificationModelPrivate;
    connect(&(p->timer), SIGNAL(timeout()), this, SLOT(timeout()));
}

NotificationModel::~NotificationModel() {
    for(int i=0; i<p->notifications.size(); i++)
        delete p->notifications[i];
    p->notifications.clear();
    delete p;
}

int NotificationModel::rowCount(const QModelIndex &parent) const {
    //printf("Count %d\n", notifications.size());
    return p->notifications.size();
}

QVariant NotificationModel::data(const QModelIndex &parent, int role) const {
    //printf("Data %d.\n", parent.row());
    if (!parent.isValid())
            return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();
    return QVariant(p->notifications[parent.row()]->getText());
}

void NotificationModel::insertNotification(Notification *n) {
    int loc = p->notifications.size();
    QModelIndex insertionPoint = QAbstractItemModel::createIndex(loc, 0);
    beginInsertRows(insertionPoint, loc, loc);
    p->notifications.push_back(n);
    endInsertRows();
    p->timer.stop();
    p->timer.setInterval(nextTimeout());
    p->timer.start();
}

void NotificationModel::deleteFirst() {
    if(p->notifications.empty())
        return;
    int loc = 0;
    QModelIndex deletePoint = QAbstractItemModel::createIndex(loc, 0);
    beginRemoveRows(deletePoint, loc, loc);
    delete p->notifications[loc];
    p->notifications.pop_front();
    endRemoveRows();
}

void NotificationModel::timeout() {
    p->timer.stop();
    deleteFirst();
    if(p->notifications.size() > 0) {
        p->timer.setInterval(nextTimeout());
        p->timer.start();
    }
}

int NotificationModel::nextTimeout() const {
    return 5000;
}
