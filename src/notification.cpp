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


#include "notificationserver.h"
#include "notification.h"
#include <string>

using namespace std;

struct NotificationPrivate {
    NotificationID id;
    Urgency urg;
    QString text;
    NotificationType type;
    NotificationServer *server;
};

/*
 * This constructor really should not exist, but
 * QML requires it.
 */

Notification::Notification(QObject *parent) : QObject(parent), p(new NotificationPrivate()) {
    p->id = (NotificationID) -1;
    p->urg = URGENCY_LOW;
    p->text = "default text";
    p->server = nullptr;
}

Notification::Notification(NotificationID id, const Urgency ur, QString text, NotificationType type, NotificationServer *srv, QObject *parent) :
                QObject(parent), p(new NotificationPrivate()) {
    p->id = id;
    p->urg = ur;
    p->text = text;
    p->type = type;
    p->server = srv;
}

Notification::Notification(NotificationID id, const Urgency ur, NotificationType type, NotificationServer *srv, QObject *parent) :
    Notification(id, ur, "", type, srv, parent){

}

Notification::~Notification() {
    if(p->server)
        p->server->CloseNotification(p->id, 0);
}

Urgency Notification::getUrgency() const {
    return p->urg;
}

QString Notification::getBody() const {
    return p->text;
}

void Notification::setBody(const QString text) {
    p->text = text;
    emit bodyChanged(text);
}

NotificationID Notification::getID() const {
    return p->id;
}

NotificationType Notification::getType() const {
    return p->type;
}

int Notification::getDisplayTime() const {
    return 5000;
}

bool Notification::operator<(const Notification &n) const {
    if(p->type < n.p->type)
        return true;
    if(p->type > n.p->type)
        return false;
    return p->urg > n.p->urg;
}

