#!/usr/bin/python

filein = 'firefly_mmo.jpg'

##
# get file details
#

import Image
import sys
import time
import os.path

im = Image.open ( filein )

print im.format, im.size


##
# building mapping of RGB888 to RGBX
#
import array

redmap = array.array ( 'B', [0]*256 ) # [0] * ( im.size[0] * im.size[1] ) )
greenmap = array.array ( 'B', [0]*256 ) # [0] * ( im.size[0] * im.size[1] ) )
bluemap = array.array ( 'B', [0]*256 ) # [0] * ( im.size[0] * im.size[1] ) )

index = 0
for i in range ( 4 ):
    for r in range ( 4 ):
        for g in range ( 4 ):
            for b in range ( 4 ):

                # map 0-255 back to RGBX where each of R, G, B are values in the 8bit colour space
                # ... 0, 21, 42, 63 * 4 brightnesses

                redmap [ index ] = ( r * 21 * ( i + 1 ) )
                greenmap [ index ] = ( g * 21 * ( i + 1 ) )
                bluemap [ index ] = ( b * 21 * ( i + 1 ) )

                index += 1

print "max index:", index
print "redmap: ", redmap


##
# build lookup table: map RGBX to RGBX-low-depth
#

mapped = array.array ( 'B', [0] * index )

index = 0
for i in range ( 4 ):
    for r in range ( 4 ):
        for g in range ( 4 ):
            for b in range ( 4 ):

                mapped [ index ] = ( r * (i+1) ) + ( g * 4 * (i+1) ) + ( b * 16 * (i+1) )

                index += 1

print mapped

##
# convert RGB888 image to RGBX image
#

for y in range ( im.size [ 1 ] ):
    for x in range ( im.size [ 0 ] ):

        # print im.getpixel ( (x,y) )

        pass

##
# save the constructed image
#

path, ext = os.path.splitext ( filein )

# im.save ( path + '.png' )

sys.exit ( 0 )
