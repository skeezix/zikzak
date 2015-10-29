
all: libdebug2r_test

CFLAGS = -Wall -DTESTMAIN -g
LDFLAGS =

CC = gcc
LD = ld

libdebug2r_test:	libdebug2r.o
	${CC} -o libdebug2r_test libdebug2r.o

clean:
	rm -f *.o *~*
