## Setting up Pi 0 W
### OS image
Download 'Raspberry Pi OS Lite' from: https://www.raspberrypi.org/software/operating-systems/

Unzip this.

Run `$ dmesg -w`

Insert an SD card into reader

Check the SD card device name. For me, `/dev/sdb`

dd is dangerous because if you point it at the wrong drive, it will happily overwrite it with whatever you send it. Using of=/dev/sda instead of of=/dev/sdb would brick your machine. Make sure you are happy with what the command will do before running.

Clear the SD card: `$ sudo dd if=/dev/zero of=/dev/sdb bs=1M status=progress`

Write the OS image: `$ sudo dd if=[name of OS .img file] of=/dev/sdb bs=1M status=progress`

I had to boot the Pi once before I could see the SD card contents on my computer, not sure why. We ideall want to do the following before booting the Pi.

RFKILL may block your Wifi on first boot. I tried editing /media/[username]/boot/cmdline.txt. Add to the end fo the line: `systemd.restore_state=0 rfkill.default_state=1`

`$ rfkill list all` showed Wifi unblocked.

Edit /media/[username]/boot/config.txt. Add to the end of the file: `enable_uart=1`

Run: `$ touch /media/[username]/boot/ssh`

Run before removing card: `$ sync`

### Serial console
Initially connected to the Pi over serial. I have an FTDI breakout connected to computer with pins:
- GND connected to Pi pin 6
- AD1 connected to Pi pin 8 (serial TX)
- AD0 connected to Pi pin 10 (serial RX)

The FTDI breakout was on USB0, so: `$ screen /dev/ttyUSB0 115200`

Then power on the Pi to get a shell.

Login with username pi, password raspberry. Change the password with: `$ passwd`

### Get ssh access
On the Pi, edit `/etc/wpa_supplicant/wpa_supplicant.conf`

Add the lines:

```
network={
    ssid="Your access point name"
    psk="Your access point password in plain text"
}
```

Not certain if you need to do this, but I also edited: `/etc/dhcpcd.conf`, adding the lines:
```
interface wlan0
static routers=[IP of your router]
```

Reboot: `$ sudo reboot now`

You should now have SSH access. You might catch the Pi IP in the boot text in your serial console. Otherwise, login again and run: `$ ifconfig`. The IP address should be visible under the wlan0 heading.

### Setup Pi
Run `$ sudo apt-get update && sudo apt-get upgrade`

Then follow the instructions in setup.py. It's intended to be copied to the Pi and run, but there's a line that needs adding (the comment about adding a line above exit 0)

## Making the Pi 0 a USB HID consumer device
HID (human interface device) is a standard for USB devices such as keyboards, defining the codes that translate to keypresses.

Any media keys on a keyboard are sent as a 'consumer' device, not as keyboard keys.

We want to make a Raspberry Pi 0 act as a media controller so we can connect buttons and a rotary encoder to control music/video etc.

This is described in the links below.

media.c is currently sending the media key bites. The report_desc defines each key we want to use. Then each media key becomes a bit in `report`, in the order they were defined. Eg, the first key defined is NEXT_TRACK. So sending bit 1 skips to the next track. The second key is PREV_TRACK, sent as the next bit: 2, or 0b00000010.

To build media.c: `$ make`

## Connecting to Computer Log
Check if the Pi has been recognised by the host with: `$ dmesg -w`

Then plug in the Pi.

The following output was reported for me:
```
288576.232419] usb 1-2: New USB device found, idVendor=1d6b, idProduct=0104, bcdDevice= 5.10
[288576.232425] usb 1-2: New USB device strings: Mfr=0, Product=0, SerialNumber=0
[288576.234759] input: HID 1d6b:0104 as /devices/pci0000:00/0000:00:14.0/usb1/1-2/1-2:1.0/0003:1D6B:0104.001A/input/input41
[288576.292403] hid-generic 0003:1D6B:0104.001A: input,hidraw0: USB HID v1.01 Device [HID 1d6b:0104] on usb-0000:00:14.0-2/input0
```

## Links
Working with RPi and SD cards on Linux: https://www.thirtythreeforty.net/posts/2020/01/mastering-embedded-linux-part-3-buildroot/

Followed for setup: https://randomnerdtutorials.com/raspberry-pi-zero-usb-keyboard-hid/

This seems to have been based on information here: http://www.isticktoit.net/?p=1383 This seems to be where the "isticktoit_usb" name that pops up in other similar tutorials came from.

Followed for making consumer device work: https://www.ekwbtblog.com/entry/2019/01/31/000000

Coppied the report defined towards the end of the thread: https://www.microchip.com/forums/m440956.aspx#441405 There are comments on that version explaining (partly) what each line does.

I think this is a tool for generating report_desc: https://www.usb.org/document-library/hid-descriptor-tool

Two versions of the USB org documentation: 
- https://www.usb.org/sites/default/files/documents/hut1_12v2.pdf
  - Section 3.4.1 talks about different kinds of controls
  - Section 15 lists the codes for each media control
- https://www.usb.org/sites/default/files/documents/hid1_11.pdf
  - Section 4.3 lists the Protocol codes we're using

Kernel documentation that sends you to usb.org: https://www.kernel.org/doc/Documentation/ABI/testing/configfs-usb-gadget

Might be useful for setting this up properly: https://www.elinux.org/images/e/ef/USB_Gadget_Configfs_API_0.pdf

