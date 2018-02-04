.PHONY: all clean program


CFLAGS := -mmcu=atmega168 -Os -Wall -Wextra -std=c11 -DF_CPU=8000000UL
OBJS := megahack.o

all: megahack

clean:
	rm -f megahack $(OBJS)

program:
	avrdude -cusbasp -pm168 -B2 -U flash:w:megahack.bin:r

megahack: $(OBJS)
	avr-gcc $(CFLAGS) -o $@ $(OBJS)

megahack.bin: megahack
	avr-objcopy -O binary -j .text $< $@

.c.o:	
	avr-gcc $(CFLAGS) -c -o $@ $<
