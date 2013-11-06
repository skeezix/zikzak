#if 0 // colour grid


/*
  D3 : Horizontal Sync (68 ohms in series) --> Pin 13 on DB15 socket
  D4 : Red pixel output (470 ohms in series) --> Pin 1 on DB15 socket
  D5 : Green pixel output (470 ohms in series) --> Pin 2 on DB15 socket
  D6 : Blue pixel output (470 ohms in series) --> Pin 3 on DB15 socket
  D10 : Vertical Sync (68 ohms in series) --> Pin 14 on DB15 socket
  
  Gnd : --> Pins 5, 6, 7, 8, 10 on DB15 socket
*/



// Timer 1 - Vertical sync

// output    OC1B   pin 16  (D10) <------- VSYNC

//   Period: 16.64 mS (60 Hz)
//      1/60 * 1e6 = 16666.66 uS
//   Pulse for 64 uS  (2 x HSync width of 32 uS)
//    Sync pulse: 2 lines
//    Back porch: 33 lines
//    Active video: 480 lines
//    Front porch: 10 lines
//       Total: 525 lines

// Timer 2 - Horizontal sync

// output    OC2B   pin 5  (D3)   <------- HSYNC

//   Period: 32 uS (31.25 kHz)
//      (1/60) / 525 * 1e6 = 31.74 uS
//   Pulse for 4 uS (96 times 39.68 nS)
//    Sync pulse: 96 pixels
//    Back porch: 48 pixels
//    Active video: 640 pixels
//    Front porch: 16 pixels
//       Total: 800 pixels

// Pixel time =  ((1/60) / 525 * 1e9) / 800 = 39.68  nS
//  frequency =  1 / (((1/60) / 525 * 1e6) / 800) = 25.2 MHz

// However in practice, it we can only pump out pixels at 375 nS each because it
//  takes 6 clock cycles to read one in from RAM and send it out the port.

const int horizontalBytes = 60;  // 480 pixels wide
const int verticalPixels = 480;  // 480 pixels high


const int verticalLines = verticalPixels / 16;  
const int horizontalPixels = horizontalBytes * 8;

const unsigned char verticalBackPorchLines = 35;  // includes sync pulse?
//const unsigned char verticalFrontPorchLines = 525 - verticalBackPorchLines;

volatile int vLine;
volatile int messageLine;
volatile unsigned char backPorchLinesToGo;

// bitmap - gets sent to PORTD
// For D4/D5/D6 bits need to be shifted left 4 bits
//  ie. 00BGR0000

char message [verticalLines]  [horizontalBytes];

// ISR: Vsync pulse
ISR (TIMER1_OVF_vect)
{
  vLine = 0; 
  messageLine = 0;
  backPorchLinesToGo = verticalBackPorchLines;
} // end of TIMER1_OVF_vect
  
// ISR: Hsync pulse ... this interrupt merely wakes us up
ISR (TIMER2_OVF_vect)
{
} // end of TIMER2_OVF_vect


int main ( void ) {

  DDRB |= (1<<PB0);
  PORTB |= (1<<PB0);
  //PORTB &= ~(1<<PB0);

  // setup
  //
  // initial bitmap ... change to suit
  for (int y = 0; y < verticalLines; y++) {
    for (int x = 0; x < horizontalBytes; x++) {
      //message [y] [x] = (x + y) << 4;
      //message [y] [x] = (x + y) << 0;
      //message [y] [x] = 0;
      message [y] [x] = 1;
      //message [y] [x] = (x + y) & 0x07;
    }
  }
   
  // disable Timer 0
  TIMSK0 = 0;  // no interrupts on Timer 0
  OCR0A = 0;   // and turn it off
  OCR0B = 0;
  
  // Timer 1 - vertical sync pulses
  DDRD |= (1<<PD4); // vsync
  Timer1::setMode (15, Timer1::PRESCALE_1024, Timer1::CLEAR_B_ON_COMPARE);
  OCR1A = 259;  // 16666 / 64 uS = 260 (less one)
  OCR1B = 0;    // 64 / 64 uS = 1 (less one)
  TIFR1 = _BV (TOV1);   // clear overflow flag
  TIMSK1 = _BV (TOIE1);  // interrupt on overflow on timer 1

  // Timer 2 - horizontal sync pulses
  DDRD |= (1<<PD6); // hsync
  Timer2::setMode (7, Timer2::PRESCALE_8, Timer2::CLEAR_B_ON_COMPARE);
  OCR2A = 63;   // 32 / 0.5 uS = 64 (less one)
  OCR2B = 7;    // 4 / 0.5 uS = 8 (less one)
  TIFR2 = _BV (TOV2);   // clear overflow flag
  TIMSK2 = _BV (TOIE2);  // interrupt on overflow on timer 2
 
  // prepare to sleep between horizontal sync pulses  
  set_sleep_mode ( SLEEP_MODE_IDLE );  
  
  // pins for outputting the colour information
  DDRD |= (1<<PD0); // red
  DDRD |= (1<<PD1); // green
  DDRD |= (1<<PD2); // blue

  sei();

  // main loop
  //

  while ( 1 ) {
    // sleep to ensure we start up in a predictable way
    sleep_mode ();
    doOneScanLine ();
  }  // end of loop

  return ( 0 );
}


// draw a single scan line
void doOneScanLine () {

  //PORTB ^= (1<<PB0);
    
  // after vsync we do the back porch
  if (backPorchLinesToGo) {
    backPorchLinesToGo--;
    return;   
  }  // end still doing back porch
    
  // if all lines done, do the front porch
  if (vLine >= verticalPixels) {
    return;
  }
    
  // pre-load pointer for speed
  register char * messagePtr =  & (message [messageLine] [0] );

  _delay_us (1);
  
  // how many pixels to send
  register unsigned char i = horizontalBytes;

  // blit pixel data to screen    
  while (i--) {
    PORTD = * messagePtr++;
  }

  // stretch final pixel
  nop; nop; nop;
  
  PORTD = 0;  // back to black
  // finished this line 
  vLine++;

  // every 16 pixels it is time to move to a new line in our text
  if ((vLine & 0xF) == 0) {
    messageLine++;
  }
    
}  // end of doOneScanLine

#endif
