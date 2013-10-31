#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly and at the same time comply to
##    the new jaunty notification spec (read: visual guidelines)
##
## Run:
##    chmod +x sd-example-incoming-file.py
##    ./sd-example-incoming-file.py
##
## Copyright 2012 Canonical Ltd.
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

import sys
import time
import pynotify
import gobject
import example

def action_decline (notification, action):
	if action == "action_decline":
		print "You are rejecting to receive the file"
	else:
		print "That should not have happened (action_decline)!"

def action_accept (notification, action):
	if action == "action_accept":
		print "Accepting incoming file"
	else:
		print "That should not have happened (action_accept)!"

def pushNotification (title, body, icon):
	n = pynotify.Notification (title, body, icon);

	# NOTE: the order in which actions are added is important... positive
	# always comes first!
	n.add_action ("action_accept",  "Accept",  action_accept);
	n.add_action ("action_decline", "Decline", action_decline);

	# indicate to the notification-daemon, that we want to use snap-decisions
	n.set_hint_string ("x-canonical-snap-decisions", "true");
	n.set_hint_string ("x-canonical-non-shaped-icon", "true");

	n.show ()
	return n

if __name__ == '__main__':
	if not pynotify.init ("sd-example-incoming-file"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	# be nice and check for required capabilities
	if not example.capabilities['x-canonical-snap-decisions']:
		sys.exit (2)

	loop = gobject.MainLoop ()
	n = pushNotification ("Incoming file",
                          "Frank would like to send you the file: essay.pdf",
                          "document")
	n.connect ("closed", example.closedHandler, loop)

	loop.run ()
