#!/usr/bin/env python3

import time

def write_report(report):
    with open('/dev/hidg0', 'rb+') as fd:
        fd.write(report.encode())

#
# Key defines
#

A_CHAR = chr(4)
NULL_CHAR = chr(0)
PLAY_PAUSE = chr(0xCD)
#PLAY_PAUSE = chr(0xE8)
MUTE = chr(0xE2)
#MUTE = chr(0x7F)
VOL_UP = chr(0xE9)
#VOL_UP = chr(0x80)
COLLECTION = chr(0x01)
RISING_EDGE = chr(0x1)
FALLING_EDGE = chr(0x0)
PLAY = chr(0xB0)
STOP = chr(0xB7)

VOL_UP_CORRECT = chr(0x20)

#
# Typing
#

#time.sleep(3)
#write_report(NULL_CHAR + chr(1) + chr(0xE9) * 5 )

write_report(chr(0x20))
write_report(NULL_CHAR)

#time.sleep(2)
#write_report(NULL_CHAR*8)
"""
# Press a
write_report(NULL_CHAR*2+chr(4)+NULL_CHAR*5)
# Release keys
write_report(NULL_CHAR*8)
# Press SHIFT + a = A
write_report(chr(32)+NULL_CHAR+chr(4)+NULL_CHAR*5)

# Press b
write_report(NULL_CHAR*2+chr(5)+NULL_CHAR*5)
# Release keys
write_report(NULL_CHAR*8)
# Press SHIFT + b = B
write_report(chr(32)+NULL_CHAR+chr(5)+NULL_CHAR*5)

# Press SPACE key
write_report(NULL_CHAR*2+chr(44)+NULL_CHAR*5)

# Press c key
write_report(NULL_CHAR*2+chr(6)+NULL_CHAR*5)
# Press d key
write_report(NULL_CHAR*2+chr(7)+NULL_CHAR*5)

# Press RETURN/ENTER key
write_report(NULL_CHAR*2+chr(40)+NULL_CHAR*5)

# Press e key
write_report(NULL_CHAR*2+chr(8)+NULL_CHAR*5)
# Press f key
write_report(NULL_CHAR*2+chr(9)+NULL_CHAR*5)
"""

# Release all keys
#write_report(NULL_CHAR*8)
