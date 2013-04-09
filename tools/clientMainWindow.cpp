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

#include"clientMainWindow.h"
#include "notificationclient.h"

ClientMainWindow::ClientMainWindow(NotificationClient &cl, QWidget *parent) :
    QMainWindow(parent), client(cl) {
    setupUi(this);

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
}

ClientMainWindow::~ClientMainWindow() {

}

void ClientMainWindow::sendNotification(NotificationType type, Urgency urg, QString text) {
    unsigned int res = client.sendNotification(type, urg, text);
    QString msg = "Sent Notification which got reply id ";
    msg += QString::number(res, 10);
    msg += ".\n";
    appendText(msg);
}

void ClientMainWindow::appendText(QString text) {
    this->plainTextEdit->appendPlainText(text);
}

void ClientMainWindow::sendLowNotification() {
    sendNotification(ASYNCHRONOUS, URGENCY_LOW, "Low urgency asynchronous");
}

void ClientMainWindow::sendNormalNotification() {
    sendNotification(ASYNCHRONOUS, URGENCY_NORMAL, "Normal urgency asynchronous");
}

void ClientMainWindow::sendCriticalNotification() {
    sendNotification(ASYNCHRONOUS, URGENCY_CRITICAL, "Critical urgency asynchronous");
}

void ClientMainWindow::sendSynchronousNotification() {
    sendNotification(SYNCHRONOUS, URGENCY_NORMAL, "Normal urgency synchronous");
}

void ClientMainWindow::sendLowInteractiveNotification() {
    sendNotification(INTERACTIVE, URGENCY_LOW, "Low urgency interactive");
}

void ClientMainWindow::sendNormalInteractiveNotification() {
    sendNotification(INTERACTIVE, URGENCY_NORMAL, "Normal urgency interactive");
}

void ClientMainWindow::sendCriticalInteractiveNotification() {
    sendNotification(INTERACTIVE, URGENCY_CRITICAL, "Critical urgency interactive");
}

void ClientMainWindow::sendSnapNotification() {
    sendNotification(SNAP, URGENCY_LOW, "Low urgency snap");
}

void ClientMainWindow::sendNormalSnapNotification() {
    sendNotification(SNAP, URGENCY_NORMAL, "Normal urgency snap");
}

void ClientMainWindow::sendCriticalSnapNotification() {
    sendNotification(SNAP, URGENCY_CRITICAL, "Critical urgency snap");

}
