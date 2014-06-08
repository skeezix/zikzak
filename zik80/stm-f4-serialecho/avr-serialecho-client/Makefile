all:	serialclient

ATMEGA=atmega644

CC=avr-gcc
LD=avr-gcc
CFLAGS=-g -Os -mmcu=${ATMEGA}

APPOBJS = serialclient.o lib_serial.o

# avr-gcc -g -Os -mmcu=atmega644 -c demo.c
# avr-gcc -g -mmcu=atmega8 -o demo.elf demo.o
# avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex
# sudo avrdude -p m644 -c avrispmkII -P usb -U demo.hex

clean:
	rm -f serialclient.o
	rm -f serialclient.elf
	rm -f serialclient.hex
	rm -f serialclient.map
	rm -f *.o
	rm -f *~*

serialclient:	${APPOBJS}
	avr-gcc -g -mmcu=${ATMEGA} -o serialclient.elf ${APPOBJS}

	avr-objcopy -j .text -j .data -O ihex serialclient.elf serialclient.hex

deploy:
	sudo avrdude -p ${ATMEGA} -c avrispmkII -P usb -U serialclient.hex

fusedump:
	@echo "low-fuse"
	@sudo avrdude -p ${ATMEGA} -c avrispmkII -P usb -U lfuse:r:-:h -q 2>/dev/null
	@echo "high-fuse"
	@sudo avrdude -p ${ATMEGA} -c avrispmkII -P usb -U hfuse:r:-:h -q 2>/dev/null
	@echo "ext-fuse"
	@sudo avrdude -p ${ATMEGA} -c avrispmkII -P usb -U efuse:r:-:h -q 2>/dev/null
