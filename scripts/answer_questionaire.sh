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
ssh -T expuser@client$i <<THEEND
DISPLAY=:0.0 xdotool mousemove 1176 308
sleep 0.2
DISPLAY=:0.0 xdotool click 1
sleep 0.2
DISPLAY=:0.0 xdotool mousemove 780 60
sleep 0.2
DISPLAY=:0.0 xdotool click 1
sleep 0.2
DISPLAY=:0.0 xdotool type "client$i"
sleep 0.2
DISPLAY=:0.0 xdotool mousemove 812 86
sleep 0.2
DISPLAY=:0.0 xdotool click 1
sleep 0.2
DISPLAY=:0.0 xdotool type "client$i"
sleep 0.2
DISPLAY=:0.0 xdotool mousemove 1024 124
sleep 0.2
DISPLAY=:0.0 xdotool click 1
sleep 0.2
THEEND
done
