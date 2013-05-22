#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly
##
## Run:
##    chmod +x update-notifications.py
##    ./update-notifications.py
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
import example

if __name__ == '__main__':
	if not pynotify.init ("update-notifications"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	# try the icon-summary-body case
	n = pynotify.Notification (
			"Inital notification (1. notification)",
			"This is the original content of this notification-bubble.",
			os.getcwd() + "/assets/avatar1.jpg")
	n.show ()
	time.sleep (2); # simulate app activity

	# update the current notification with new content
	n.update ("Updated notification (1. notification)",
		  "Here the same bubble with new title- and body-text, even the icon can be changed on the update.",
                  os.getcwd() + "/assets/avatar2.jpg")
	n.show ();
	time.sleep (4); # wait longer now

	# create a new bubble using the icon-summary-body layout
	n = pynotify.Notification (
			"Initial layout (2. notification)",
			"This bubble uses the icon-title-body layout with a secondary icon.",
			os.getcwd() + "/assets/avatar3.jpg")
        n.set_hint_string ("x-canonical-secondary-icon", os.getcwd() + "/assets/icon_message.png");
	n.show ()
	time.sleep (2); # simulate app activity

	# now update current bubble again, but change the layout
	n.clear_hints()
	n.update ("Updated layout (2. notification)",
		  "After the update we now have a bubble using the title-body layout.",
		  " ")
	n.show ()
