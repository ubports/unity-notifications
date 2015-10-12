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
##    chmod +x summary-only.py
##    ./summary-only.py
##
## Copyright 2009 Canonical Ltd.
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
import pynotify
import gobject
import example

if __name__ == '__main__':
	if not pynotify.init ("summary-only"):
		sys.exit (1)

	# call this so we can savely use capabilities dictionary later
	example.initCaps ()

	# show what's supported
	example.printCaps ()

	loop = gobject.MainLoop ()

	# try the summary-only case
	n = pynotify.Notification ("Summary-only",
				   "")
	n.show ()
	n.connect ("closed", example.closedHandler, loop)
	loop.run ()
