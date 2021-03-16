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
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "NotificationPlugin.h"
#include "NotificationModel.h"
#include "NotificationServer.h"

#include <unity/shell/notifications/ModelInterface.h>

#include <qqml.h>

#include <QDBusConnection>
#include <QQmlEngine>
#include <QQmlContext>

// These are qml singletons, so we might as well store them
// in global variables.
static NotificationModel *m = NULL;
static NotificationServer *s = NULL;

using namespace unity::shell::notifications;

static QObject* modelProvider(QQmlEngine* /* engine */, QJSEngine* /* scriptEngine */)
{
    return m;
}

void NotificationPlugin::registerTypes(const char *uri) {
    // @uri Unity.Notifications
    qmlRegisterUncreatableType<ModelInterface>(uri, 1, 0, "ModelInterface", "Abstract Interface. Cannot be instantiated.");
    qmlRegisterSingletonType<NotificationModel>(uri, 1, 0, "Model", modelProvider);
    qmlRegisterUncreatableType<Notification>(uri, 1, 0, "Notification", "Notification objects can only be created by the plugin");
    qmlRegisterUncreatableType<ActionModel>(uri, 1, 0, "ActionModel", "Abstract Interface. Cannot be instantiated.");
}

void NotificationPlugin::initializeEngine(QQmlEngine *engine, const char * /* uri */) {
    m = new NotificationModel();
    s = new NotificationServer(QDBusConnection::sessionBus(), *m, engine);
}
