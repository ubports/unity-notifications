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

#ifndef NOTIFICATIONMODEL_H
#define NOTIFICATIONMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QScopedPointer>
#include "notify-backend.h"
#include "Notification.h"

class Notification;

struct NotificationModelPrivate;

class NotificationModel : public QAbstractListModel {
    Q_OBJECT

public:
    static const int maxNotifications = MAX_NOTIFICATIONS;
    static const int maxSnapsShown = 5;

    NotificationModel(QObject *parent=nullptr);
    virtual ~NotificationModel();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    void insertNotification(const QSharedPointer<Notification> &n);
    QList<QSharedPointer<Notification>> getAllNotifications() const;
    QSharedPointer<Notification> getNotification(NotificationID id) const;
    QSharedPointer<Notification> getNotification(const QString &summary) const;
    QSharedPointer<Notification> getDisplayedNotification(int index) const;
    bool hasNotification(NotificationID id) const;
    QList<QSharedPointer<Notification>> removeAllNotificationsForClient(const QString& clientId);

    // getRaw() is only meant to be used from QML, since QML cannot handle
    // QSharedPointers... on C++-side only use getNotification()
    Q_INVOKABLE Notification* getRaw(const unsigned int notificationId) const;

    Q_INVOKABLE int queued() const;
    Q_INVOKABLE int numNotifications() const;
    Q_INVOKABLE void removeNotification(const NotificationID id);

    bool showingNotificationOfType(const Notification::Type type) const;
    bool showingNotification(const NotificationID id) const;

    void notificationUpdated(const NotificationID id);

private Q_SLOTS:
    void timeout();
    void onDataChanged(unsigned int id);

Q_SIGNALS:
    void queueSizeChanged(int newSize);

private:
    QScopedPointer<NotificationModelPrivate> p;

    bool nonSnapTimeout();
    int nextTimeout() const;
    void incrementDisplayTimes(const int displayedTime) const;
    void pruneExpired();
    void removeNonSnap();

    int insertionPoint(const QSharedPointer<Notification> &n) const;
    void insertEphemeral(const QSharedPointer<Notification> &n);
    void insertConfirmation(const QSharedPointer<Notification> &n);
    void insertInteractive(const QSharedPointer<Notification> &n);
    void insertSnap(const QSharedPointer<Notification> &n);
    void insertExtSnap(const QSharedPointer<Notification> &n);
    void insertToVisible(const QSharedPointer<Notification> &n, int location=-1);
    QSharedPointer<Notification> deleteFromVisible(int loc);
    void deleteFirst();
    int findFirst(const Notification::Type type) const;
    int countShowing(const Notification::Type type) const;
};

#endif
