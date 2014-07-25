#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly
##
## Run:
##    chmod +x sd-example-incoming-call-canceled.py
##    ./sd-example-incoming-call-canceled.py
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
		print "Hang up"
	else:
		print "That should not have happened (action_decline_1)!"

def action_decline_2 (notification, action):
	if action == "action_decline_2":
		print "Reject with SMS"
	else:
		print "That should not have happened (action_decline_2)!"

def action_decline_3 (notification, action):
	if action == "action_decline_3":
		print "Dude, my wife is here!"
	else:
		print "That should not have happened (action_decline_3)!"

def action_decline_4 (notification, action):
	if action == "action_decline_4":
		print "I'm sleeping."
	else:
		print "That should not have happened (action_decline_4)!"

def action_decline_5 (notification, action):
	if action == "action_decline_5":
		print "No time... I'm riding!"
	else:
		print "That should not have happened (action_decline_5)!"

def action_decline_6 (notification, action):
	if action == "action_decline_6":
		print "Send SMS..."
	else:
		print "That should not have happened (action_decline_6)!"

def action_accept (notification, action):
	if action == "action_accept":
		print "Picking up the phone"
	else:
		print "That should not have happened (action_accept)!"

def pushNotification (title, body, icon):
	n = pynotify.Notification (title, body, icon);

	# NOTE: the order in which actions are added is important... positive
	# always comes first!
	n.add_action ("action_accept",    "Pick up", action_accept);
	n.add_action ("action_decline_1", "Hang up", action_decline_1);
	n.add_action ("action_decline_2", "Reject with SMS", action_decline_2);
	n.add_action ("action_decline_3", "message:Dude, my wife is here!", action_decline_3);
	n.add_action ("action_decline_4", "message:I'm sleeping.", action_decline_4);
	n.add_action ("action_decline_5", "message:No time... I'm riding!", action_decline_5);
	n.add_action ("action_decline_6", "edit:Send SMS...", action_decline_6);

	# indicate to the notification-daemon, that we want to use snap-decisions
	n.set_hint_string ("x-canonical-snap-decisions", "true");
	n.set_hint_string ("x-canonical-secondary-icon", "incoming-call");

	# set the button-tint hint so that the right/positive button is tinted and
	# not using the stock clear-color
	n.set_hint_string ("x-canonical-private-affirmative-tint", "true");
	n.set_hint_string ("x-canonical-private-rejection-tint", "true");
	n.set_urgency (pynotify.URGENCY_CRITICAL)

	n.show ()
	return n

def hung_up_timeout (notification):
	print "Application is hanging up now"
	notification.close ()

if __name__ == '__main__':
	if not pynotify.init ("sd-example-incoming-call-canceled"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	# be nice and check for required capabilities
	if not example.capabilities['x-canonical-private-button-tint'] and not example.capabilities['x-canonical-snap-decisions']:
		sys.exit (2)

	loop = gobject.MainLoop ()
	n = pushNotification ("Incoming call",
	                      "Bro Coly\n+1 (0)555-27439",
	                      os.getcwd() + "/assets/avatar3.jpg")
	n.connect ("closed", example.closedHandler, loop)

	# simulate caller hanging up before phone is picked up
	timeout_id = gobject.timeout_add (3000, hung_up_timeout, n)

	loop.run ()
