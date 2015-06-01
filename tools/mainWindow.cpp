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
#include "notify-backend.h"
#include "NotificationModel.h"
#include "mainWindow.h"
#include "Notification.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);
    m = new NotificationModel();
    notificationCount = 0;
    syncCount = 0;
    interactiveCount = 0;
    snapCount = 0;
    listView->setModel(m);

    connect(this->synchronousSendButton, SIGNAL(clicked()), this, SLOT(sendSynchronousNotification()));

    connect(this->notificationSendButton, SIGNAL(clicked()), this, SLOT(sendLowNotification()));
    connect(this->notificationNormalButton, SIGNAL(clicked()), this, SLOT(sendNormalNotification()));
    connect(this->notificationCriticalButton, SIGNAL(clicked()), this, SLOT(sendCriticalNotification()));

    connect(this->interactiveSendButton, SIGNAL(clicked()), this, SLOT(sendLowInteractiveNotification()));
    connect(this->interactiveNormalButton, SIGNAL(clicked()), this, SLOT(sendNormalInteractiveNotification()));
    connect(this->interactiveCriticalButton, SIGNAL(clicked()), this, SLOT(sendCriticalInteractiveNotification()));

    connect(this->snapSendButton, SIGNAL(clicked()), this, SLOT(sendSnapNotification()));
    connect(this->snapNormalButton, SIGNAL(clicked()), this, SLOT(sendNormalSnapNotification()));
    connect(this->snapCriticalButton, SIGNAL(clicked()), this, SLOT(sendCriticalSnapNotification()));

    connect(m, SIGNAL(queueSizeChanged(int)), this, SLOT(queueSizeChanged(int)));

}

MainWindow::~MainWindow() {

}

void MainWindow::sendLowNotification() {
    QString text("low async number ");
    text += QString::number(notificationCount, 10);
    sendNotification(notificationOffset + notificationCount++, Notification::Ephemeral, Notification::Low, text);
}

void MainWindow::sendNormalNotification() {
    QString text("normal async number ");
    text += QString::number(notificationCount, 10);
    sendNotification(notificationOffset + notificationCount++, Notification::Ephemeral, Notification::Normal, text);
}

void MainWindow::sendCriticalNotification() {
    QString text("critical async number ");
    text += QString::number(notificationCount, 10);
    sendNotification(notificationOffset + notificationCount++, Notification::Ephemeral, Notification::Critical, text);
}

void MainWindow::queueSizeChanged(int newsize) {
    QString text("Notifications in queue: ");
    text += QString::number(newsize, 10);
    queueLabel->setText(text);
}

void MainWindow::sendSynchronousNotification() {
    int timeout = 5000;
    QString text("sync number ");
    text += QString::number(syncCount, 10);
    QSharedPointer<Notification> n(new Notification(syncOffset + syncCount++, timeout, Notification::Low, text, Notification::Confirmation));

    m->insertNotification(n); // Wrap in a try/catch eventually.
}

void MainWindow::sendLowInteractiveNotification() {
    QString text("low interactive number ");
    text += QString::number(interactiveCount, 10);
    sendNotification(interactiveOffset + interactiveCount++, Notification::Interactive, Notification::Low, text);
}

void MainWindow::sendNormalInteractiveNotification() {
    QString text("normal interactive number ");
    text += QString::number(interactiveCount, 10);
    sendNotification(interactiveOffset + interactiveCount++, Notification::Interactive, Notification::Normal, text);
}

void MainWindow::sendCriticalInteractiveNotification() {
    QString text("critical interactive number ");
    text += QString::number(interactiveCount, 10);
    sendNotification(interactiveOffset + interactiveCount++, Notification::Interactive, Notification::Critical, text);
}

void MainWindow::sendSnapNotification() {
    QString text("low snap number ");
    text += QString::number(snapCount, 10);
    sendNotification(snapOffset + snapCount++, Notification::SnapDecision, Notification::Low, text);
}

void MainWindow::sendNormalSnapNotification() {
    QString text("normal snap number ");
    text += QString::number(snapCount, 10);
    sendNotification(snapOffset + snapCount++, Notification::SnapDecision, Notification::Normal, text);
}

void MainWindow::sendCriticalSnapNotification() {
    QString text("critical snap number ");
    text += QString::number(snapCount, 10);
    sendNotification(snapOffset + snapCount++, Notification::SnapDecision, Notification::Critical, text);
}

void MainWindow::sendNotification(int id, Notification::Type type, Notification::Urgency urg, QString text) const {
    int timeout = 5000;
    QSharedPointer<Notification> n(new Notification(id, timeout, urg, text, type));
    m->insertNotification(n); // Wrap in a try/catch eventually.
}
