#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly
##
## Run:
##    chmod +x sd-example-simunlock.py
##    ./sd-example-simunlock.py
##
## Copyright 2013 Canonical Ltd.
##
## Author:
##    Mirco "MacSlow" Mueller <mirco.mueller@canonical.com>
##
## This program is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License version 3, as published
## by the Free Software Foundation.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranties of
## MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
## PURPOSE.  See the GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program.  If not, see <http://www.gnu.org/licenses/>.
##
################################################################################

import os
import sys

from gi.repository import Gio, GLib, Notify

APPLICATION_ID = 'com.canonical.indicator.network.example'
SIM_UNLOCK_MENU_PATH = '/com/canonical/indicator/network/example/unlocksim'
SIM_UNLOCK_ACTION_PATH = '/com/canonical/indicator/network/example/unlocksim'

def quit_callback(notification, loop):
	global connection
	global exported_action_group_id
	global exported_menu_model_id

	connection.unexport_action_group (exported_action_group_id)
	connection.unexport_menu_model (exported_menu_model_id)

	loop.quit()

def cancel (notification, action, data):
	if action == "cancel":
		print "Cancel"
	else:
		print "That should not have happened (cancel)!"

def accept (notification, action, data):
	if action == "accept":
		global pin
		print "PIN entered: " + pin
	else:
		print "That should not have happened (accept)!"

def simpin_changed (action, variant):
	global pin
	pin = variant.get_string();

def bus_acquired(bus, name):
	# menu
	unlock_menu = Gio.Menu();
	pin_unlock = Gio.MenuItem.new ("", "notifications.simunlock");
	pin_unlock.set_attribute_value ("x-canonical-type", GLib.Variant.new_string("com.canonical.snapdecision.pinlock"));
	unlock_menu.append_item (pin_unlock);

	# actions
	unlock_actions = Gio.SimpleActionGroup.new();
	action = Gio.SimpleAction.new_stateful("simunlock", GLib.VariantType.new("s"), GLib.Variant.new_string(""));
	action.connect ("change-state", simpin_changed);
	unlock_actions.insert (action);

	global connection
	connection = bus

	global exported_action_group_id
	exported_action_group_id = connection.export_action_group(SIM_UNLOCK_ACTION_PATH, unlock_actions)

	global exported_menu_model_id
	exported_menu_model_id = connection.export_menu_model(SIM_UNLOCK_MENU_PATH, unlock_menu)

def pushNotification (title, body, icon):
	n = Notify.Notification.new(title, body, icon);

	# NOTE: the order in which actions are added is important... positive
	# always comes first!
	n.add_action ("accept", "Accept", accept, None, None);
	n.add_action ("cancel", "Cancel", cancel, None, None);

	# create the menu-model
	menu_model_actions = GLib.VariantBuilder.new (GLib.VariantType.new ("a{sv}")); 
	entry = GLib.Variant.new_dict_entry(GLib.Variant.new_string("notifications"),
			GLib.Variant.new_variant(GLib.Variant.new_string(SIM_UNLOCK_ACTION_PATH)));
	menu_model_actions.add_value (entry);

	menu_model_paths = GLib.VariantBuilder.new (GLib.VariantType.new ("a{sv}"));
	entry = GLib.Variant.new_dict_entry(GLib.Variant.new_string("busName"),
			GLib.Variant.new_variant(GLib.Variant.new_string(APPLICATION_ID)));
	menu_model_paths.add_value (entry);
	entry = GLib.Variant.new_dict_entry(GLib.Variant.new_string("menuPath"),
			GLib.Variant.new_variant(GLib.Variant.new_string(SIM_UNLOCK_MENU_PATH)));
	menu_model_paths.add_value (entry);
	entry = GLib.Variant.new_dict_entry(GLib.Variant.new_string("actions"),
			GLib.Variant.new_variant(menu_model_actions.end()));
	menu_model_paths.add_value (entry);

	n.set_hint ("x-canonical-private-menu-model", menu_model_paths.end ());

	# indicate to the notification-daemon, that we want to use snap-decisions
	n.set_hint_string ("x-canonical-snap-decisions", "true");
	n.set_hint_string ("x-canonical-private-button-tint", "true");

	Gio.bus_own_name(Gio.BusType.SESSION, APPLICATION_ID, 0, bus_acquired, None, None)

	return n

if __name__ == '__main__':
	if not Notify.init("sd-example-simunlock"):
		sys.exit (1)

	connection = None
	exported_menu_model_id = 0
	exported_action_group_id = 0
	pin = "0000"

	loop = GLib.MainLoop()
	n = pushNotification ("Unlock SIM-card", "", "")
	n.connect('closed', quit_callback, loop)
	n.show ()

	loop.run()
