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

#include"notify-backend.h"
#include<QObject>
#include<QDBusInterface>

class NotificationClient : public QObject {
    Q_OBJECT

public:
    NotificationClient(QObject *parent=0);
    ~NotificationClient();
    unsigned int sendNotification(NotificationType ntype, Urgency urg, QString text);

public slots:
    void NotificationClosed(unsigned int id, unsigned int reason);
    void ActionInvoked(unsigned int id, QString key);

private:
    QDBusInterface service;

};

#endif
