#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly
##
## Run:
##    chmod +x sd-example-incoming-call.py
##    ./sd-example-incoming-call.py
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
import time
import pynotify
import gobject
import example

def action_decline_1 (notification, action):
	if action == "action_decline_1":
		print "Decline"
	else:
		print "That should not have happened (action_decline_1)!"

def action_decline_2 (notification, action):
	if action == "action_decline_2":
		print "\"Can't talk now, what's up?\""
	else:
		print "That should not have happened (action_decline_2)!"

def action_decline_3 (notification, action):
	if action == "action_decline_3":
		print "\"I call you back.\""
	else:
		print "That should not have happened (action_decline_3)!"

def action_decline_4 (notification, action):
	if action == "action_decline_4":
		print "Send custom message..."
	else:
		print "That should not have happened (action_decline_4)!"

def action_accept (notification, action):
	if action == "action_accept":
		print "Accepting call"
	else:
		print "That should not have happened (action_accept)!"

def pushNotification (title, body, icon):
	n = pynotify.Notification (title, body, icon);

	# NOTE: the order in which actions are added is important... positive
	# always comes first!
	n.add_action ("action_accept",    "Accept",                         action_accept);
	n.add_action ("action_decline_1", "Decline",                        action_decline_1);
	n.add_action ("action_decline_2", "\"Can't talk now, what's up?\"", action_decline_2);
	n.add_action ("action_decline_3", "\"I call you back.\"",           action_decline_3);
	n.add_action ("action_decline_4", "Send custom message...",         action_decline_4);

	# indicate to the notification-daemon, that we want to use snap-decisions
	n.set_hint_string ("x-canonical-snap-decisions", "true");
	n.set_hint_string ("x-canonical-private-button-tint", "true");
	n.set_hint_string ("x-canonical-secondary-icon", os.getcwd() + "/assets/icon_phone.png");
	n.set_urgency (pynotify.URGENCY_CRITICAL)

	n.show ()
	return n

if __name__ == '__main__':
	if not pynotify.init ("sd-example-incoming-call"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	# be nice and check for required capabilities
	if not example.capabilities['x-canonical-snap-decisions']:
		sys.exit (2)

	loop = gobject.MainLoop ()
	n = pushNotification ("Incoming call",
	                      "Frank Zappa\n+44 (0)7736 027340",
	                      os.getcwd() + "/assets/avatar4.jpg")
	n.connect ("closed", example.closedHandler, loop)

	loop.run ()
