/* Case parts for Zikzak rev3
 * www.zikzak.ca
 */

// ref
// colours: https://en.wikibooks.org/wiki/OpenSCAD_User_Manual/Transformations#color

// TODO:
// - add cart hole 'sides' as guides for the cart to go down nicely into edge connector
//

// render target
// - leftbacktop
// - rightfrontbottom
// - all
target = "leftbacktop";
//target = "rightfrontbottom";
//target = "all";

// scale? Use this to print out tiny versions for inspection prior to printing real ones
//scale_factor = 1;
scale_factor = 0.4;

// invert for print?
//rotate_y = 180;   // invert
rotate_y = 0;   // no invert

// fan holes?
fan_holes = 1;
//fan_holes = 0;

// DO IT
//

// facets
$fn = 50;

// board params
inner_width = 150;
inner_depth = 100;
inner_height = 40;

// calcs
sloppyness = 2;
wall_thickness = 2.5;
jack_buffer = 2; 			// amount of extra cutout space around each side of a jack

outer_width = inner_width + sloppyness + wall_thickness;
outer_depth = inner_depth + sloppyness + wall_thickness;
outer_height = inner_height + sloppyness + wall_thickness;

module leftbacktop () {
	function r_from_dia(d) = d / 2;

	module walls() {
		union() {
			// left wall
			color ( "thistle" ) {
				cube ( [ wall_thickness, outer_depth, outer_height ] );
			}
			// back wall
			color ( "cadetblue" ) {
				translate ( [ 0, inner_depth + sloppyness, 0 ] ) {
					cube ( [ outer_width, wall_thickness, outer_height ] );
				}
			}
			// top
			color ( "MediumSeaGreen" ) {
				translate ( [ 0, 0, inner_height + sloppyness ] ) {
					cube ( [ outer_width, outer_depth, wall_thickness] );
				}
			}

        } // union
	} // walls

	module cutouts() {
		// LEFT SIDE
		//

		// joy0 + headphone
		translate ( [ -10, 9 - jack_buffer, 25 - jack_buffer ] ) {
			cube( [ 20, 30 + 15 + (2*jack_buffer), 13 + (2*jack_buffer) ] );
		}

		// BACK
		//

		// USB power/serial
		translate ( [ 3 - jack_buffer, inner_depth, 25 - jack_buffer ] ) {
			cube( [ 7 + (2*jack_buffer), 20, 4 + (2*jack_buffer) ] );
		}
		// volume potwheel
		translate ( [ 16, inner_depth, 25 - jack_buffer ] ) {
			cube( [ 15 + (2*jack_buffer), 20, 2 + (2*jack_buffer) ] );
		}
		// PS/2 keyb
		translate ( [ 36, inner_depth, 25 - jack_buffer ] ) {
			cube( [ 15, 20, 15 + (2*jack_buffer) ] );
		}
		// VGA
		translate ( [ 55, inner_depth, 25 - jack_buffer ] ) {
			cube( [ 31, 20, 14 + (2*jack_buffer) ] );
		}
        
        // TOP
        //

   		// cart
		translate ( [ 30, 2, 35 - jack_buffer ] ) {
			cube( [ 82 + (2*jack_buffer), 25+(2*jack_buffer), 20 ] );
		}

	} // cutouts

    module reartrim() {
        translate ( [ 135, inner_depth + sloppyness + 2, 13 ] )  {
            rotate ( [ 90, 0, 180 ] ) {
                linear_extrude(slices=1,height=5) {
                    text ( text="ZZ", size=20, font="DejaVu Sans Mono:style=Bold" );
                } // extrude
            } // rotate
        } // translate
     } // rear trim
     
     module toptrim() {
         translate ( [ 10, 60, 39 + sloppyness  ] )  {
             linear_extrude(slices=1,height=5) {
                text ( text="/////////////////////////", size=20, font="DejaVu Sans Mono:style=Bold",spacing=.3 );
             } // ex
         } // tr
     } // top trim
  
    difference() {
		walls();
		cutouts();
        reartrim();
        toptrim();
	}

} // left back top

module rightfrontbottom () {

	module walls() {
		union() {
			// right wall
			color ( "thistle" ) {
                translate ( [ inner_width + sloppyness, 0, 0 ] ) {
                    cube ( [ wall_thickness, outer_depth, outer_height ] );
                }
			}
			// front wall
			color ( "cadetblue" ) {
                cube ( [ outer_width, wall_thickness, outer_height ] );
			}
			// bottom
			color ( "MediumSeaGreen" ) {
                cube ( [ outer_width, outer_depth, wall_thickness] );
			}

        } // union
	} // walls

    module trimtext() {
        translate ( [ 105, 0, 13 ] )  {
            rotate ( [ 90, 0, 0 ] ) {
                linear_extrude(slices=1,height=5) {
                    text ( text="ZZ", size=20, font="DejaVu Sans Mono:style=Bold" );
                } // extrude
            } // rotate
        } // translate
     } // trim text
    
    difference() {
		walls();
        trimtext();
        if ( fan_holes == 1 ) translate( [ 75, 55, 0 ] ) fanholes();
	}

} // right front bottom

rotate ( [ 0, rotate_y, 0 ] ) {
    scale ( [ scale_factor, scale_factor, scale_factor ] ) {
        if ( target == "leftbacktop" ) {
            leftbacktop();
        } else if ( target == "rightfrontbottom" ) {
            rightfrontbottom();
        } else if ( target == "all" ) {
            leftbacktop();
            rightfrontbottom();
        } // render the right piece
    } // scale
} // rotate

module fanholes() {
    // How large is the fan rotor ? (mm)
RotorDiameter=70;
// How large should each hole be ? (mm)
HolesDiameter=10;
// How much space should be left between each hole, proportionally to the hole size ?
HolesRingsSpacingFraction = 0.4; // [0.1:4.0]
// The height of the generated solids (mm)
Height = 10; 
// The number of faces of each hole outer perimeter
Resoultion=20;// [3:100]


// VARIABLES INITIALIZATION
echo("RotorDiameter=",RotorDiameter);
echo("HolesDiameter=",HolesDiameter);
echo("Height=",Height);
echo("Resoultion=",Resoultion);

HolesBoundingBox = HolesDiameter*(1+HolesRingsSpacingFraction);
echo("HolesBoundingBox=",HolesBoundingBox);
NumberOfRings = RotorDiameter / HolesBoundingBox;
echo("NumberOfRings=",NumberOfRings);

OuterRadius = RotorDiameter/2;
echo("OuterRadius=",OuterRadius);

RingRadiusStep = HolesBoundingBox;
echo("RingRadiusStep=",RingRadiusStep);


 
difference()
{
 for (i=[0:1:NumberOfRings/2])
 {
  difference()
  {  
   for(j=[0:360/((i*HolesBoundingBox*6/HolesBoundingBox)):360])
   {
    translate([sin(j)*(i*HolesBoundingBox),cos(j)*(i*HolesBoundingBox),0])cylinder(h=Height,d=HolesDiameter, center = false, $fn=Resoultion);
   }
  } 
 }
}

} // fanholes