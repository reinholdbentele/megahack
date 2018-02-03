#!/bin/bash -e

PGM="$1"
avr-gcc -mmcu=atmega168 -Os -Wall -std=c11 -DF_CPU=8000000UL -o output $PGM
avr-objcopy -O binary -j .text output output.bin
avrdude -cusbasp -pm168 -B2 -U flash:w:output.bin:r