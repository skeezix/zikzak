#!/usr/bin/python

# example invocations:
# ./zikburner.py -m dump  -a 0 -l 20 -d /dev/ttyUSB0
# ./zikburner.py -m burn -d /dev/ttyUSB0 -f ../from-serial/lib_crc32.h -v

import sys
import os
import zlib
import time

def readline():
    b = None
    while True:
        c = ser.read()
        if c:
            if c == '\r':
                pass
            elif c == '\n':
                break
            else:
                if b == None:
                    b = c
                else:
                    b += c
        else:
            break

    if cmdoptions.verbose:
        print "--", b

    return b

def wait_for_ready ( waitstring = "+READY" ):
    failcount = 0
    while True:
        b = readline()
        if b == waitstring:
            return True
        elif not b:
            failcount += 1
            if failcount > 10:
                break
            else:
                #print "# Waiting..", failcount
                time.sleep ( 0.1 )
    return False

def send ( b ):
    if cmdoptions.verbose:
        print "++", b
    ser.write ( b )

def dump_to_buffer ( address, length ):

    send ( "dump " + address + " " + length + "\r" )

    buffer = []
    while True:
        b = readline()
        if b:
            ( a, v ) = b.split ( ':', 1 )
            a = a.strip()
            v = int ( v.strip(), 16 )
            buffer.append ( (a, v) )
        else:
            break

    if len ( buffer ) > 0:
        return buffer
    else:
        return None

def burnblock ( address, bits, blocksize ):

    send ( "receive " + str(blocksize) + "\r" )
    ser.write ( bits )
    wait_for_ready()

    #send ( "buffer\r" )
    #wait_for_ready()

    send ( 'burn ' + str ( address ) + "\r" )
    success = None
    if wait_for_ready ( "+BURNOK" ):
        success = True
    else:
        success = False
    wait_for_ready()

    return success

# command line args
#

from optparse import OptionParser

cmdparser = OptionParser()
cmdparser.add_option ( "-a", "--address", dest="address", help="address to begin dumping", default=None )
cmdparser.add_option ( "-l", "--length", dest="length",   help="length of dump", default=None )
cmdparser.add_option ( "-d", "--device", dest="device",   help="device name (fully qualified path)", default=None )
cmdparser.add_option ( "-f", "--file", dest="file",   help="file to burn (fully qualified)", default=None )
cmdparser.add_option ( "-m", "--mode", dest="mode",   help="Operation; one of 'dump', 'burn'", default=None )
cmdparser.add_option ( "-v", "--verbose", dest="verbose",   help="Display received dialog", action="store_true", default=False )

(cmdoptions, cmdargs) = cmdparser.parse_args()

# dump argument summary
#

print "Mode:\t", cmdoptions.mode
print "Port device:\t", cmdoptions.device

if not cmdoptions.mode:
    print "Missing mode (dump, burn, ...)"
    sys.exit()

if cmdoptions.mode == 'dump':
    if not cmdoptions.address or not cmdoptions.length or not cmdoptions.device:
        print "Missing arguments; need address, length, portname"
        sys.exit()
    print "Start address:\t", cmdoptions.address
    print "Length of dump:\t", cmdoptions.length

elif cmdoptions.mode == 'burn':
    if not cmdoptions.file or not cmdoptions.device:
        print "Missing arguments; need file, portname"
        sys.exit()
    print "Filename:\t", cmdoptions.file

    try:
        filesize = os.path.getsize ( cmdoptions.file )
    except:
        print "Bad filename", cmdoptions.file
        sys.exit()

    if filesize == 0:
        print "Bad file size", cmdoptions.file
        sys.exit()

# preamble
#
import serial

# baud was 9600
ser = serial.Serial ( port = cmdoptions.device, baudrate = 38400, timeout = 2.05, rtscts = True )

if not ser:
    print "Couldn't open serial port", cmdoptions.device
    sys.exit()

send ( "ohai\r" )
wait_for_ready()

send ( "charecho\r" )
wait_for_ready()

send ( "format\r" )
wait_for_ready()

# do it
#

if cmdoptions.mode == 'dump':

    b = dump_to_buffer ( cmdoptions.address, cmdoptions.length )
    wait_for_ready()

    if b:
        for i in b:
            print i [ 0 ] + ":\t" + str ( i [ 1 ] ) + "\t" + hex( i [ 1 ] ) + '\t' + chr ( i [ 1 ] ).strip()
    else:
        print "*** Dump error ***"

elif cmdoptions.mode == 'burn':

    hexfile = open ( cmdoptions.file, 'r' )

    '''
    crc32 = 0
    with open ( cmdoptions.file, 'r' ) as hexfile:
        bits = hexfile.read()
        crc32 = zlib.crc32 ( bits, 0 )
    '''

    print ">>> Starting the burn process..."

    starttime = int ( time.time() )

    success = None
    if filesize > 255:
        # loop across blocks

        counter = 0
        remainder = filesize
        address = 0

        while True:
            print int ( time.time() ) - starttime, "secs\tBurning block", counter, '(address', address, '- remaining', remainder, ')'

            bits = hexfile.read ( min ( 255, remainder ) )

            if bits == "":
                break # EOF

            success = burnblock ( address = address, bits = bits, blocksize = len ( bits ) )

            if not success:
                break

            address += min ( 255, remainder )
            remainder -= len ( bits )
            counter += 1

    else:
        # single block
        with open ( cmdoptions.file, 'r' ) as hexfile:
            bits = hexfile.read()
            success = burnblock ( address = 0, bits = bits, blocksize = filesize )

    if not success:
        print "    Burn *** FAILURE ***"
        sys.exit()

    hexfile.close()

    print ">>> Pull dump of device for comparison..."
    b = dump_to_buffer ( "0", str(filesize) )
    wait_for_ready()

    print ">>> Comparing..."

    with open ( cmdoptions.file, 'r' ) as hexfile:
        bits = hexfile.read()

    for counter in range ( filesize ):
        if ord ( bits [ counter ] ) != b [ counter ][ 1 ]:
            print "*** Burn ERROR *** Burn fails comparison at offset", counter
            print "                   Local " + str ( ord ( bits [ counter ] ) ) + " " + hex ( ord ( bits [ counter ] ) )
            print "                   Remote " + str ( b [ counter ][ 1 ] ) + " " + hex ( b [ counter ][ 1 ] )
            sys.exit()

    print ">>> Burn verification OK"

    #print "calc hex:", crc32, hex(crc32) ## why is this a different value from mcu? we end up having to do a dump-compare anyway I guess..

# done
#

ser.close()
sys.exit()
