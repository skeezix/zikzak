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
cmdparser.add_option ( "", "--srdemo", dest="srdemo", action="store_true", help="include sprite-receive demo in outputs", default=False )
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
ser = serial.Serial ( port = cmdoptions.device, baudrate = cmdoptions.baudrate, timeout = 0.10, rtscts = True )

if not ser:
    print "Couldn't open serial port", cmdoptions.device
    sys.exit()

#send ( "ohai\r" )
#wait_for_ready()

# do it
#

# turn off debug serial responses
#
send ( 'LF\r' )
#send ( 'LO\r' )

# run demos..
#
if cmdoptions.linedemo:

    clockstart = time.clock()
    print "linedemo: start", clockstart

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

    while True:
        line = ser.readline()
        if line == "":
            break
        print "RESP:", line.strip()

    clockend = time.clock()
    testdur_s = ( clockend - clockstart ) / 1000000
    print "linedemo: end", clockend
    print "linedemo: duration: ", testdur_s, clockend - clockstart

if cmdoptions.filldemo:

    clockstart = time.clock()
    print "filldemo: start", clockstart

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

    while True:
        line = ser.readline()
        if line == "":
            break
        print "RESP:", line.strip()

    clockend = time.clock()
    testdur_s = ( clockend - clockstart ) / 1000000
    print "filldemo: end", clockend
    print "filldemo: duration: ", testdur_s

if cmdoptions.srdemo:
    id = 0
    w = 100
    h = 100
    i = 0

    cmd = 'SR' + chr(id) + chr(w) + chr(h)
    if int ( cmdoptions.verbose ) > 0:
        print 'Command: ' + cmd
    send ( cmd )

    f = open ( 'command-line/firefly_mmo.raw', 'r' )
    data = f.read ( 25 )
    send ( data )
    while True:
        print " .. reading ", i, i*30
        data = f.read ( 30 )
        if data == "":
            break
        send ( data )
        i += 1

        line = ser.readline()
        if line != "":
            print "RESP:", line.strip()


    f.close()
    send ( '\r' )
    print "done sending file\n"

if cmdoptions.clearpattern:

        cmd = 'DP' + '\r'
        if int ( cmdoptions.verbose ) > 0:
            print 'Command ' + str(n) + ': ' + cmd
        send ( cmd )

# done
#

ser.close()
sys.exit()
