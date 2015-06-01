/*
 * Copyright (C) 2015 Canonical, Ltd.
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

#ifndef DBUSTYPES_HPP_
#define DBUSTYPES_HPP_

#include <QDebug>
#include <QDBusArgument>
#include <QString>
#include <QVariantMap>

struct NotificationData
{
    QString appName;
    quint32 id;
    QString appIcon;
    QString summary;
    QString body;
    QList<QString> actions;
    QVariantMap hints;
    qint32 expireTimeout;

    NotificationData& setAppName(const QString& appName_);

    NotificationData& setId(quint32 id_);

    NotificationData& setAppIcon(const QString& appIcon_);

    NotificationData& setSummary(const QString& summary_);

    NotificationData& setBody(const QString& body_);

    NotificationData& setActions(const QList<QString>& actions_);

    NotificationData& setHints(const QVariantMap& hints_);

    NotificationData& setExpireTimeout(quint32 expireTimeout_);

    NotificationData& operator=(const NotificationData& other);

    bool operator==(const NotificationData& other) const;
};

typedef QList<NotificationData> NotificationDataList;

QDBusArgument &operator<<(QDBusArgument &, const NotificationData &);
const QDBusArgument &operator>>(const QDBusArgument &, NotificationData &);

namespace DBusTypes {
    void registerNotificationMetaTypes();
}

Q_DECLARE_METATYPE(NotificationData)
Q_DECLARE_METATYPE(NotificationDataList)

#endif
