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
#include <algorithm>

using namespace std;

struct NotificationBackendPrivate {
    vector<Notification*> notifications;
};

struct less_mag : public binary_function<Notification*, Notification*, bool> {
    bool operator()(const Notification *x, const Notification *y) const { return *x < *y; }
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
    reorder();
    return true;
}

const Notification& NotificationBackend::getNotification(size_t i) const {
    if(i >= p->notifications.size())
        throw out_of_range("Tried to access a non-existing notification.");
    return *(p->notifications[i]);
}

void NotificationBackend::deleteNotification(const Notification *n) {
    for(vector<Notification*>::iterator it = p->notifications.begin();
            it != p->notifications.end(); ++it)
        if(*it == n) {
            delete *it;
            p->notifications.erase(it);
            return;
        }
    throw runtime_error("Tried to remove Notification not in NotificationBackend.");
}

bool NotificationBackend::hasNotification(const NotificationID id) const {
    try {
        getNotificationByID(id);
        return true;
    } catch(const out_of_range &e) {
    }
    return false;
}

const Notification& NotificationBackend::getNotificationByID(const NotificationID id) const {
    for(vector<Notification*>::const_iterator it = p->notifications.begin();
            it != p->notifications.end(); ++it)
        if((*it)->getID() == id) {
            return **it;
        }
    throw out_of_range("Tried to get Notification not in the queue.");
}

void NotificationBackend::reorder() {
    sort(p->notifications.begin(),
            p->notifications.end(),
            less_mag());
}
