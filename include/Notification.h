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


#ifndef NOTIFICATION_HPP_
#define NOTIFICATION_HPP_

#include "ActionModel.h"
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
    Q_PROPERTY(QString summary READ getSummary WRITE setSummary NOTIFY summaryChanged)
    Q_PROPERTY(QString body READ getBody WRITE setBody NOTIFY bodyChanged)
    Q_PROPERTY(NotificationID id READ getID)
    Q_PROPERTY(int value READ getValue WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString icon READ getIcon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString secondaryIcon READ getSecondaryIcon WRITE setSecondaryIcon NOTIFY secondaryIconChanged)
    Q_PROPERTY(Urgency urgency READ getUrgency WRITE setUrgency NOTIFY urgencyChanged)
    Q_PROPERTY(Type type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(ActionModel* actions READ getActions NOTIFY actionsChanged)
    Q_PROPERTY(QVariantMap hints READ getHints WRITE setHints NOTIFY hintsChanged)

private:
    QScopedPointer<NotificationPrivate> p;

public:
    enum Urgency { Low, Normal, Critical };
    enum Type { PlaceHolder, Confirmation, Ephemeral, Interactive, SnapDecision };

Q_SIGNALS:
    void bodyChanged(const QString &text);
    void iconChanged(const QString &icon);
    void secondaryIconChanged(const QString &secondaryIcon);
    void summaryChanged(const QString &summary);
    void valueChanged(int value);
    void urgencyChanged(Urgency urg);
    void typeChanged(Type type);
    void actionsChanged(const QStringList &actions);
    void hintsChanged(const QVariantMap& hints);

    void dataChanged(unsigned int id);

    void dismissed();
    void completed(unsigned int id);

public:
    Notification(QObject *parent=nullptr);
    Notification(NotificationID id,
            int displayTime,
            const Urgency ur,
            const QString &text, Type type=Ephemeral,
            NotificationServer *srv=nullptr,
            QObject *parent=nullptr);
    Notification(NotificationID id,
            int displayTime,
            const Urgency ur,
            Type type=Ephemeral,
            NotificationServer *srv=nullptr,
            QObject *parent=nullptr);
    virtual ~Notification();

    NotificationID getID() const;
    int getDisplayTime() const;

    QString getIcon() const;
    void setIcon(const QString &icon);
    QString getSecondaryIcon() const;
    void setSecondaryIcon(const QString &secondaryIcon);
    QString getBody() const;
    void setBody(const QString &text);
    QString getSummary() const;
    void setSummary(const QString &summary);
    int getValue() const;
    void setValue(int value);
    Urgency getUrgency() const;
    void setUrgency(Urgency urg);
    Type getType() const;
    void setType(Type type);
    ActionModel* getActions() const;
    void setActions(const QStringList &actions);
    QString getClientId() const;
    void setClientId(const QString& clientId);
    void detachFromServer();

    QVariantMap getHints() const;
    void setHints(const QVariantMap& hints);

    Q_INVOKABLE void invokeAction(const QString &action);
    Q_INVOKABLE void close();

    bool operator<(const Notification &n) const; // Order by "interestingness".

private:
    QString filterText(const QString& text);
};

Q_DECLARE_METATYPE(Notification::Urgency)
Q_DECLARE_METATYPE(Notification::Type)

#endif /* NOTIFICATION_HPP_ */
