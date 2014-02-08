all:	sender

#ATMEGA=atmega128
ATMEGA=atmega644

CC=avr-gcc
LD=avr-gcc
#CFLAGS=-g -Os -mmcu=${ATMEGA}
CFLAGS=-g -Os -mmcu=${ATMEGA}

# avr-gcc -g -Os -mmcu=atmega644 -c demo.c
# avr-gcc -g -mmcu=atmega8 -o demo.elf demo.o
# avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex
# sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex

clean:
	rm -f sender.o
	rm -f sender.elf
	rm -f sender.hex
	rm -f sender.map
	rm -f *~*

sender:	sender.o
	avr-gcc -g -mmcu=${ATMEGA} -o sender.elf sender.o

	avr-objcopy -j .text -j .data -O ihex sender.elf sender.hex

deploy:
	sudo avrdude -p ${ATMEGA} -c avrispmkII -P usb -U sender.hex
