#!/bin/sh

##############################################################################
##
## Copyright 2015-2016 Markus Prasser
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

for i in 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18; do
echo "client$i"
ssh -T ewfuser@client$i <<THEEND
ln -s /home/ewfuser/.wine/drive_c/Program\ Files/SpeedCrunch/speedcrunch.exe /home/ewfuser/.wine/drive_c/windows/system32/CALC.exe
ln -s /home/ewfuser/.wine/drive_c/Program\ Files/SpeedCrunch/speedcrunch.exe /home/ewfuser/.wine/drive_c/windows/system32/CALC.EXE
THEEND
done
