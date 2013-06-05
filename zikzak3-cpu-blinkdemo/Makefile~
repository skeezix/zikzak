all:	demo

ATMEGA=atmega128
#ATMEGA=atmega644

CC=avr-gcc
LD=avr-gcc
#CFLAGS=-g -Os -mmcu=${ATMEGA}
CFLAGS=-g -Os -mmcu=${ATMEGA}

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

demo:	demo.o# compat.o
	avr-gcc -g -mmcu=${ATMEGA} -o demo.elf demo.o

	avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex

deploy:
	sudo avrdude -p ${ATMEGA} -c avrispmkII -P usb -U demo.hex
