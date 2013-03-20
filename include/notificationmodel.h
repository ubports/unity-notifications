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

class Notification;

struct NotificationModelPrivate;

class NotificationModel : public QAbstractListModel {
    Q_OBJECT

public:
    NotificationModel(QObject *parent=nullptr);
    virtual ~NotificationModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &parent, int role) const;

    void insertNotification(Notification *n);
    void deleteFirst();

    int queued() const;

private slots:
    void timeout();

signals:

    void queueSizeChanged(int newSize);

private:
    NotificationModelPrivate *p;

    int nextTimeout() const;

    void insertAsync(Notification *n);
    void insertToVisible(Notification *n);
};

#endif
