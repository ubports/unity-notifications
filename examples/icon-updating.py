#!/usr/bin/python

################################################################################
##3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
##      10        20        30        40        50        60        70        80
##
## Info: 
##    Example of how to use libnotify correctly
##
## Run:
##    chmod +x icon-updating.py
##    ./icon-updating.py
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
import gtk

if __name__ == '__main__':
	if not pynotify.init ("icon-updating"):
		sys.exit (1);

	# call this so we can savely use capabilities dictionary later
	example.initCaps ();

	# show what's supported
	example.printCaps ();

	# create new notification, set icon using hint "image_path"
	n = pynotify.Notification ("Test 1/3",
                                   "Set icon via hint \"image_path\" to logo-icon.");
	n.set_hint_string ("image_path",
                           "/usr/share/icons/Humanity/places/64/distributor-logo.svg");
	n.show ();
	time.sleep (4); # wait a bit

	# update notification using hint image_data
	n.clear_hints ();
	n.update ("Test 2/3",
                  "Set icon via hint \"image_data\" to avatar-photo.");
	pixbuf = gtk.gdk.pixbuf_new_from_file ("../icons/avatar.png");
	n.set_icon_from_pixbuf (pixbuf);
	n.show ();
	time.sleep (4); # wait a bit

	# update notification using icon-parameter directly
	n.clear_hints ();
	n.update ("Test 3/3",
                  "Set icon via icon-parameter directly to totem-icon.",
                  "totem");
	n.show ();
