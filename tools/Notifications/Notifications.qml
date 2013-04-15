/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import Ubuntu.Components 0.1

ListView {
    id: notificationRenderer

    objectName: "notificationRenderer"
    anchors.fill: parent
    interactive: false

    spacing: units.gu(.5)
    delegate: Notification {
        objectName: "notification" + index
        //iconSource: model.icon
        //secondaryIconSource: model.secondaryIcon
        //summary: model.summary
        //body: model.body
        iconSource: "graphics/avatar3.jpg"
        summary: notificationmodel.data.summary //tempHackGetSummary()
        body: notificationmodel.data.body //tempHackGetBody()
        //actions: model.actions
    }
}
