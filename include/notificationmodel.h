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

#ifndef NOTIFICATIONMODEL_H
#define NOTIFICATIONMODEL_H

#include<QAbstractListModel>
#include<QSharedPointer>
#include<QScopedPointer>
#include "notify-backend.h"

class Notification;

struct NotificationModelPrivate;

class NotificationModel : public QAbstractListModel {
    Q_OBJECT

public:
    static const int maxNotifications = 50;
    static const int maxSnapsShown = 5;

    NotificationModel(QObject *parent=nullptr);
    virtual ~NotificationModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &parent, int role) const;

    void insertNotification(QSharedPointer<Notification> n);

    Q_INVOKABLE int queued() const;
    Q_INVOKABLE int numNotifications() const;
    Q_INVOKABLE void removeNotification(const NotificationID id);
    Q_INVOKABLE QString tempHackGetData() const;

private slots:
    void timeout();

signals:

    void queueSizeChanged(int newSize);

private:
    QScopedPointer<NotificationModelPrivate> p;

    bool nonSnapTimeout();
    int nextTimeout() const;
    void incrementDisplayTimes(const int displayedTime) const;
    void pruneExpired();
    void removeNonSnap();

    int insertionPoint(const QSharedPointer<Notification> n) const;
    void insertAsync(QSharedPointer<Notification> n);
    void insertSync(QSharedPointer<Notification> n);
    void insertInteractive(QSharedPointer<Notification> n);
    void insertSnap(QSharedPointer<Notification> n);
    void insertToVisible(QSharedPointer<Notification> n, int location=-1);
    void deleteFromVisible(int loc);
    void deleteFirst();
    int findFirst(const NotificationType type) const;
    bool showingNotificationOfType(const NotificationType type) const;
    int countShowing(const NotificationType type) const;
};

#endif
