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

#include "NotificationModel.h"
#include "NotificationServer.h"
#include "serverMainWindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char **argv) {
    bool gui = !(argc == 2 && QString::fromUtf8(argv[1]) == "--no-gui");

    QSharedPointer<QCoreApplication> app;
    if (gui) {
        app.reset(new QApplication(argc, argv));
    } else {
        app.reset(new QCoreApplication(argc, argv));
    }

    NotificationModel model;
    new NotificationServer(QDBusConnection::sessionBus(), model, app.data());

    if (gui) {
        ServerMainWindow *w = new ServerMainWindow(model);
        w->show();
    }
    return app->exec();
}
