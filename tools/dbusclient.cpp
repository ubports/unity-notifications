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

void getCaps(QDBusInterface &service) {
    QDBusReply<QStringList> reply = service.call("GetCapabilities");
    if(!reply.isValid()) {
        printf("Got no reply for capability query.\n");
        return;
    }
    QStringList caps = reply.value();
    printf("The server has the following capabilities:\n");
    for(int i=0; i<caps.size(); i++) {
        printf(" %s\n", caps[i].toUtf8().constData());
    }
}

void getInfo(QDBusInterface &service) {
    QString name, vendor, version;
    QDBusReply<void> reply = service.call("GetServerInformation", name, vendor, version);
    if(!reply.isValid()) {
        printf("Got no reply for server info query.\n");
        return;
    }
    printf("Server info:\n");
    printf(" name:    %s\n", name.toUtf8().constData());
    printf(" vendor:  %s\n", vendor.toUtf8().constData());
    printf(" version: %s\n", version.toUtf8().constData());
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QDBusInterface service(DBUS_SERVICE_NAME, DBUS_PATH, DBUS_INTERFACE);

    getCaps(service);
    getInfo(service);
    return 0;
}
