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
#include <QImage>
#include <QStringList>

struct NotificationPrivate;
class NotificationServer;

class Notification : public QObject {
    Q_OBJECT
    Q_ENUMS(Urgency)
    Q_ENUMS(Type)
    Q_ENUMS(Hint)
    Q_PROPERTY(QString summary READ getSummary WRITE setSummary NOTIFY summaryChanged)
    Q_PROPERTY(QString body READ getBody WRITE setBody NOTIFY bodyChanged)
    Q_PROPERTY(NotificationID id READ getID)
    Q_PROPERTY(int value READ getValue WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString icon READ getIcon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString secondaryIcon READ getSecondaryIcon WRITE setSecondaryIcon NOTIFY secondaryIconChanged)
    Q_PROPERTY(Urgency urgency READ getUrgency WRITE setUrgency NOTIFY urgencyChanged)
    Q_PROPERTY(Type type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QStringList actions READ getActions WRITE setActions NOTIFY actionsChanged)

private:

    QScopedPointer<NotificationPrivate> p;

public:
    enum Urgency { Low, Normal, Critical };
    enum Type { Confirmation, Ephemeral, Interactive, SnapDecision, PlaceHolder };
    enum Hint { ButtonTint, IconOnly };

signals:

    void bodyChanged(QString text);
    void iconChanged(QString icon);
    void secondaryIconChanged(QString secondaryIcon);
    void summaryChanged(QString summary);
    void valueChanged(int value);
    void urgencyChanged(Urgency urg);
    void typeChanged(Type type);
    void actionsChanged(QStringList actions);

public:
    Notification(QObject *parent=0);
    Notification(NotificationID id, const Urgency ur, QString text, Type type=Confirmation, NotificationServer *srv=nullptr, QObject *parent=0);
    Notification(NotificationID id, const Urgency ur, Type type=Confirmation, NotificationServer *srv=nullptr, QObject *parent=0);
    virtual ~Notification();


    NotificationID getID() const;
    int getDisplayTime() const;

    QString getIcon() const;
    void setIcon(QString icon);
    QString getSecondaryIcon() const;
    void setSecondaryIcon(QString secondaryIcon);
    QString getBody() const;
    void setBody(const QString text);
    QString getSummary() const;
    void setSummary(QString summary);
    int getValue() const;
    void setValue(int value);
    Urgency getUrgency() const;
    void setUrgency(Urgency urg);
    Type getType() const;
    void setType(Type type);
    QStringList getActions() const;
    void setActions(QStringList actions);

    bool operator<(const Notification &n) const; // Order by "interestingness".
};

#endif /* NOTIFICATION_HPP_ */
