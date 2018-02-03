.PHONY: all clean program


CFLAGS := -mmcu=atmega168 -Os -Wall -std=c11 -DF_CPU=8000000UL
OBJS := helloworld.o

all: helloworld

helloworld: $(OBJS)
	avr-gcc $(CFLAGS) -o $@ $(OBJS)

helloworld.bin: helloworld
	avr-objcopy -O binary -j .text $< $@

.c.o:	
	avr-gcc $(CFLAGS) -c -o $@ $<
