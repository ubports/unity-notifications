#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly
##
## Run:
##    chmod +x normal-urgency.py
##    ./normal-urgency.py
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
import pynotify
import gobject
import example

if __name__ == '__main__':
	if not pynotify.init ("normal-urgency"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	loop = gobject.MainLoop ()

	# try the icon-summary-body case
	n = pynotify.Notification ("Cole Raby",
				   "Hey pal, what's up with the party "
				   "next weekend? Will you join me "
				   "and Anna?",
				   os.getcwd() + "/assets/avatar2.jpg")
	n.set_urgency (pynotify.URGENCY_NORMAL)

	n.show ()
	n.connect ("closed", example.closedHandler, loop)
	loop.run ()
