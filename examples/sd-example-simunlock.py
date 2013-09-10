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

APPLICATION_ID = 'com.canonical.indicator.network'
SIM_UNLOCK_MENU_PATH = '/com/canonical/indicator/network/unlocksim'
SIM_UNLOCK_ACTION_PATH = '/com/canonical/indicator/network/unlocksim'

def quit_callback(notification, loop):
	loop.quit()

def action_decline_1 (notification, action, data):
	if action == "action_decline_1":
		print "Decline"
	else:
		print "That should not have happened (action_decline_1)!"

def action_accept (notification, action, data):
	if action == "action_accept":
		print "Accepting call"
	else:
		print "That should not have happened (action_accept)!"

def pushNotification (title, body, icon):
	n = Notify.Notification.new(title, body, icon);

	# NOTE: the order in which actions are added is important... positive
	# always comes first!
	n.add_action ("action_accept",    "Accept", action_accept, None);
	n.add_action ("action_decline_1", "Decline", action_decline_1, None);

	# create the menu-model
	menu_model_actions = GLib.VariantBuilder.new (GLib.VariantType.new ("a{sv}")); 
	entry = GLib.Variant.new_dict_entry(GLib.Variant.new_string("notifications"),
			GLib.Variant.new_variant(GLib.Variant.new_string(SIM_UNLOCK_ACTION_PATH)));
	menu_model_actions.add_value (entry);

	# indicate to the notification-daemon, that we want to use snap-decisions
	n.set_hint_string ("x-canonical-snap-decisions", "true");
	n.set_hint_string ("x-canonical-private-button-tint", "true");
	n.set_hint_string ("x-canonical-dbus-name", APPLICATION_ID);
	n.set_hint ("x-canonical-actions-path", menu_model_actions.end());
	n.set_hint_string ("x-canonical-menu-path", SIM_UNLOCK_MENU_PATH);

	return n

if __name__ == '__main__':
	if not Notify.init("sd-example-simunlock"):
		sys.exit (1)

	loop = GLib.MainLoop()
	n = pushNotification ("Unlock SIM-card", "", "")
	n.connect('closed', quit_callback, loop)
	n.show ()

	loop.run()
