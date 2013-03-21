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

#include <QMainWindow>
#include <QListView>
#include "ui_mainWindow.h"

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

public slots:
    void queueSizeChanged(int newsize);

private slots:
    void sendNotification();
    void sendSynchronousNotification();
    void sendInteractiveNotification();
    void sendSnapNotification();

};
