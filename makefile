CC=gcc
OBJS=src/gpio.c src/host.c src/rotary_encoder.c src/button.c src/led.c src/media.c
media: $(OBJS)
	$(CC) $^ -o media -l bcm2835

