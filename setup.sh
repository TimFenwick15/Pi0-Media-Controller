#!/bin/bash

### NOT YET TESTED - lines only performed manually ###

# Usage: 
# Copy this script, and isticktoit_usb to the home directory of your Raspberry Pi
# $ sudo ./setup.sh

echo "dtoverlay=dwc2" | tee -a /boot/config.txt
echo "dwc2" | tee -a /etc/modules
echo "libcomposite" | tee -a /etc/modules
touch /usr/bin/isticktoit_usb
chmod +x /usr/bin/isticktoit_usb

# Add this line to /etc/rc.local above the exit 0 (uncomment the line first)
# /usr/bin/isticktoit_usb # libcomposite configuration

cp isticktoit_usb /usr/bin/isticktoit_usb

echo Done, now reboot

