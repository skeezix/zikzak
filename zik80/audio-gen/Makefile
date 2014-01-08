all:	pokey ym-hack

ATMEGA=atmega644

CC=avr-gcc
LD=avr-gcc
#CFLAGS=-g -Os -mmcu=${ATMEGA}
CFLAGS=-g -Os -mcall-prologues -mmcu=${ATMEGA}

# avr-gcc -g -Os -mmcu=atmega644 -c demo.c
# avr-gcc -g -mmcu=atmega8 -o demo.elf demo.o
# avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex
# sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex

# generate song dump
# xxd -i -l 500  song_hacked.ym  > ../song-hacked.c
# ./ym-hack ./ym/song_000.ym  > song_hacked.c

LIBOBJS=lib_ay-3-8912.o song_hacked.o
OBJS=main.o ${LIBOBJS}

clean:
	rm -f pokey.o ym-hack
	rm -f pokey.elf
	rm -f pokey.hex
	rm -f pokey.map
	rm -f pokey
	rm -f *~*
	rm -f ${OBJS}

pokey:	${OBJS}
	avr-gcc -g -mmcu=${ATMEGA} -o pokey.elf ${OBJS}
	avr-objcopy -j .text -j .data -O ihex pokey.elf pokey.hex

deploy:
	sudo avrdude -e -p ${ATMEGA} -c avrispmkII -P usb -U pokey.hex

fuses: # remove CKDIV, remove JTAG
	sudo avrdude -v -e -p atmega644 -c avrispmkII -P usb -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

ym-hack: ym-hack.c
	gcc -o ym-hack ym-hack.c
