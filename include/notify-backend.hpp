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

#include<cstdlib>

typedef int NotificationID;

enum Urgency {
    URGENCY_LOW,
    URGENCY_NORMAL,
    URGENCY_CRITICAL
};

const unsigned int MAX_NOTIFICATIONS = 50;

class Renderer;
class NotificationBackend;
class Notification;

#endif
