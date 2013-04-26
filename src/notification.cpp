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
    Notification::Urgency urg;
    QString summary;
    QString body;
    int value;
    Notification::Type type;
    NotificationServer *server;
    QString icon;
    QString secondaryIcon;
    QStringList actions;
    int hints;
    int displayTime;
};

/*
 * This constructor really should not exist, but
 * QML requires it.
 */

Notification::Notification(QObject *parent) : QObject(parent), p(new NotificationPrivate()) {
    p->id = (NotificationID) -1;
    p->urg = Notification::Urgency::Low;
    p->body = "default text";
    p->server = nullptr;
    p->value = -2;
    p->hints = Notification::Hint::None;
}

Notification::Notification(NotificationID id, int displayTime, const Urgency ur, QString text, Type type, NotificationServer *srv, QObject *parent) :
                QObject(parent), p(new NotificationPrivate()) {
    p->id = id;
    p->urg = ur;
    p->body = text;
    p->type = type;
    p->server = srv;
    p->value = -2;
    p->hints = Notification::Hint::None;
    p->displayTime = displayTime;
}

Notification::Notification(NotificationID id, int displayTime, const Urgency ur, Type type, NotificationServer *srv, QObject *parent) :
    Notification(id, displayTime, ur, "", type, srv, parent){

}

Notification::~Notification() {
    if(p->server)
        p->server->CloseNotification(p->id, 0);
}

QString Notification::getBody() const {
    return p->body;
}

void Notification::setBody(const QString text) {
    if(p->body != text) {
        p->body = text;
        emit bodyChanged(text);
    }
}

NotificationID Notification::getID() const {
    return p->id;
}

Notification::Type Notification::getType() const {
    return p->type;
}

int Notification::getDisplayTime() const {
    return p->displayTime;
}

bool Notification::operator<(const Notification &n) const {
    if(p->type < n.p->type)
        return true;
    if(p->type > n.p->type)
        return false;
    return p->urg > n.p->urg;
}

QString Notification::getIcon() const {
    return p->icon;
}

void Notification::setIcon(QString icon) {
    p->icon = icon;
    emit iconChanged(p->icon);
}

QString Notification::getSecondaryIcon() const {
    return p->secondaryIcon;
}

void Notification::setSecondaryIcon(QString secondaryIcon) {
    p->secondaryIcon = secondaryIcon;
    emit secondaryIconChanged(p->secondaryIcon);
}

QString Notification::getSummary() const {
    return p->summary;
}

void Notification::setSummary(QString summary) {
    if(p->summary != summary) {
        p->summary = summary;
        emit summaryChanged(p->summary);
    }
}

int Notification::getValue() const {
    return p->value;
}

void Notification::setValue(int value) {
    p->value = value;
    emit valueChanged(p->value);
}

Notification::Urgency Notification::getUrgency() const {
    return p->urg;
}
void Notification::setUrgency(Notification::Urgency urg) {
    if(p->urg != urg) {
        p->urg = urg;
        emit urgencyChanged(p->urg);
    }
}

void Notification::setType(Type type) {
    if(p->type != p->type) {
        p->type = type;
        emit typeChanged(p->type);
    }
}

QStringList Notification::getActions() const {
    return p->actions;
}

void Notification::setActions(QStringList actions) {
    if(p->actions != actions) {
        p->actions = actions;
        emit actionsChanged(p->actions);
    }
}

int Notification::getHints() const {
    return p->hints;
}

void Notification::setHints(int hints) {
    p->hints = hints;
    emit hintsChanged(p->hints);
}

void Notification::onHovered() {

}

void Notification::onDisplayed() {

}

void Notification::invokeAction(QString action) const {
    for(int i=0; i<p->actions.size(); i++) {
        if(p->actions[i] == action)
            p->server->invokeAction(p->id, action);
            return;
    }
    printf("Error: tried to invoke action not in actionList.\n");
}
