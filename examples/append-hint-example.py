#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly with the new Qt/QML-based
##    implementation of NotifyOSD
##
## Run:
##    chmod +x append-hint-example.py
##    ./append-hint-example.py
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

def pushNotification (title, body, icon, secondaryIcon):
	n = pynotify.Notification (title, body, icon);
	n.set_hint_string ("x-canonical-append", "true");
	n.set_hint_string ("x-canonical-secondary-icon", secondaryIcon);
	n.show ()
	time.sleep (3) # simulate a user typing

if __name__ == '__main__':
	if not pynotify.init ("append-hint-example"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	# be nice and check for required capabilities
	if not example.capabilities['x-canonical-append']:
		sys.exit (2)

	# try the append-hint 
	if example.capabilities['x-canonical-append']:
		pushNotification ("Cole Raby",
				  "Hey Bro Coly!",
				  os.getcwd() + "/assets/avatar1.jpg",
				  os.getcwd() + "/assets/icon_facebook.png")

		pushNotification ("Cole Raby",
				  "What's up dude?",
				  os.getcwd() + "/assets/avatar1.jpg",
				  os.getcwd() + "/assets/icon_facebook.png")

		pushNotification ("Cole Raby",
				  "Did you watch the air-race in Oshkosh last week?",
				  os.getcwd() + "/assets/avatar1.jpg",
				  os.getcwd() + "/assets/icon_facebook.png")


		pushNotification ("Cole Raby",
				  "Phil owned the place like no one before him!",
				  os.getcwd() + "/assets/avatar1.jpg",
				  os.getcwd() + "/assets/icon_facebook.png")

		pushNotification ("Cole Raby",
				  "Did really everything in the race work according to regulations?",
				  os.getcwd() + "/assets/avatar1.jpg",
				  os.getcwd() + "/assets/icon_facebook.png")

		pushNotification ("Cole Raby",
				  "Somehow I think to remember Burt Williams did cut corners and was not punished for this.",
				  os.getcwd() + "/assets/avatar1.jpg",
				  os.getcwd() + "/assets/icon_facebook.png")

		pushNotification ("Cole Raby",
				  "Hopefully the referees will watch the videos of the race.",
				  os.getcwd() + "/assets/avatar1.jpg",
				  os.getcwd() + "/assets/icon_facebook.png")

		pushNotification ("Cole Raby",
				  "Burt could get fined with US$ 50000 for that rule-violation :)",
				  os.getcwd() + "/assets/avatar1.jpg",
				  os.getcwd() + "/assets/icon_facebook.png")

	else:
		print "The daemon does not support the x-canonical-append hint!"

