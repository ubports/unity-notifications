/*
 * Copyright (C) 2014 Canonical, Ltd.
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

#ifndef ACTION_MODEL_HPP_
#define ACTION_MODEL_HPP_

#include <QStringListModel>

struct ActionModelPrivate;

class ActionModel : public QStringListModel {
	Q_OBJECT
    Q_ENUMS(ActionsRoles)

public:
    ActionModel(QObject *parent=nullptr);
    virtual ~ActionModel();

    int rowCount(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    enum ActionsRoles {
        RoleActionLabel = Qt::UserRole + 1,
        RoleActionId    = Qt::UserRole + 2
    };
    Q_INVOKABLE QVariant data(int row, int role) const;

    void insertAction(const QString &id, const QString &label);

    QStringList getRawActions() const;

private:
    QScopedPointer<ActionModelPrivate> p;

};

#endif /* ACTION_MODEL_HPP_ */
