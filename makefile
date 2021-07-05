CC=gcc
OBJS=src/gpio.c src/host.c src/rotary_encoder.c src/button.c src/led.c src/media.c
media_keys: $(OBJS)
	$(CC) $^ -o bin/media_keys -l bcm2835

install: 
	cp bin/media_keys /usr/bin
	cp bin/isticktoit_usb /usr/bin

