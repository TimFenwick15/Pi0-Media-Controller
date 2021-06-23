CC=gcc
OBJS=src/rotary_encoder.c src/media.c
media: $(OBJS)
	$(CC) $^ -o media -l bcm2835

