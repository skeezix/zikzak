#!/usr/bin/python

# Command line:
# arg: a filename to an image (not-png!)
# Outputs a png-image after downcovnerting the colour space
# Outputs a piece of C code containing the image

import sys
import time
import os.path
import Image

filein = sys.argv [ 1 ]

##
# get file details
#

im = Image.open ( filein )

print im.format, im.size


##
# convert RGB888 image to RGBX image
#

for y in range ( im.size [ 1 ] ):
    for x in range ( im.size [ 0 ] ):

        r, g, b = im.getpixel ( (x,y) )
        # print "rgb(", r, ",", g, ",", b, ") "

        # verify nothing too high .. that we're past the 8bpp
        if r > 255 or g > 255 or b > 255:
            print "Colour depth error"
            sys.exit ( 0 )

        # determine if 'bright' or not
        bright = 0

        if r + g + b > ( (256*3)/2 ):
            #bright = 1
            pass

        if r >= 128 or g >= 128 or b >= 128:
            bright = 1

        # if x < im.size [ 0 ] / 2: # force half to always dark, for comparison
        #   bright = 0

        # brightness multiplier
        if bright:
            bright = 1 # 64
        else:
            bright = 1 # 32

        # scale colour, allowing for brightness
        r = int ( r / 64 ) * bright
        g = int ( g / 64 ) * bright
        b = int ( b / 64 ) * bright

        if True or y < im.size [ 1 ] / 2: # allow half to process, other half to stay original, for comparison
            im.putpixel ( (x,y), (r,g,b) )

        pass

##
# save the constructed image
#

path, ext = os.path.splitext ( filein )

if ext == '.png':
    print "Already png file .. skipping"
    sys.exit ( 0 )

im.save ( path + '.png' )

# spit out raw image
f = open ( path + ".raw", "w" )

ymax = min ( im.size [ 1 ], 100 )
xmax = min ( im.size [ 0 ], 100 )
for y in range ( ymax ):
    for x in range ( xmax ):
        r, g, b = im.getpixel ( (x,y) )
        c = int(r) + int ( g << 2 ) + int ( b << 4 )
        f.write ( chr(c) )

f.close()

# exeunt
#
sys.exit ( 0 )
