all:	vgatest

DEVICE=atmega644
DEVICEDUDE=m644

CC=avr-gcc
LD=avr-gcc
CFLAGS=-g -Os -mmcu=${DEVICE}

OBJS=main.o

# avr-gcc -g -Os -mmcu=atmega644 -c demo.c
# avr-gcc -g -mmcu=atmega644 -o demo.elf demo.o
# avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex
# sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex

dump_map:
	avr-gcc -g -mmcu=${DEVICE} -o vgatest.elf -Wl,-Map,vgatest.map main.c

dump_asm:
	avr-gcc ${CFLAGS} -S -c main.c

dump_casm: # both C and asm
	avr-gcc ${CFLAGS} -c -g -Wa,-a,-ad main.c > main.casm

clean:
	rm -f ${OBJS}
	rm -f vgatest.elf
	rm -f vgatest.hex
	rm -f vgatest.map
	rm -f main.s
	rm -f main.casm
	rm -f *~*

vgatest:	${OBJS}
	avr-gcc -g -mmcu=${DEVICE} -o vgatest.elf ${OBJS}
	avr-objcopy -j .text -j .data -O ihex vgatest.elf vgatest.hex

deploy:
	sudo avrdude -p ${DEVICEDUDE} -c avrispmkII -P usb -U vgatest.hex
