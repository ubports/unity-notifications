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

// All core definitions of Notify-Backend.

#ifndef NOTIFY_BACKEND_
#define NOTIFY_BACKEND_

#include <cstdlib>

typedef unsigned int NotificationID;

/*enum Urgency {
    URGENCY_LOW,
    URGENCY_NORMAL,
    URGENCY_CRITICAL
};

enum NotificationType {
    SYNCHRONOUS,
    SNAP,
    INTERACTIVE,
    ASYNCHRONOUS
};*/

const unsigned int MAX_NOTIFICATIONS = 50;

class Renderer;
class NotificationBackend;
class Notification;

#if 1
#define DBUS_SERVICE_NAME "com.canonical.notificationproto"
#define DBUS_INTERFACE    "com.canonical.notificationproto"
#define DBUS_PATH         "/com/canonical/notificationproto"
#else
#define DBUS_SERVICE_NAME "org.freedesktop.Notifications"
#define DBUS_INTERFACE    "org.freedesktop.Notifications"
#define DBUS_PATH         "/org/freedesktop/Notifications"
#endif

#define URGENCY_HINT "urgency"
#define SYNCH_HINT "x-canonical-private-synchronous"
#define SNAP_HINT "x-canonical-snap-decisions"
#define INTERACTIVE_HINT "x-canonical-switch-to-application"

#define APPEND_HINT "x-canonical-append"

#endif
