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

#ifndef NOTIFICATIONBACKEND_HPP_
#define NOTIFICATIONBACKEND_HPP_

#include "notify-backend.hpp"

struct NotificationBackendPrivate;

class NotificationBackend {
private:

    NotificationBackendPrivate *p;
    Renderer &r; // Remove this once updates are sent with signals.

    void reorder();

public:
    NotificationBackend(Renderer &re);
    ~NotificationBackend();

    bool insertNotification(Notification *n); // This will become private, accessible to friend classes (dbus injector)

    size_t numNotifications() const;
    const Notification& getNotification(size_t i) const;
    void deleteNotification(const Notification *n);

    bool hasNotification(const NotificationID id) const;
    const Notification& getNotificationByID(const NotificationID id) const;

    void clear();

    // This object sends signal "changed" when notification status
    // changes.

};

#endif /* NOTIFICATIONBACKEND_HPP_ */
