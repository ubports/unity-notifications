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

#include"clientMainWindow.h"
#include "notificationclient.h"
#include "notification.h"

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

void ClientMainWindow::sendNotification(Notification::Type type, Notification::Urgency urg, QString text) {
    QString summary("summary");
    unsigned int res = client.sendNotification(type, urg, summary, text);
    QString msg = "Sent Notification which got reply id ";
    msg += QString::number(res, 10);
    msg += ".\n";
    appendText(msg);
}

void ClientMainWindow::appendText(QString text) {
    this->plainTextEdit->appendPlainText(text);
}

void ClientMainWindow::sendLowNotification() {
    sendNotification(Notification::Ephemeral, Notification::Low, "Low urgency asynchronous");
}

void ClientMainWindow::sendNormalNotification() {
    sendNotification(Notification::Ephemeral, Notification::Normal, "Normal urgency asynchronous");
}

void ClientMainWindow::sendCriticalNotification() {
    sendNotification(Notification::Ephemeral, Notification::Critical, "Critical urgency asynchronous");
}

void ClientMainWindow::sendSynchronousNotification() {
    sendNotification(Notification::Confirmation, Notification::Normal, "Normal urgency synchronous");
}

void ClientMainWindow::sendLowInteractiveNotification() {
    sendNotification(Notification::Interactive, Notification::Low, "Low urgency interactive");
}

void ClientMainWindow::sendNormalInteractiveNotification() {
    sendNotification(Notification::Interactive, Notification::Normal, "Normal urgency interactive");
}

void ClientMainWindow::sendCriticalInteractiveNotification() {
    sendNotification(Notification::Interactive, Notification::Critical, "Critical urgency interactive");
}

void ClientMainWindow::sendSnapNotification() {
    sendNotification(Notification::SnapDecision, Notification::Low, "Low urgency snap");
}

void ClientMainWindow::sendNormalSnapNotification() {
    sendNotification(Notification::SnapDecision, Notification::Normal, "Normal urgency snap");
}

void ClientMainWindow::sendCriticalSnapNotification() {
    sendNotification(Notification::SnapDecision, Notification::Critical, "Critical urgency snap");

}
