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


#ifndef NOTIFICATION_HPP_
#define NOTIFICATION_HPP_

#include "notify-backend.h"
#include <QString>
#include <QObject>
#include <QScopedPointer>

struct NotificationPrivate;

class Notification : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)

private:

    QScopedPointer<NotificationPrivate> p;

signals:

    void textChanged(QString text);

public:
    Notification();
    Notification(NotificationID id, const Urgency ur, QString text, NotificationType type=ASYNCHRONOUS);
    ~Notification();

    QString getText() const;
    void setText(const QString text);

    NotificationID getID() const;
    Urgency getUrgency() const;
    NotificationType getType() const;
    int getDisplayTime() const;

    bool operator<(const Notification &n) const; // Order by "interestingness".
};

#endif /* NOTIFICATION_HPP_ */
