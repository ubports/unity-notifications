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

#include "notificationclient.h"
#include "notificationclientplugin.h"

#include <qqml.h>

#include <QtDBus>
#include <QQmlEngine>
#include <QQmlContext>

void NotificationClientPlugin::registerTypes(const char *uri) {
    qmlRegisterUncreatableType<NotificationClient>(uri, 1, 0, "NotificationClient", "");
}

void NotificationClientPlugin::initializeEngine(QQmlEngine *engine, const char *uri) {
    NotificationClient *cl = new NotificationClient(engine);
    if(!QDBusConnection::sessionBus().connect(DBUS_SERVICE_NAME, DBUS_PATH, DBUS_INTERFACE,
            "NotificationClosed", cl, SLOT(NotificationClosed(unsigned int, unsigned int)))) {
        printf("Could not connect to NotificationClosed signal.\n\n");
    }
    if(!QDBusConnection::sessionBus().connect(DBUS_SERVICE_NAME, DBUS_PATH, DBUS_INTERFACE,
            "ActionInvoked", cl, SLOT(ActionInvoked(unsigned int, QString)))) {
        printf("Could not connect to ActionInvoked signal.\n\n");
    }
    engine->rootContext()->setContextProperty("notificationclient", cl);
}
