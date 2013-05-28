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

#ifndef NOTIFICATIONCLIENT_H_
#define NOTIFICATIONCLIENT_H_

#include "notify-backend.h"
#include "notification.h"
#include <QObject>
#include <QDBusInterface>

class ClientMainWindow;

class NotificationClient : public QObject {
    Q_OBJECT

public:
    NotificationClient(QObject *parent=0);
    ~NotificationClient();
    NotificationID sendNotification(Notification::Type ntype, Notification::Urgency urg, QString summary, QString body);
    NotificationID appendText(NotificationID id, QString text);

public slots:
    /* These slots are needed to catch the incoming DBus messages. */
    void NotificationClosed(NotificationID id, unsigned int reason);
    void ActionInvoked(NotificationID id, QString key);

signals:
    /* These signals are for client applications to bind to. */
    void closed(NotificationID id, unsigned int reason);
    void invoked(NotificationID id, QString key);

    /* This is a temporary for development purposes. */
    void eventHappened(QString text);

private:
    QDBusInterface service;

};

#endif
