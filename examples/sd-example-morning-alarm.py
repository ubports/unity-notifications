#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly
##
## Run:
##    chmod +x sd-example-morning-alarm.py
##    ./sd-example-morning-alarm.py
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

def action_snooze (notification, action):
	if action == "snooze":
		print "You want to snooze some more you lazy bastard!"
	else:
		print "That should not have happened (action_snooze)!"

def action_ok (notification, action):
	if action == "ok":
		print "Getting up is the right thing to do... good for you!"
	else:
		print "That should not have happened (action_ok)!"

def pushNotification (title, body, icon):
	n = pynotify.Notification (title, body, icon);

	# NOTE: the order in which actions are added is important... positive
	# always comes first!
	n.add_action ("ok",     "Ok",     action_ok);
	n.add_action ("snooze", "Snooze", action_snooze);

	# indicate to the notification-daemon, that we want to use snap-decisions
	n.set_hint_string ("x-canonical-snap-decisions", "true");
	n.set_hint_string ("x-canonical-noshape-icon", "true");

	n.show ()
	return n

if __name__ == '__main__':
	if not pynotify.init ("sd-example-morning-alarm"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	# be nice and check for required capabilities
	if not example.capabilities['x-canonical-snap-decisions']:
		sys.exit (2)

	loop = gobject.MainLoop ()
	n = pushNotification ("Morning alarm",
                          "It's 6:30... time to get up!",
                          os.getcwd() + "/assets/notification-weekday-alarm.svg")
	n.connect ("closed", example.closedHandler, loop)
	
	loop.run ()
