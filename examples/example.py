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
##    chmod +x example.py
##    ./example.py
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

import pynotify

# even in Python this is globally nasty :), do something nicer in your own code
capabilities = {'actions':                              False,
		'body':                                 False,
		'body-hyperlinks':                      False,
		'body-images':                          False,
		'body-markup':                          False,
		'icon-multi':                           False,
		'icon-static':                          False,
		'sound':                                False,
		'sound-file':                           False,
		'suppress-sound':                       False,
		'image/svg+xml':                        False,
		'urgency':                              False,
		'value':                                False,
		'x-canonical-value-bar-tint':           False,
		'x-canonical-private-synchronous':      False,
		'x-canonical-private-icon-only':        False,
		'x-canonical-truncation':               False,
		'x-canonical-snap-decisions':           False,
		'x-canonical-snap-decisions-timeout':   False,
		'x-canonical-snap-decisions-swipe':     False,
		'x-canonical-switch-to-application':    False,
		'x-canonical-secondary-icon':           False,
		'x-canonical-private-affirmative-tint': False,
		'x-canonical-private-rejection-tint':   False,
		'x-canonical-private-menu-model':       False,
		'x-canonical-non-shaped-icon':          False}

def initCaps ():
	caps = pynotify.get_server_caps ()
	if caps is None:
	        print "Failed to receive server caps."
		sys.exit (1)

	for cap in caps:
		capabilities[cap] = True

def printCaps ():
	info = pynotify.get_server_info ()
	print "Name:          " + info["name"]
	print "Vendor:        " + info["vendor"]
	print "Version:       " + info["version"]
	print "Spec. Version: " + info["spec-version"]

	caps = pynotify.get_server_caps ()
	if caps is None:
	        print "Failed to receive server caps."
		sys.exit (1)

	print "Supported capabilities/hints:"
	if capabilities['actions']:
		print "\tactions"
	if capabilities['body']:
		print "\tbody"
	if capabilities['body-hyperlinks']:
		print "\tbody-hyperlinks"
	if capabilities['body-images']:
		print "\tbody-images"
	if capabilities['body-markup']:
		print "\tbody-markup"
	if capabilities['icon-multi']:
		print "\ticon-multi"
	if capabilities['icon-static']:
		print "\ticon-static"
	if capabilities['sound']:
		print "\tsound"
	if capabilities['sound-file']:
		print "\tsound-file"
	if capabilities['suppress-sound']:
		print "\tsuppress-sound"
	if capabilities['urgency']:
		print "\turgency"
	if capabilities['value']:
		print "\tvalue"
	if capabilities['x-canonical-value-bar-tint']:
		print "\tx-canonical-value-bar-tint"
	if capabilities['image/svg+xml']:
		print "\timage/svg+xml"
	if capabilities['x-canonical-private-synchronous']:
		print "\tx-canonical-private-synchronous"
	if capabilities['x-canonical-private-icon-only']:
		print "\tx-canonical-private-icon-only"
	if capabilities['x-canonical-truncation']:
		print "\tx-canonical-truncation"
	if capabilities['x-canonical-snap-decisions']:
		print "\tx-canonical-snap-decisions"
	if capabilities['x-canonical-snap-decisions-timeout']:
		print "\tx-canonical-snap-decisions-timeout"
	if capabilities['x-canonical-snap-decisions-swipe']:
		print "\tx-canonical-snap-decisions-swipe"
	if capabilities['x-canonical-switch-to-application']:
		print "\tx-canonical-switch-to-application"
	if capabilities['x-canonical-secondary-icon']:
		print "\tx-canonical-secondary-icon"
	if capabilities['x-canonical-private-affirmative-tint']:
		print "\tx-canonical-private-affirmative-tint"
	if capabilities['x-canonical-private-rejection-tint']:
		print "\tx-canonical-private-rejection-tint"
	if capabilities['x-canonical-private-menu-model']:
		print "\tx-canonical-private-menu-model"
	if capabilities['x-canonical-non-shaped-icon']:
		print "\tx-canonical-non-shaped-icon"

	print "Notes:"
	if info["name"] == "notify-osd":
		print "\tx- and y-coordinates hints are ignored"
		print "\texpire-timeout is ignored"
		print "\tbody-markup is accepted but filtered"
	else:
		print "\tnone"

def closedHandler (notification, loop):
	print "\"closed\"-handler called"
	loop.quit ()
