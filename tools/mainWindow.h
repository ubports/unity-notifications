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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include "ui_mainWindow.h"
#include "notify-backend.h"
#include "Notification.h"

class NotificationModel;

class MainWindow : public QMainWindow, private Ui_MainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent=nullptr);
    ~MainWindow();

private:
    NotificationModel *m;
    int notificationCount;
    int syncCount;
    int interactiveCount;
    int snapCount;

    // Offsets to make notification ids unique.
    static const int notificationOffset = 0;
    static const int syncOffset = 10000;
    static const int interactiveOffset = 20000;
    static const int snapOffset = 40000;

    void sendNotification(int id, Notification::Type type, Notification::Urgency urg, QString text) const;

public Q_SLOTS:
    void queueSizeChanged(int newsize);

private Q_SLOTS:
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
