CC=gcc
OBJS=src/gpio.c src/host.c src/rotary_encoder.c src/button.c src/led.c src/media.c
media_keys: $(OBJS)
	$(CC) $^ -o bin/media_keys -l bcm2835

install: media_keys
	cp bin/$^ /usr/bin

