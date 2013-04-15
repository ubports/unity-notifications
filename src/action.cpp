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

#include "action.h"

struct ActionPrivate {
    QString label;
    QString action;
};

Action::Action(QObject *parent) : QObject(parent), p(new ActionPrivate()) {
	p->label = QString("default label");
	p->action = QString("default action");
}

Action::Action(QString label, QString action, QObject *parent) : QObject(parent), p(new ActionPrivate()) {
    p->label  = label;
    p->action = action;
}

Action::~Action() {
}

QString Action::getLabel() const {
	return p->label;
}

void Action::setLabel(QString label) {
	p->label = label;
	emit labelChanged(p->label);
}

QString Action::getAction() const {
	return p->action;
}

void Action::setAction(QString action) {
	p->action = action;
	emit actionChanged(p->action);
}
