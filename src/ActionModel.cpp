/*
 * Copyright (C) 2013 Canonical, Ltd.
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

#include "ActionModel.h"

struct ActionModelPrivate {
    QList<QString> labels;
    QList<QString> ids;
};

ActionModel::ActionModel(QObject *parent) : QStringListModel(parent), p(new ActionModelPrivate) {
}

ActionModel::~ActionModel() {
}

int ActionModel::rowCount(const QModelIndex &index) const {
    return p->labels.size();
}

QVariant ActionModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
            return QVariant();

    switch(role) {
        case RoleActionLabel:
            return QVariant(p->labels[index.row()]);

        case RoleActionId:
            return QVariant(p->ids[index.row()]);

        default:
            return QVariant();
    }
}

QHash<int, QByteArray> ActionModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles.insert(RoleActionLabel, "label");
    roles.insert(RoleActionId, "id");

    return roles;
}

QVariant ActionModel::data(int row, int role) const {
    return data(index(row, 0), role);
}

void ActionModel::insertAction(const QString &id, const QString &label) {
    p->ids.push_back(id);
    p->labels.push_back(label);
}

QStringList ActionModel::getRawActions() const {
    QStringList actions;
    for (int i = 0; i < p->ids.size(); ++i) {
        actions << p->ids[i] << p->labels[i];
    }
    return actions;
}
