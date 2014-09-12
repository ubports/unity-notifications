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
## Copyright 2014 Canonical Ltd.
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
	n = pynotify.Notification ("Volume", # for a11y-reasons supply something meaning full
							   "",      # this needs to be empty!
							   icon);
	n.set_hint_int32 ("value", value);
	n.set_hint_string ("x-canonical-private-synchronous", "true");
	n.set_hint_string ("x-canonical-non-shaped-icon", "true");
	n.show ()
	return n

def updateNotification (n, icon, value):
	n.clear_hints()
	n.update("Volume", # for a11y-reasons supply something meaning full
			 "", # this needs to be empty!
			 icon);
	n.set_hint_int32 ("value", value);
	n.set_hint_string ("x-canonical-private-synchronous", "true");
	n.set_hint_string ("x-canonical-non-shaped-icon", "true");
	n.show ()

if __name__ == '__main__':
	if not pynotify.init ("icon-value"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	# try the icon-value case
	if example.capabilities['x-canonical-private-synchronous'] and example.capabilities['x-canonical-non-shaped-icon'] and example.capabilities['value']:
		n = pushNotification ("audio-volume-muted", 0);
		time.sleep (1)
		updateNotification (n, "audio-volume-low", 33);
		time.sleep (1)
		updateNotification (n, "audio-volume-medium", 66);
		time.sleep (1)
		updateNotification (n, "audio-volume-high", 100);
		time.sleep (1)

		# trigger "overshoot"-effect
		updateNotification (n, "audio-volume-high", 100);
		time.sleep (1)
		updateNotification (n, "audio-volume-medium", 66);
		time.sleep (1)
		updateNotification (n, "audio-volume-low", 33);
		time.sleep (1)
		updateNotification (n, "audio-volume-muted", 0);

	else:
		print "The daemon does not support the x-canonical-private-icon-only, x-canonical-non-shaped-icon and value hints!"
