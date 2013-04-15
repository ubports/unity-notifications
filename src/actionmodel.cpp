/*
 * Copyright (C) 2013 Canonical, Ltd.
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

#include "actionmodel.h"

struct ActionModelPrivate {
	QList<QSharedPointer<Action> > actions;
};

ActionModel::ActionModel(QObject *parent) : QAbstractListModel(parent), p(new ActionModelPrivate) {
}

ActionModel::~ActionModel() {
}

int ActionModel::rowCount(const QModelIndex &index) const {
    return p->actions.size();
}

QVariant ActionModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
            return QVariant();

    switch(role) {
        case RoleLabel:
            return QVariant(p->actions[index.row()]->getLabel());

        case RoleAction:
            return QVariant(p->actions[index.row()]->getAction());

        default:
            return QVariant();
    }
}

QHash<int, QByteArray> ActionModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles.insert(RoleLabel, "label");
    roles.insert(RoleAction, "action");

    return roles;
}

void ActionModel::insertAction(QSharedPointer<Action> action) {
	p->actions.push_back(action);
}
