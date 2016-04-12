#!/usr/bin/env python3

##############################################################################
##
## Copyright 2015-2016 Markus Prasser
##
## This file is part of the EcoLabLib.
##
##  EcoLabLib is free software: you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  EcoLabLib is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with EcoLabLib.  If not, see <http://www.gnu.org/licenses/>.
##
##############################################################################

import datetime
import os
import shutil
import stat
import sys
import tempfile

def main():
    currentDate = datetime.date.today()

    print("Backing up any interesting configuration file")

    # Lists of all directories and files which shall be backed up
    dirList = [ "/etc/postfix", "/etc/ssh" ]
    fileList = [ "/etc/apt/apt.conf.d/02periodic",
            "/etc/aliases",
            "/etc/apt/apt.conf.d/20auto-upgrades",
            "/etc/dhcp/dhcpd.conf",
            "/etc/fstab",
            "/etc/group",
            "/etc/hostname",
            "/etc/hosts",
            "/etc/mdadm/mdadm.conf",
            "/etc/network/interfaces",
            "/etc/iptables.sav",
            "/etc/NetworkManager/NetworkManager.conf",
            "/etc/passwd",
            "/etc/rsyncd.conf",
            "/etc/rsyslog.conf",
            "/etc/shadow",
            "/etc/apt/sources.list",
            "/etc/sysctl.conf",
            "/etc/updatedb.conf" ]

    # Create a temporary directory to store the data
    tempDir = tempfile.TemporaryDirectory()

    # Copy all given files and directories
    try:
        for dirItem in dirList:
            shutil.copytree( dirItem, tempDir.name + "/" + dirItem.split( '/' )[-1], symlinks = True )
            print( "Copied folder '" + dirItem + "'" )
        for fileItem in fileList:
            tempFile = shutil.copy( fileItem, str( tempDir.name ) )
            # os.chmod( tempFile, 0o644  )
            print( "Copied file '" + fileItem + "'" )
    except shutil.Error as error:
        print( "Error copying '" + error.args[ 0 ][ 0 ][ 0 ] + "' to '" + error.args[ 0 ][ 0 ][ 1 ] + "':\n\t" + error.args[ 0 ][ 0 ][ 2 ] )
        print( "Perhaps you should retry as root?" )
        tempDir.cleanup()
        sys.exit( 1 )
    except PermissionError as error:
        sys.stderr.write( error.strerror )
        tempDir.cleanup()
        sys.exit( 1 )

    # Create the archive in the current directory
    shutil.make_archive( str( "settings_files" + currentDate.isoformat() + ".tar.gz" ), "gztar", os.getcwd(), tempDir.name )

    # Destroy the temporary directory to clean up all data
    tempDir.cleanup()

if __name__ == "__main__":
    main()

# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4
