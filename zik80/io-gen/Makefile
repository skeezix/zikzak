all:	io

ATMEGA=atmega644

CC=avr-gcc
LD=avr-gcc
#CFLAGS=-g -Os -mmcu=${ATMEGA}
CFLAGS=-g -Os -mcall-prologues -mmcu=${ATMEGA}

# avr-gcc -g -Os -mmcu=atmega644 -c demo.c
# avr-gcc -g -mmcu=atmega8 -o demo.elf demo.o
# avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex
# sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex

LIBOBJS=lib_keyb.o
OBJS=main.o ${LIBOBJS}

clean:
	rm -f io.o
	rm -f io.elf
	rm -f io.hex
	rm -f io.map
	rm -f io
	rm -f *~*
	rm -f ${OBJS}

io:	${OBJS}
	avr-gcc -g -mmcu=${ATMEGA} -o io.elf ${OBJS}
	avr-objcopy -j .text -j .data -O ihex io.elf io.hex

deploy:
	sudo avrdude -p ${ATMEGA} -c avrispmkII -P usb -U io.hex
