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

UbuntuShape {
    id: notification

    property alias iconSource: avatarIcon.source
    property alias secondaryIconSource: secondaryIcon.source
    property alias summary: summaryLabel.text
    property alias body: bodyLabel.text
    property var actions

    width: parent.width
    height: childrenRect.height
    color: Qt.rgba(0, 0, 0, 0.85)

    Item {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: childrenRect.height + contentColumn.anchors.margins * 2

        Column {
            id: contentColumn

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: units.gu(1)
            }
            height: childrenRect.height
            spacing: units.gu(1)

            Row {
                id: topRow

                spacing: units.gu(1)
                width: parent.width
                height: childrenRect.height

                UbuntuShape {
                    id: icon

                    objectName: "icon"
                    width: (body == "") ? units.gu(2) : units.gu(6)
                    height: (body == "") ? units.gu(2) : units.gu(6)
                    image: Image {
                        id: avatarIcon

                        fillMode: Image.PreserveAspectCrop
                        sourceSize.width: parent.width
                        sourceSize.height: parent.height
                    }
                }

                Image {
                    id: secondaryIcon

                    objectName: "secondaryIcon"
                    width: units.gu(2)
                    height: units.gu(2)
                    visible: source !== undefined && source != "" && bodyLabel.visible
                    sourceSize.width: width
                    sourceSize.height: height
                    fillMode: Image.PreserveAspectCrop
                }

                Column {
                    id: labelColumn
                    width: parent.width - x
                    height: childrenRect.height

                    Label {
                        id: summaryLabel

                        objectName: "summaryLabel"
                        width: parent.width - x
                        fontSize: "medium"
                        color: "#f3f3e7"
                        elide: Text.ElideRight
                    }

                    Label {
                        id: bodyLabel

                        objectName: "bodyLabel"
                        width: parent.width - x
                        visible: body != ""
                        fontSize: "small"
                        color: "#f3f3e7"
                        opacity: 0.6
                        wrapMode: Text.WordWrap
                        maximumLineCount: 10
                        elide: Text.ElideRight
                    }
                }
            }

            Row {
                id: buttonRow

                objectName: "buttonRow"
                spacing: units.gu(1)
                layoutDirection: Qt.RightToLeft
                visible: type == "Notifications.Type.SnapDecision"
                width: childrenRect.width

                Repeater {
                    model: actions

                    Button {
                        id: button

                        objectName: "button" + index
                        color: Positioner.isFirstItem ? "#d85317" : "#cdcdcb"
                        width: (topRow.width - units.gu(1)) / 2
                        height: units.gu(4)
                        text: label
                        onClicked: print("clicked " + id)
                    }
                }
            }
        }
    }
}
