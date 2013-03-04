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

#include "notificationbackend.hpp"
#include "notification.hpp"
#include <vector>
#include <stdexcept>

using namespace std;

struct NotificationBackendPrivate {
    vector<Notification*> notifications;
};

NotificationBackend::NotificationBackend() {
    p = new NotificationBackendPrivate();
}

NotificationBackend::~NotificationBackend() {
    for(size_t i=0; i < p->notifications.size(); i++)
        delete p->notifications[i];
    delete p;
}

size_t NotificationBackend::numNotifications() const {
    return p->notifications.size();
}

bool NotificationBackend::hasNotification(const Notification *n) const {
    for(size_t i=0; i < p->notifications.size(); i++)
        if(p->notifications[i] == n)
            return true;
    return false;
}

// Takes ownership of the argument.
bool NotificationBackend::insertNotification(Notification *n) {
    if(hasNotification(n)) {
        throw runtime_error("Tried to reinsert a notification that is already in the system");
    }
    if(p->notifications.size() >= MAX_NOTIFICATIONS) {
        delete n;
        return false;
    }
    p->notifications.push_back(n);
    return true;
}

const Notification& NotificationBackend::getNotification(size_t i) const {
    if(i >= p->notifications.size())
        throw out_of_range("Tried to access a non-existing notification.");
    return *(p->notifications[i]);
}
