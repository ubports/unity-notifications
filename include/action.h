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

#ifndef ACTION_HPP_
#define ACTION_HPP_

#include <QString>
#include <QObject>

struct ActionPrivate;

class Action : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString label READ getLabel WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QString action READ getAction WRITE setAction NOTIFY actionChanged)

private:
    QScopedPointer<ActionPrivate> p;

signals:
    void labelChanged(QString label);
    void actionChanged(QString action);

public:
    Action(QObject *parent=0);
    Action(QString label, QString action, QObject *parent=0);
    virtual ~Action();

    QString getLabel() const;
    void setLabel(QString label);
    QString getAction() const;
    void setAction(QString action);
};

#endif /* ACTION_HPP_ */
