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

#include <QApplication>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusInterface>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QDBusInterface service(DBUS_SERVICE_NAME, DBUS_PATH, DBUS_INTERFACE);
    QDBusReply<QStringList> reply = service.call("GetCapabilities");
    if(!reply.isValid()) {
        printf("Got no reply from notification service.\n");
        return 1;
    }
    QStringList caps = reply;
    printf("Server has %d caps.\n", caps.size());

}
