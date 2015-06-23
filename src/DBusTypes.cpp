/*
 * Copyright (C) 2015 Canonical, Ltd.
 *
 * Authors:
 *    Pete Woods <pete.woods@canonical.com>
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

#include <DBusTypes.h>
#include <QDBusMetaType>

NotificationData& NotificationData::setAppName(const QString& appName_) {
    appName = appName_;
    return *this;
}

NotificationData& NotificationData::setId(quint32 id_) {
    id = id_;
    return *this;
}

NotificationData& NotificationData::setAppIcon(const QString& appIcon_) {
    appIcon = appIcon_;
    return *this;
}

NotificationData& NotificationData::setSummary(const QString& summary_) {
    summary = summary_;
    return *this;
}

NotificationData& NotificationData::setBody(const QString& body_) {
    body = body_;
    return *this;
}

NotificationData& NotificationData::setActions(const QList<QString>& actions_) {
    actions = actions_;
    return *this;
}

NotificationData& NotificationData::setHints(const QVariantMap& hints_) {
    hints = hints_;
    return *this;
}

NotificationData& NotificationData::setExpireTimeout(quint32 expireTimeout_) {
    expireTimeout = expireTimeout_;
    return *this;
}

NotificationData& NotificationData::operator=(const NotificationData& other) {
    appName = other.appName;
    id = other.id;
    appIcon = other.appIcon;
    summary = other.summary;
    body = other.body;
    actions = other.actions;
    hints = other.hints;
    expireTimeout = other.expireTimeout;
    return *this;
}

bool NotificationData::operator==(const NotificationData& other) const {
    return (appName == other.appName) && (id == other.id)
            && (appIcon == other.appIcon) && (summary == other.summary)
            && (body == other.body) && (actions == other.actions)
            && (hints == other.hints) && (expireTimeout == other.expireTimeout);
}

QDBusArgument &operator<<(QDBusArgument &argument, const NotificationData &data)
{
    argument.beginStructure();
    argument << data.appName;
    argument << data.id;
    argument << data.appIcon;
    argument << data.summary;
    argument << data.body;
    argument << data.actions;
    argument << data.hints;
    argument << data.expireTimeout;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, NotificationData &data)
{
    argument.beginStructure();
    argument >> data.appName;
    argument >> data.id;
    argument >> data.appIcon;
    argument >> data.summary;
    argument >> data.body;
    argument >> data.actions;
    argument >> data.hints;
    argument >> data.expireTimeout;
    argument.endStructure();

    return argument;
}

void DBusTypes::registerNotificationMetaTypes()
{
    qRegisterMetaType<NotificationData>("NotificationData");
    qDBusRegisterMetaType<NotificationData>();

    qRegisterMetaType<NotificationDataList>("NotificationDataList");
    qDBusRegisterMetaType<NotificationDataList>();
}
