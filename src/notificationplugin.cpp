/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * Authors:
 *    Jussi Pakkanen <jussi.pakkanen@canonical.com>
 *    Michał Sawicz <michal.sawicz@canonical.com>
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

#include "notificationplugin.h"
#include "notificationmodel.h"
#include "notificationserver.h"

#include <qqml.h>

#include <QtDBus>
#include <QQmlEngine>
#include <QQmlContext>

void NotificationPlugin::registerTypes(const char *uri) {
    // @uri Unity.Notifications
    qmlRegisterUncreatableType<NotificationModel>(uri, 1, 0, "NotificationModel", "");
}

void NotificationPlugin::initializeEngine(QQmlEngine *engine, const char *uri) {
    NotificationModel *m = new NotificationModel();

    new NotificationServer(*m, engine);

    if(!QDBusConnection::sessionBus().registerService(DBUS_SERVICE_NAME)) {
        fprintf(stderr, "Service name already taken.\n");
    }
    if(!QDBusConnection::sessionBus().registerObject(DBUS_PATH, engine)) {
        fprintf(stderr, "Could not register to DBus session.\n");
    }

    // Shared pointer problem: http://qt-project.org/wiki/SharedPointersAndQmlOwnership
    engine->rootContext()->setContextProperty("notificationmodel", m);
}
