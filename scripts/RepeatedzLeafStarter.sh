#!/bin/bash

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

for i in {1..100}
do
	/usr/bin/wine /opt/zTree_3.5.1/zleaf.exe /server 127.0.0.1 /channel 0 /name testzLeaf
        sleep 3
        killall zleaf.exe
        sleep 1
        echo $i
done
