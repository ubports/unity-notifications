/*
* Copyright 2013 Canonical Ltd.
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
import QtTest 1.0
import Unity.Notifications 0.1 as Notifications
import Unity.Notifications.Mocks 0.1 as NotificationMocks

TestCase {

	NotificationMocks.Source {
		id: source
	}

	Repeater {
		id: repeater
		model: Notifications.Model
		delegate: Item {
			property var data: model
			property Repeater actionRepeater

			actionRepeater: Repeater {
				model: parent.data.actions
				delegate: Item {
					property var data: model
				}
			}
		}
	}

	SignalSpy {
		id: dataSpy
		target: Notifications.Model
		signalName: "dataChanged"
	}

	function initTestCase() {
		Notifications.Model.source = source;
	}

	function init() {
		Notifications.Model.confirmationPlaceholder = false;
		tryCompare(repeater, "count", "0", "There should be no notifications")
	}

	function cleanup() {
		// dismiss all Notifications
		for (var i = 0; i < repeater.count; i++) {
			repeater.itemAt(i).notification.dismissed()
			repeater.itemAt(i).notification.completed()
		}
		dataSpy.clear()
	}

	/* make sure all the required types are registered */
	function test_types() {
		compare(typeof Notifications.Urgency.Low, "number",
				"there should be an Urgency::Low enum");
		compare(typeof Notifications.Urgency.Normal, "number",
				"there should be an Urgency::Normal enum");
		compare(typeof Notifications.Urgency.Critical, "number",
				"there should be an Urgency::Critical enum");
		compare(typeof Notifications.Type.Confirmation, "number",
				"there should be a Type::Confirmation enum");
		compare(typeof Notifications.Type.Ephemeral, "number",
				"there should be a Type::Ephemeral enum");
		compare(typeof Notifications.Type.Interactive, "number",
				"there should be a Type::Interactive enum");
		compare(typeof Notifications.Type.SnapDecision, "number",
				"there should be a Type::SnapDecision enum");
		compare(typeof Notifications.Type.Placeholder, "number",
				"there should be a Type::Placeholder enum");
		compare(typeof Notifications.Hint.ButtonTint, "number",
				"there should be a Hint::ButtonTint enum");
		compare(typeof Notifications.Hint.IconOnly, "number",
				"there should be a Hint::IconOnly enum");
		compare(typeof Notifications.Notification, "object",
				"Notification should be a registered type");
		compare(typeof Notifications.Action, "object",
				"Action should be a registered type");
		compare(typeof Notifications.Model, "object",
				"Notifications.Model should be a registered singleton");
		compare(typeof Notifications.Model.source, "object",
				"Notifications.Model should have a source property");
		compare(typeof Notifications.Model.confirmationPlaceholder, "boolean",
				"Notifications.Model should have a confirmationPlaceholder property");
	}

	/* make sure all the required roles and methods of Notification are exposed */
	function test_notification_members() {
		source.send({
			"type": Notifications.Type.Interactive,
		});
		tryCompare(repeater, "count", 1, "there should be one notification");

		var data = repeater.itemAt(0).data;
		compare(typeof data.type, "number",
				"NotificationModel should expose a \"type\" role of type Type");
		compare(typeof data.urgency, "number",
				"NotificationModel should expose a \"urgency\" role of type Urgency");
		compare(typeof data.id, "number",
				"NotificationModel should expose a \"id\" role of type int");
		compare(typeof data.summary, "string",
				"NotificationModel should expose a \"summary\" role of type QString");
		compare(typeof data.body, "string",
				"NotificationModel should expose a \"body\" role of type QString");
		compare(typeof data.value, "number",
				"NotificationModel should expose a \"value\" role of type int");
		compare(typeof data.icon, "object",
				"NotificationModel should expose an \"icon\" role of type QUrl");
		compare(typeof data.secondaryIcon, "object",
				"NotificationModel should expose a \"secondaryIcon\" role of type QUrl");
		compare(typeof data.actions, "object",
				"NotificationModel should expose an \"actions\" role" +
				" of type QQmlListProperty");
		compare(typeof data.hints, "int",
				"NotificationModel should expose a \"hints\" role of type Hints");
		compare(typeof data.notification, "object",
				"NotificationModel should expose a \"notification\" role" +
				" of type Notification");

		var notification = data.notification;
		compare(typeof notification.dismissed, "function",
				"Notification::dismissed should be a signal");
		compare(typeof notification.completed, "function",
				"Notification::completed should be a signal");
		compare(typeof notification.onHovered, "function",
				"Notification::onHovered should be a slot");
		compare(typeof notification.onDisplayed, "function",
				"Notification::onDisplayed should be a slot");
	}

	/* make sure all the required roles and methods of Action are exposed */
	function test_action_members() {
		source.send({
			"type": Notifications.Type.Interactive,
			"actions": [
				{"label": "test"}
			]
			});
		tryCompare(repeater, "count", 1, "there should be one notification");

		var data = repeater.itemAt(0).data;
		data = repeater.itemAt(0).actionRepeater.itemAt(0).data;
		compare(typeof data.label, "string",
				"Notification::actions should expose a \"label\" role of type QString");
		compare(typeof data.action, "object",
				"Notification::actions should expose an \"action\" role of type Action");

		var action = data.action;
		compare(typeof action.invoke, "function", "Action::invoke should be a function");
	}

	/* make sure the model is empty by default */
	function test_empty() {
		tryCompare(repeater, "count", 0, "there should be no notifications");
	}

	/* make sure there is a placeholder item used as the first item when
       confirmationPlaceholder is true and that any additional notification is added
	   after it */
	function test_placeholder() {
		Notifications.Model.confirmationPlaceholder = true;
		tryCompare(repeater, "count", 1, "there should be one notification");
		compare(repeater.itemAt(0).data.type, Notifications.Type.Placeholder,
				"the notification should be of Placeholder type");
		source.send({
			"type": Notifications.Type.Ephemeral
		})
		tryCompare(repeater, "count", 2, "second notification was added");
		compare(repeater.itemAt(0).data.type, Notifications.Type.Placeholder,
				"the first notification should be of Placeholder type");
		compare(repeater.itemAt(1).data.type, Notifications.Type.Ephemeral,
			"the second notification should be of Ephemeral type");
	}

	/* make sure the placeholder item is updated with data incoming in a Confirmation
	   notification */
	function test_confirmation() {
		Notifications.Model.confirmationPlaceholder = true;
		tryCompare(repeater, "count", 1, "there should be only one notification");
		source.send({
			"type": Notifications.Type.Confirmation
		});
		tryCompare(dataSpy, "count", 1, "notification data should have been updated");
		compare(repeater.count, 1, "there should be only one notification");
		compare(repeater.itemAt(0).type, Notifications.Type.Confirmation,
				"the first notification should be of Confirmation type");
	}
}