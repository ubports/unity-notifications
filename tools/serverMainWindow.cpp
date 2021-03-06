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
#include "serverMainWindow.h"

ServerMainWindow::ServerMainWindow(NotificationModel &m, QWidget *parent) :
    QMainWindow(parent), model(m) {
    setupUi(this);
    listView->setModel(&model);
    connect(&model, SIGNAL(queueSizeChanged(int)), this, SLOT(queueSizeChanged(int)));
}

ServerMainWindow::~ServerMainWindow() {

}

void ServerMainWindow::queueSizeChanged(int newSize) {
    QString text("Notifications in queue: ");
    text += QString::number(newSize, 10);
    queueLabel->setText(text);

}
