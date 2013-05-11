all:	demo

CC=avr-gcc
LD=avr-gcc
CFLAGS=-g -Os -mmcu=atmega644

# avr-gcc -g -Os -mmcu=atmega644 -c demo.c
# avr-gcc -g -mmcu=atmega8 -o demo.elf demo.o
# avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex
# sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex

clean:
	rm -f demo.o
	rm -f demo.elf
	rm -f demo.hex
	rm -f demo.map
	rm -f compat.o

demo:	demo.o compat.o
	avr-gcc -g -mmcu=atmega8 -o demo.elf demo.o

	avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex

deploy:
	sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex
