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

#ifndef CLIENTMAINWINDOW_H
#define CLIENTMAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include "ui_clientMainWindow.h"
#include "notify-backend.h"
#include "notification.h"

class NotificationClient;

class ClientMainWindow : public QMainWindow, private Ui_ClientMainWindow {
    Q_OBJECT

public:
    ClientMainWindow(NotificationClient &cl, QWidget *parent=nullptr);
    ~ClientMainWindow();

public slots:
    void appendText(QString text);

private:
    NotificationClient &client;
    void sendNotification(Notification::Type type, Notification::Urgency urg, QString text);

private slots:
    void sendLowNotification();
    void sendNormalNotification();
    void sendCriticalNotification();

    void sendSynchronousNotification();

    void sendLowInteractiveNotification();
    void sendNormalInteractiveNotification();
    void sendCriticalInteractiveNotification();

    void sendSnapNotification();
    void sendNormalSnapNotification();
    void sendCriticalSnapNotification();
};

#endif
