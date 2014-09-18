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


#include "NotificationServer.h"
#include "Notification.h"
#include <string>
#include <QTextDocument>

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
    ActionModel* actionsModel;
    QVariantMap hints;
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
    p->actionsModel = new ActionModel();
}

Notification::Notification(NotificationID id, int displayTime, const Urgency ur, const QString &text, Type type, NotificationServer *srv, QObject *parent) :
                QObject(parent), p(new NotificationPrivate()) {
    p->id = id;
    p->urg = ur;
    p->body = text;
    p->type = type;
    p->server = srv;
    p->value = -2;
    p->displayTime = displayTime;
    p->actionsModel = new ActionModel();
}

Notification::Notification(NotificationID id, int displayTime, const Urgency ur, Type type, NotificationServer *srv, QObject *parent) :
    Notification(id, displayTime, ur, QString(), type, srv, parent){
    p->actionsModel = new ActionModel();
}

Notification::~Notification() {
    if(p->server)
        p->server->CloseNotification(p->id);
}

QString Notification::getBody() const {
    return p->body;
}

void Notification::setBody(const QString &text) {
    QString filtered = filterText(text);
    if(p->body != filtered) {
        p->body = filtered;
        Q_EMIT bodyChanged(p->body);
        Q_EMIT dataChanged(p->id);
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

void Notification::setIcon(const QString &icon) {
    if (icon.startsWith(" ") || icon.size() == 0) {
        p->icon = nullptr;
    }
    else {
        p->icon = icon;

        if (icon.indexOf("/") == -1) {
            p->icon.prepend("image://theme/");
        }
    }

    Q_EMIT iconChanged(p->icon);
    Q_EMIT dataChanged(p->id);
}

QString Notification::getSecondaryIcon() const {
    return p->secondaryIcon;
}

void Notification::setSecondaryIcon(const QString &secondaryIcon) {
    if (secondaryIcon.startsWith(" ") || secondaryIcon.size() == 0) {
        p->secondaryIcon = nullptr;
    }
    else {
        p->secondaryIcon = secondaryIcon;

        if (secondaryIcon.indexOf("/") == -1) {
            p->secondaryIcon.prepend("image://theme/");
        }
    }

    Q_EMIT secondaryIconChanged(p->secondaryIcon);
    Q_EMIT dataChanged(p->id);
}

QString Notification::getSummary() const {
    return p->summary;
}

void Notification::setSummary(const QString &summary) {
    QString filtered = filterText(summary);
    if(p->summary != filtered) {
        p->summary = filtered;
        Q_EMIT summaryChanged(p->summary);
        Q_EMIT dataChanged(p->id);
    }
}

int Notification::getValue() const {
    return p->value;
}

void Notification::setValue(int value) {
    p->value = value;
    Q_EMIT valueChanged(p->value);
    Q_EMIT dataChanged(p->id);
}

Notification::Urgency Notification::getUrgency() const {
    return p->urg;
}
void Notification::setUrgency(Notification::Urgency urg) {
    if(p->urg != urg) {
        p->urg = urg;
        Q_EMIT urgencyChanged(p->urg);
    }
}

void Notification::setType(Type type) {
    if(p->type != p->type) {
        p->type = type;
        Q_EMIT typeChanged(p->type);
    }
}

ActionModel* Notification::getActions() const {
    return p->actionsModel;
}

void Notification::setActions(const QStringList &actions) {
    if(p->actions != actions) {
        p->actions = actions;
        Q_EMIT actionsChanged(p->actions);

        for (int i = 0; i < p->actions.size(); i += 2) {
            p->actionsModel->insertAction(p->actions[i], p->actions[i+1]);
        }
    }
}

void Notification::detachFromServer() {
    /*
     * FIXME. This function should not exist. In fact
     * a notification should not need to know the server
     * it is connected to. Instead the way p->server
     * is used needs to be converted into a proper
     * Qt signal. Unfortunately, due to deadlines,
     * we did not have time. The next time this is worked
     * on, fix this properly.
     */
    p->server = nullptr;
}

QVariantMap Notification::getHints() const {
    return p->hints;
}

void Notification::setHints(const QVariantMap& hints) {
    if (p->hints != hints) {
        p->hints = hints;
        Q_EMIT hintsChanged(p->hints);
    }
}

void Notification::onHovered() {

}

void Notification::onDisplayed() {

}

void Notification::invokeAction(const QString &action) {
    for(int i=0; i<p->actions.size(); i++) {
        if(p->actions[i] == action) {
            p->server->invokeAction(p->id, action);
            Q_EMIT completed(p->id);
            return;
        }
    }
    fprintf(stderr, "Error: tried to invoke action not in actionList.\n");
}

void Notification::close() {
    Q_EMIT completed(p->id);
}

QString Notification::filterText(const QString& text) {
    QTextDocument filtered;
    filtered.setHtml(text);
    return filtered.toPlainText();
}
