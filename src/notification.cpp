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


#include "notification.hpp"
#include <string>

using namespace std;

struct NotificationPrivate {
    NotificationID id;
    Urgency urg;
    QString text;
    NotificationType type;
};

Notification::Notification(NotificationID id, const Urgency ur, QString text, NotificationType type) {
    p = new NotificationPrivate();
    p->id = id;
    p->urg = ur;
    p->text = text;
    p->type = type;
}

Notification::~Notification() {
    delete p;
}

Urgency Notification::getUrgency() const {
    return p->urg;
}

QString Notification::getText() const {
    return p->text;
}

NotificationID Notification::getID() const {
    return p->id;
}

NotificationType Notification::getType() const {
    return p->type;
}

bool Notification::operator<(const Notification &n) const {
    if(p->type < n.p->type)
        return true;
    if(p->type > n.p->type)
        return false;
    return p->urg > n.p->urg;
}

