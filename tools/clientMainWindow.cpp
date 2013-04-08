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

ClientMainWindow::ClientMainWindow(QWidget *parent, NotificationClient &cl) :
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

void ClientMainWindow::sendLowNotification() {
    client.sendNotification(ASYNCHRONOUS, URGENCY_LOW, "Low urgency asynchronous");
}

void ClientMainWindow::sendNormalNotification() {

}

void ClientMainWindow::sendCriticalNotification() {

}

void ClientMainWindow::sendSynchronousNotification() {

}

void ClientMainWindow::sendLowInteractiveNotification() {

}

void ClientMainWindow::sendNormalInteractiveNotification() {

}

void ClientMainWindow::sendCriticalInteractiveNotification() {
}

void ClientMainWindow::sendSnapNotification() {

}

void ClientMainWindow::sendNormalSnapNotification() {

}

void ClientMainWindow::sendCriticalSnapNotification() {

}
