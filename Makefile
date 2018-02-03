.PHONY: all clean program


CFLAGS := -mmcu=atmega168 -Os -Wall -Wextra -std=c11 -DF_CPU=8000000UL
OBJS := helloworld.o

all: helloworld

clean:
	rm -f helloworld $(OBJS)

program:
	avrdude -cusbasp -pm168 -B2 -U flash:w:helloworld.bin:r

helloworld: $(OBJS)
	avr-gcc $(CFLAGS) -o $@ $(OBJS)

helloworld.bin: helloworld
	avr-objcopy -O binary -j .text $< $@

.c.o:	
	avr-gcc $(CFLAGS) -c -o $@ $<
