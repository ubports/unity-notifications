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

#include<cstdlib>

struct NotificationBackendPrivate;
class Notification;

const unsigned int MAX_NOTIFICATIONS = 50;

class NotificationBackend {
private:

    NotificationBackendPrivate *p;

public:
    NotificationBackend();
    ~NotificationBackend();

    bool hasNotification(const Notification *n) const;
    bool insertNotification(Notification *n);

    size_t numNotifications() const;
    const Notification& getNotification(size_t i) const;
    void deleteNotification(const Notification *n);

    // This object sends signal "changed" when notification status
    // changes.

};

#endif /* NOTIFICATIONBACKEND_HPP_ */
