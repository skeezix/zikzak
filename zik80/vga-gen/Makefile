all:	zikgpu

#ATMEGA=atmega1284p
ATMEGA=atmega644

#IGNORE_SIGNATURE=-F
IGNORE_SIGNATURE=

CC=avr-gcc
CXX=avr-g++
LD=avr-gcc
#CFLAGS=-g -Os -mmcu=${ATMEGA}
CFLAGS=-g -Os -mcall-prologues -mmcu=${ATMEGA}
CXXFLAGS=-g -Os -mcall-prologues -mmcu=${ATMEGA}

# avr-gcc -g -Os -mmcu=atmega644 -c demo.c
# avr-gcc -g -mmcu=atmega8 -o demo.elf demo.o
# avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex
# sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex

OBJS=main.o

clean:
	rm -f zikgpu.o
	rm -f zikgpu.elf
	rm -f zikgpu.hex
	rm -f zikgpu.map
	rm -f zikgpu
	rm -f *~*
	rm -f ${OBJS}

zikgpu:	${OBJS}
	avr-gcc -g -mmcu=${ATMEGA} -o zikgpu.elf ${OBJS}
	avr-objcopy -j .text -j .data -O ihex zikgpu.elf zikgpu.hex

deploy:
	sudo avrdude -p ${ATMEGA} -c avrispmkII -P usb -U zikgpu.hex ${IGNORE_SIGNATURE}
