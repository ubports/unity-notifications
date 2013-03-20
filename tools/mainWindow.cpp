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
#include "notify-backend.hpp"
#include "notificationmodel.h"
#include "mainWindow.h"
#include "notification.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);
    m = new NotificationModel();
    notificationCount = 0;
    syncCount = 0;
    interactiveCount = 0;
    listView->setModel(m);
    connect(this->notificationSendButton, SIGNAL(clicked()), this, SLOT(sendNotification()));
    connect(this->synchronousSendButton, SIGNAL(clicked()), this, SLOT(sendSynchronousNotification()));
    connect(this->interactiveSendButton, SIGNAL(clicked()), this, SLOT(sendInteractiveNotification()));
    connect(m, SIGNAL(queueSizeChanged(int)), this, SLOT(queueSizeChanged(int)));

}

MainWindow::~MainWindow() {

}

void MainWindow::sendNotification() {
    QString text("notification number ");
    text += QString::number(notificationCount, 10);
    QSharedPointer<Notification> n(new Notification(notificationCount++, URGENCY_LOW, text));
    m->insertNotification(n); // Wrap in a try/catch eventually.
}

void MainWindow::queueSizeChanged(int newsize) {
    QString text("Notifications in queue: ");
    text += QString::number(newsize, 10);
    queueLabel->setText(text);
}

void MainWindow::sendSynchronousNotification() {
    QString text("sync number ");
    text += QString::number(syncCount, 10);
    QSharedPointer<Notification> n(new Notification(syncCount++, URGENCY_LOW, text, SYNCHRONOUS));

    m->insertNotification(n); // Wrap in a try/catch eventually.
}

void MainWindow::sendInteractiveNotification() {
    QString text("interactive number ");
    text += QString::number(interactiveCount, 10);
    QSharedPointer<Notification> n(new Notification(interactiveCount++, URGENCY_LOW, text, INTERACTIVE));

    m->insertNotification(n); // Wrap in a try/catch eventually.
}
