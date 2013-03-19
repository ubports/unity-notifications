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

#include "notificationmodel.h"
#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);
    NotificationModel *m = new NotificationModel();
/*
    QTimer::singleShot(3000, m, SLOT(testInsert()));
    QTimer::singleShot(6000, m, SLOT(testDelete()));
    notifications.push_back(new Notification(1, URGENCY_LOW, "first"));
    notifications.push_back(new Notification(2, URGENCY_LOW, "second"));
    notifications.push_back(new Notification(3, URGENCY_LOW, "third"));
*/

    this->listView->setModel(m);
    connect(this->notificationSendButton, SIGNAL(clicked()), this, SLOT(sendNotification()));
}

MainWindow::~MainWindow() {

}

void MainWindow::sendNotification() {
    printf("Foo.\n");
}
