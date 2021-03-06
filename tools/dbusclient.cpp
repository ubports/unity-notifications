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
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "NotificationClient.h"
#include "clientMainWindow.h"

#include <QApplication>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusInterface>
#include <QDBusMetaType>

struct InfoStruct {
    QString name;
    QString vendor;
    QString version;
};
Q_DECLARE_METATYPE(InfoStruct)

QDBusArgument &operator<<(QDBusArgument &argument, const InfoStruct &mystruct)
{
    argument.beginStructure();
    argument << mystruct.name << mystruct.vendor << mystruct.version;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, InfoStruct &mystruct)
{
    argument.beginStructure();
    argument >> mystruct.name >> mystruct.vendor >> mystruct.version;
    argument.endStructure();
    return argument;
}

void getCaps(OrgFreedesktopNotificationsInterface &service) {
    QDBusReply<QStringList> reply = service.GetCapabilities();
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

void getInfo(OrgFreedesktopNotificationsInterface &service) {
    QString name, vendor, version;
    QDBusReply<InfoStruct> reply = service.GetServerInformation();
    if(!reply.isValid()) {
        printf("Got no reply for server info query.\n");
        return;
    }
    InfoStruct i = reply.value();
    printf("Server info:\n");
    printf(" name:    %s\n", i.name.toUtf8().constData());
    printf(" vendor:  %s\n", i.vendor.toUtf8().constData());
    printf(" version: %s\n", i.version.toUtf8().constData());
}

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    qDBusRegisterMetaType<InfoStruct>();
    NotificationClient *cl = new NotificationClient(QDBusConnection::sessionBus(), &app);
    ClientMainWindow *win = new ClientMainWindow(*cl);
    QObject::connect(cl, SIGNAL(eventHappened(QString)), win, SLOT(appendText(QString)));
    OrgFreedesktopNotificationsInterface service(DBUS_SERVICE_NAME, DBUS_PATH, QDBusConnection::sessionBus());

    getCaps(service);
    printf("\n");
    getInfo(service);
    win->show();
    return app.exec();
}
