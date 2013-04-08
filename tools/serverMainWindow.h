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
#include "ui_serverMainWindow.h"
#include "notify-backend.h"

class NotificationModel;

class ServerMainWindow : public QMainWindow, private Ui_ServerMainWindow {
    Q_OBJECT

public:
    ServerMainWindow(NotificationModel &m, QWidget *parent=nullptr);
    ~ServerMainWindow();

public slots:
    void queueSizeChanged(int newSize);

private:
    NotificationModel &model;
};
