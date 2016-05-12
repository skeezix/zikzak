all:	ustest

MEGA=atmega644 #atmega644 #atmega1284

CC=avr-gcc
#CC=/usr/local/avr/bin/avr-gcc # 4.8.1 custom build
LD=avr-gcc
CFLAGS=-g -Os -mmcu=${MEGA} -I/usr/local/include

OBJS=main.o uart.o

clean:
	rm -f ${OBJS}
	rm -f ustest.elf
	rm -f ustest.hex
	rm -f ustest.map
	rm -f *~*
	rm -f mc_sabertooth

ustest:	${OBJS}
	avr-gcc -g -mmcu=${MEGA} -o ustest.elf ${OBJS}
	avr-objcopy -j .text -j .data -O ihex ustest.elf ustest.hex

deploy:
	#sudo avrdude -p ${MEGA} -c dragon_isp -P usb -U ustest.hex
	sudo avrdude -p ${MEGA} -c avrispmkII -P usb -U ustest.hex
