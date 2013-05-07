all:	demo

CC=avr-gcc
LD=avr-gcc
CFLAGS=-g -Os -mmcu=atmega644

# avr-gcc -g -Os -mmcu=atmega644 -c demo.c
# avr-gcc -g -mmcu=atmega8 -o demo.elf demo.o
# avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex
# sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex

clean:
	rm demo.o
	rm demo.elf
	rm demo.hex
	rm demo.map

	rm compat.o

demo:	demo.o compat.o
	avr-gcc -g -mmcu=atmega8 -o demo.elf demo.o

	avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex

deploy:
	sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex
