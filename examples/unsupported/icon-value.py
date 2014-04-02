#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly
##
## Run:
##    chmod +x icon-value.py
##    ./icon-value.py
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

import sys
import time
import pynotify
import example

def pushNotification (icon, value):
	n = pynotify.Notification ("Brightness", # for a11y-reasons supply something meaning full
				   "",           # this needs to be empty!
				   icon);
	n.set_hint_int32 ("value", value);
	n.set_hint_string ("x-canonical-private-synchronous", "true");
	n.show ()
	time.sleep (1)

if __name__ == '__main__':
	if not pynotify.init ("icon-value"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	# try the icon-value case
	if example.capabilities['x-canonical-private-icon-only']:
		pushNotification ("notification-keyboard-brightness-low", 25);

		pushNotification ("notification-keyboard-brightness-medium", 50);

		pushNotification ("notification-keyboard-brightness-high", 75);

		pushNotification ("notification-keyboard-brightness-full", 100);

		# trigger "overshoot"-effect
		pushNotification ("notification-keyboard-brightness-full", 101);

		pushNotification ("notification-keyboard-brightness-high", 75);

		pushNotification ("notification-keyboard-brightness-medium", 50);

		pushNotification ("notification-keyboard-brightness-low", 25);

		pushNotification ("notification-keyboard-brightness-off", 0);

		# trigger "undershoot"-effect
		pushNotification ("notification-keyboard-brightness-off", -1);
	else:
		print "The daemon does not support the x-canonical-private-icon-only hint!"

