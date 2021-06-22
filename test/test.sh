#!/bin/bash

#echo -ne "\x1" > /dev/hidg0 # UP
#echo -ne "\0" > /dev/hidg0 # RELEASE

echo -ne "\x40" > /dev/hidg0 # UP
echo -ne "\0" > /dev/hidg0 # RELEASE

#echo -ne "\x2" > /dev/hidg0 # DOWN
#echo -ne "\0" > /dev/hidg0 # RELEASE

#echo -ne "\x4" > /dev/hidg0 # MUTE
#echo -ne "\0" > /dev/hidg0 # RELEASE

