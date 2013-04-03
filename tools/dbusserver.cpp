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

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    new NotificationServer(&app);

    if(!QDBusConnection::sessionBus().registerService(DBUS_SERVICE_NAME)) {
        printf("Service name already taken.\n");
        return 1;
    }
    if(!QDBusConnection::sessionBus().registerObject(DBUS_PATH, &app)) {
        printf("Could not register to DBus session.\n");
        return 1;
    }
    app.exec();
    return 0;
}
