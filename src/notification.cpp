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
    QString summary;
    QString body;
    NotificationType type;
    NotificationServer *server;
    QImage icon;
    QStringList actions;
};

/*
 * This constructor really should not exist, but
 * QML requires it.
 */

Notification::Notification(QObject *parent) : QObject(parent), p(new NotificationPrivate()) {
    p->id = (NotificationID) -1;
    p->urg = URGENCY_LOW;
    p->body = "default text";
    p->server = nullptr;
}

Notification::Notification(NotificationID id, const Urgency ur, QString text, NotificationType type, NotificationServer *srv, QObject *parent) :
                QObject(parent), p(new NotificationPrivate()) {
    p->id = id;
    p->urg = ur;
    p->body = text;
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

QString Notification::getBody() const {
    return p->body;
}

void Notification::setBody(const QString text) {
    p->body = text;
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


QImage Notification::getIcon() const {
    return p->icon;
}

void Notification::setIcon(QImage icon) {
    p->icon = icon;
    emit iconChanged(p->icon);
}

QString Notification::getSummary() const {
    return p->summary;
}

void Notification::setSummary(QString summary) {
    p->summary = summary;
    emit summaryChanged(p->summary);
}

Urgency Notification::getUrgency() const {
    return p->urg;
}
void Notification::setUrgency(Urgency urg) {
    p->urg = urg;
    emit urgencyChanged(p->urg);
}

void Notification::setType(NotificationType type) {
    p->type = type;
    emit typeChanged(p->type);
}

QStringList Notification::getActions() const {
    return p->actions;
}

void Notification::setActions(QStringList actions) {
    p->actions = actions;
    emit actionsChanged(p->actions);
}
