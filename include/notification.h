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
class NotificationServer;

class Notification : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString summary READ getSummary WRITE setSummary NOTIFY summaryChanged)
    Q_PROPERTY(QString body READ getBody WRITE setBody NOTIFY bodyChanged)
    Q_PROPERTY(NotificationID id READ getID)
    Q_PROPERTY(QString icon READ getIcon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(Urgency urgency READ getUrgency WRITE setUrgency NOTIFY urgencyChanged)
    Q_PROPERTY(NotificationType type READ getType WRITE setType NOTIFY typeChanged)

private:

    QScopedPointer<NotificationPrivate> p;

signals:

    void bodyChanged(QString text);
    void iconChanged(QString icon);
    void summaryChanged(QString summary);
    void urgencyChanged(Urgency urg);
    void typeChanged(NotificationType type);

public:
    Notification(QObject *parent=0);
    Notification(NotificationID id, const Urgency ur, QString text, NotificationType type=ASYNCHRONOUS, NotificationServer *srv=nullptr, QObject *parent=0);
    Notification(NotificationID id, const Urgency ur, NotificationType type=ASYNCHRONOUS, NotificationServer *srv=nullptr, QObject *parent=0);
    virtual ~Notification();


    NotificationID getID() const;
    int getDisplayTime() const;

    QString getIcon() const;
    void setIcon(QString icon);
    QString getBody() const;
    void setBody(const QString text);
    QString getSummary() const;
    void setSummary(QString summary);
    Urgency getUrgency() const;
    void setUrgency(Urgency urg);
    NotificationType getType() const;
    void setType(NotificationType type);

    bool operator<(const Notification &n) const; // Order by "interestingness".
};

#endif /* NOTIFICATION_HPP_ */
