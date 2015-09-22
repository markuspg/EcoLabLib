#!/usr/bin/env python3

##############################################################################
##
## Copyright 2015 Markus Prasser
##
## This file is part of EcoLabLib..
##
##  EcoLabLib. is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  EcoLabLib. is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with EcoLabLib..  If not, see <http://www.gnu.org/licenses/>.
##
##############################################################################

import subprocess
import threading
import time

class ScreenshotThread( threading.Thread ):
    def __init__( self, argClientName ):
        super().__init__()
        self.clientName = argClientName

    def run( self ):
        currentTime = time.strftime( "%Y-%m-%d_%H-%M" )
        print( "Taking screenshot from client '{0}'".format( self.clientName ) )
        returnCode = screenshotProcess = subprocess.call( [ "/usr/bin/ssh", "-q", "-i", "/usr/share/ecolablib/id_rsa_user", "ewfuser@" + self.clientName, "DISPLAY=:0.0 import -window root /tmp/screenshot_{0}.{1}.png".format( self.clientName, currentTime ) ] )

        if not returnCode:
            print( "Copying the screenshot of client '{0}' to the server".format( self.clientName ) )
            screenshotProcess = subprocess.call( [ "/usr/bin/scp", "-q", "-i", "/usr/share/ecolablib/id_rsa_user", "ewfuser@" + self.clientName + ":/tmp/screenshot_{0}.{1}.png".format( self.clientName, currentTime ), "." ] )
        else:
            print( "Taking a screenshot on client '{0}' failed".format( self.clientName ) )

def main():
    clientNameList = [ "client01", "client02", "client03", "client04", "client05", "client06", "client07", "client08", "client09", "client10", "client11", "client12", "client13", "client14", "client15", "client16", "client17", "client18" ]
    for name in clientNameList:
        screenshotThread = ScreenshotThread( name )
        screenshotThread.start()

if __name__ == "__main__":
    main()

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
