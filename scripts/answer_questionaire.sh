#!/bin/sh

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
