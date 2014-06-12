#!/usr/bin/python

# example invocations:
# ./linedemo.py -d /dev/ttyUSB0 -n 10
# ./command-line/linedemo.py  -n 100 --filldemo --linedemo --clearpattern

import sys
import os
import zlib
import time

def send ( b ):
    ser.write ( b )

# command line args
#

from optparse import OptionParser

cmdparser = OptionParser()
cmdparser.add_option ( "-n", "--number", dest="linecount", help="number of lines/boxes", default=10 )
cmdparser.add_option ( "-b", "--baud", dest="baudrate", help="baud rate (9600, 38400)", default=38400 )
cmdparser.add_option ( "-d", "--device", dest="device",   help="device name (fully qualified path)", default='/dev/ttyUSB0' )
cmdparser.add_option ( "-v", "--verbose", dest="verbose", help="verbosity level (1 is debug logs)", default=0 )
cmdparser.add_option ( "", "--linedemo", dest="linedemo",  action="store_true", help="include line demo in outputs", default=False )
cmdparser.add_option ( "", "--filldemo", dest="filldemo", action="store_true", help="include filled box demo in outputs", default=False )
cmdparser.add_option ( "", "--clearpattern", dest="clearpattern", action="store_true", help="include clear-to-pattern", default=False )

(cmdoptions, cmdargs) = cmdparser.parse_args()

# dump argument summary
#

print "Line count:\t", cmdoptions.linecount
print "Port device:\t", cmdoptions.device

if not cmdoptions.device:
    print "Missing device .."
    sys.exit()

# preamble
#
import serial
import random

# baud was 9600 works
# 38400 works to atmega644 20MHz
ser = serial.Serial ( port = cmdoptions.device, baudrate = cmdoptions.baudrate, timeout = 2.05, rtscts = True )

if not ser:
    print "Couldn't open serial port", cmdoptions.device
    sys.exit()

#send ( "ohai\r" )
#wait_for_ready()

# do it
#

if cmdoptions.linedemo:
    x = y = lx = ly = 0
    for n in range ( int ( cmdoptions.linecount ) + 1 ): # +1 since first is consumed as starting point

        x = random.randrange ( 10, 256 )
        y = random.randrange ( 10, 182 )
        #x = random.randrange ( ord('A'), ord('Z') )
        #y = random.randrange ( ord('A'), ord('Z') )

        c = random.randrange ( 0, 0b00111111 )
        #c = random.randrange ( ord('1'), ord('9') )

        if lx != 0:
            cmd = 'DL' + chr(c) + chr(lx) + chr(ly) + chr(x) + chr(y) + '\r'
            if int ( cmdoptions.verbose ) > 0:
                print 'Command ' + str(n) + ': ' + cmd
            send ( cmd )

        lx = x
        ly = y

if cmdoptions.filldemo:
    x = y = lx = ly = 0
    for n in range ( int ( cmdoptions.linecount ) + 1 ): # +1 since first is consumed as starting point

        x = random.randrange ( 10, 200 )
        y = random.randrange ( 10, 120 )
        lx = random.randrange ( 5, 250 - x )
        ly = random.randrange ( 5, 180 - y )

        c = random.randrange ( 0, 0b00111111 )
        #c = random.randrange ( ord('1'), ord('9') )

        cmd = 'DF' + chr(c) + chr(lx) + chr(ly) + chr(x) + chr(y) + '\r'
        if int ( cmdoptions.verbose ) > 0:
            print 'Command ' + str(n) + ': ' + cmd
        send ( cmd )

if cmdoptions.clearpattern:

        cmd = 'DP' + '\r'
        if int ( cmdoptions.verbose ) > 0:
            print 'Command ' + str(n) + ': ' + cmd
        send ( cmd )

# done
#

ser.close()
sys.exit()
