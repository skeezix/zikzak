all:	vgatest

DEVICE=atmega644

CC=avr-gcc
LD=avr-gcc

OPTIM=-O3# -Os
CFLAGS=${OPTIM} -mmcu=${DEVICE} # -g
LDFLAGS=-mmcu=${DEVICE} # -g

OBJS=main.o

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
	avr-gcc ${LDFLAGS} -o vgatest.elf ${OBJS}
	avr-objcopy -j .text -j .data -O ihex vgatest.elf vgatest.hex

deploy:
	sudo avrdude -p ${DEVICE} -c avrispmkII -P usb -U vgatest.hex
