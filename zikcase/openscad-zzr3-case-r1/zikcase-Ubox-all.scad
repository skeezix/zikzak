/* Case parts for Zikzak rev3
 * www.zikzak.ca
 */
 use <_fanholes.scad>;
 use <_assert.scad>;
 
 // ref
// colours: https://en.wikibooks.org/wiki/OpenSCAD_User_Manual/Transformations#color

// TODO:
// - FIX: INVERT
// - VERIFY: HEIGHT positions to actually be right for jacks, relative to height padding
// - FIX: so it actually fits together; sloppyness ratio? or just use scaling?
// - add cart hole 'sides' as guides for the cart to go down nicely into edge connector
// - 2 fastener bits per side instead of 1 ... problem is jacks

// render target
// - frontbacktop
// - leftrightbottom
// - all
//target = "frontbacktop";
//target = "leftrightbottom";
target = "all";

// scale? Use this to print out tiny versions for inspection prior to printing real ones
//scale_factor_x = 1;
//scale_factor_y = 1;
//scale_factor_z = 1;
scale_factor_x = 0.4;
scale_factor_y = 0.4;
scale_factor_z = 0.4;

// invert for printing?
invert_y = 1;

// fan holes?
fan_holes = 1;      // render fan holes
//fan_holes = 0;    // skip fan holes
holedia_top = 10;   // larger (faster rendering) fan holes on top
//holedia_top = 5;    // nicer smaller fan holes on top

// facets
$fn = 50;

// board params
wall_thickness = 2.5;
jack_buffer = 2; 			// amount of extra cutout space around each side of a jack
tooth_width = wall_thickness;
tooth_depth = 20;
tooth_height = 20;

pcb_width = 150;
pcb_depth = 100;
pcb_height = 40 + (1*wall_thickness);

module outline_frame ( offset = 10 ) {
    // a thick edged rect frame
    // full surface - smaller surface
    // frame thickness is 'offset'

    difference() {
        
        // top or bottom full size surface
        cube ( [ pcb_width + (wall_thickness*4), pcb_depth + (2*wall_thickness), wall_thickness] );
        // top or bottom smaller surface
        translate ( [ offset, offset, 0 ] ) {
            cube ( [ (pcb_width + (wall_thickness*4)) - (2*offset), (pcb_depth + (2*wall_thickness)) - (2*offset), wall_thickness] );
        }

    } // diff

} // outline frame

module frontbacktop () {

    module top() {
        union() {
			// top
			color ( "MediumSeaGreen" ) {
				translate ( [ 0, 0, pcb_height ] ) {
					cube ( [ pcb_width + (wall_thickness*4), pcb_depth + (2*wall_thickness), wall_thickness] );
				}
			}
        }
    }

	module walls() {
		union() {
			// front wall
			color ( "thistle" ) {
				cube ( [ pcb_width + (wall_thickness*4), wall_thickness, pcb_height ] );
			}
			// back wall
			color ( "green" ) {
				translate ( [ 0, pcb_depth + (1*wall_thickness), 0 ] ) {
					cube ( [ pcb_width + (wall_thickness*4), wall_thickness, pcb_height ] );
				}
			}
            // left fastener tooth
			color ( "MediumSeaGreen" ) {
				translate ( [ wall_thickness, pcb_depth / 3 * 2, pcb_height - tooth_height ] ) {
					cube ( [ tooth_width, tooth_depth, tooth_height ] );
				}
			}
            // right fastener tooth
			color ( "MediumSeaGreen" ) {
				translate ( [ pcb_width + (2*wall_thickness), pcb_depth / 3 * 2, pcb_height - tooth_height ] ) {
					cube ( [ tooth_width, tooth_depth, tooth_height ] );
				}
			}

        } // union
	} // walls

	module cutouts() {
		// LEFT SIDE
		//

        // left fastener small hole
        translate ( [ 0, ( pcb_depth / 3 * 2 ) + ( tooth_depth /2 ), pcb_height - ( tooth_height / 2 ) ] ) {
            rotate ( [ 0, 90, 0 ] ) {
                cylinder ( h=wall_thickness*2,r1=1,r2=1 );
            }
        }

		// BACK
		//

		// USB power/serial
		translate ( [ 3 - jack_buffer, pcb_depth + (1*wall_thickness), 25 - jack_buffer ] ) {
			cube( [ 7 + (2*jack_buffer), 20, 4 + (2*jack_buffer) ] );
		}
		// volume potwheel
		translate ( [ 16, pcb_depth + (1*wall_thickness), 25 - jack_buffer ] ) {
			cube( [ 15 + (2*jack_buffer), 20, 2 + (2*jack_buffer) ] );
		}
		// PS/2 keyb
		translate ( [ 36, pcb_depth + (1*wall_thickness), 25 - jack_buffer ] ) {
			cube( [ 15, 20, 15 + (2*jack_buffer) ] );
		}
		// VGA
		translate ( [ 55, pcb_depth + (1*wall_thickness), 25 - jack_buffer ] ) {
			cube( [ 31, 20, 14 + (2*jack_buffer) ] );
		}
        
        // TOP
        //

   		// cart
		translate ( [ 30 + (wall_thickness*2), 2, 35 - jack_buffer ] ) {
			cube( [ 82 + (2*jack_buffer), 25+(2*jack_buffer), 20 ] );
		}
        
        // RIGHT
        //
        
        // right fastener small hole
        translate ( [ pcb_width + (wall_thickness*2), ( pcb_depth / 3 * 2 ) + ( tooth_depth /2 ), pcb_height - ( tooth_height / 2 ) ] ) {
            rotate ( [ 0, 90, 0 ] ) {
                cylinder ( h=wall_thickness*2,r1=1,r2=1 );
            }
        }

	} // cutouts

    module cart_frame() {
		translate ( [ 25 + (wall_thickness*2), 2+(1*wall_thickness), pcb_height ] ) {
			cube( [ 95, 35, wall_thickness ] );
		}
    }

    module toptrim_frame() {
        
        // frame
        translate ( [ 80, 60, pcb_height ] )  {
            //cube ( [ 60, 40, wall_thickness ] );
            cylinder ( h=wall_thickness, r1=25, r2=25 );
        } // tr
        
     } // top trim frame

    module toptrim_text() {
        
        // ZZ
        translate ( [ 63, 52 + (1*wall_thickness), 39 ] )  {
             linear_extrude(slices=1,height=15) {
                text ( text="ZZ", size=20, font="DejaVu Sans Mono:style=Bold" );
             } // ex
         } // tr
             
     } // top trim text
  
    difference() {
        union() {

            // top - fanholes
            if ( fan_holes == 1 ) {
                difference() {
                    top();
                    translate( [ 75, 55, 38 ] ) fanholes(dia=140,holedia=holedia_top,height=10);
                } // diff
            } else {
                difference() {
                    top();
                    // approximate holes with a single big cutout
                    translate ( [ 20, 15, pcb_height ] ) {
                        cube ( [ pcb_width - 40, pcb_depth - 30, wall_thickness ] );
                    } // tr
                }
            }
            // + walls
            // + outline frame
            // + cart frame
            // + top trim frame
            walls();
            translate ( [ 0, 0, pcb_height ] ) {
                outline_frame ( 12 );
            }
            cart_frame();
            toptrim_frame();
        }
        // - cutouts
        // - top trim text
        cutouts();
        toptrim_text();
    } // diff

} // front back top

module leftrightbottom () {

	module walls() {
		union() {
			// left wall
			color ( "cadetblue" ) {
                translate ( [ 0, wall_thickness, 0 ] ) {
                    cube ( [ wall_thickness, pcb_depth, pcb_height ] );
                }
			}
			// right wall
			color ( "thistle" ) {
                translate ( [ pcb_width + (wall_thickness*3), wall_thickness, 0 ] ) {
                    cube ( [ wall_thickness, pcb_depth, pcb_height ] );
                }
			}

        } // union
	} // walls
    
    module bottom() {
        union() {
			// bottom
			color ( "MediumSeaGreen" ) {
                cube ( [ pcb_width + (wall_thickness*4), pcb_depth, wall_thickness] );
			}
        }
    }
    
	module cutouts() {
		// LEFT SIDE
		//

		// joy0 + headphone
		translate ( [ -10, 9 - jack_buffer, 25 - jack_buffer ] ) {
			cube( [ 20, 30 + 15 + (2*jack_buffer), 13 + (2*jack_buffer) ] );
		}

        // left fastener hole
        translate ( [ 0, ( pcb_depth / 3 * 2 ) + ( tooth_depth /2 ), pcb_height - ( tooth_height / 2 ) ] ) {
            rotate ( [ 0, 90, 0 ] ) {
                cylinder ( h=wall_thickness*2,r1=2,r2=2 );
            }
        }

        // RIGHT SIDE
        //
 
        // right fastener hole
        translate ( [ pcb_width + (3*wall_thickness), ( pcb_depth / 3 * 2 ) + ( tooth_depth /2 ), pcb_height - ( tooth_height / 2 ) ] ) {
            rotate ( [ 0, 90, 0 ] ) {
                cylinder ( h=wall_thickness*2,r1=2,r2=2 );
            }
        }

    } // cutouts

    difference() {
        union() {
            difference() {
                bottom();
                if ( fan_holes == 1 ) {
                    translate( [ 75, 55, 0 ] ) fanholes(dia=140,holedia=10,height=20);
                } // if
            }
            walls();
            outline_frame ( 12 );
        } // union
        cutouts();
    }

} // left right bottom

// DO IT
//

rotate_y = 0;

if ( invert_y == 1 ) {
    //assert ( 1==2, "Invert not coded yet" );
    rotate_y = 180;
}
echo ("Rotate Y ", rotate_y );

rotate ( [ 0, rotate_y, 0 ] ) {
    scale ( [ scale_factor_x, scale_factor_y, scale_factor_z ] ) {
        if ( target == "frontbacktop" ) {
            frontbacktop();
        } else if ( target == "leftrightbottom" ) {
            leftrightbottom();
        } else if ( target == "all" ) {
            frontbacktop();
            leftrightbottom();
        }
    }
}
