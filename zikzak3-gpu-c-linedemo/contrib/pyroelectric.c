/*
  VGA
  
  Author: Chris @ PyroElectro.com
  Description:
  This project is meant to be an example source for anyone
  interested in outputting VGA signal's from a standard
  Arduino UNO development board. The goal is to output VGA
  800x600 @ 60 Hz.
  
  Full project details:
  http://www.pyroelectro.com/tutorials/arduino_basic_vga/
*/

#include <avr/io.h>

//Quickly Turn VSYNC On/Off
//Or You Can Change The VSYNC Pin..
#define VSYNC_LOW    PORTD &= ~_BV(6)
#define VSYNC_HIGH   PORTD |= _BV(6)

//Quickly Turn HSYNC On/Off
//Or You Can Change The HSYNC Pin..
#define HSYNC_LOW    PORTD &= ~_BV(7)
#define HSYNC_HIGH   PORTD |= _BV(7)

//RED
#define RED_ON       PORTB |= _BV(4)
#define RED_OFF      PORTB &= ~_BV(4)

//Green
#define GREEN_ON     PORTB |= _BV(3)
#define GREEN_OFF    PORTB &= ~_BV(3)

//BLUE
#define BLUE_ON      PORTB |= _BV(2)
#define BLUE_OFF     PORTB &= ~_BV(2)

void setup() {
}

void loop() {
  //Initializations
  int i = 0;
  cli();
  DDRB = 0xFF;
  DDRD = 0xFF;
  
  //Loop Over-And-Over Again
  while(1){
    //Clear The i counter
    i=0;
    //VSYNC Low
    VSYNC_LOW;
    //200 Lines Of Red
    while(i < 200){
      //Red Color High
      RED_ON;
          
      //2.2uS Back Porch
      delayMicroseconds(2);  
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
          
      //20uS Color Data
      delayMicroseconds(20); // 1uS        
          
      //Red Color Low
      RED_OFF;  //Low
          
      //1uS Front Porch
      delayMicroseconds(1); // 1uS 
      i++;
          
      //3.2uS Horizontal Sync
      HSYNC_HIGH;  //HSYNC High
      delayMicroseconds(3);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      HSYNC_LOW;  //HSYNC Low
          
      //26.4uS Total
    }
    //Clear The i counter
    i=0;
    //VSYNC Low 
    VSYNC_LOW;
    //200 Lines Of Green
    while(i < 200){
      //Green Color High
      GREEN_ON;      
            
      //2.2uS Back Porch
      delayMicroseconds(2);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
          
      //20uS Color Data
      delayMicroseconds(20); // 1uS        
          
      //Green Color Low
      GREEN_OFF;  //Low
          
      //1uS Front Porch
      delayMicroseconds(1); // 1uS 
      i++;
          
      //3.2uS Horizontal Sync
      HSYNC_HIGH;  //HSYNC High
      delayMicroseconds(3);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      HSYNC_LOW;  //HSYNC Low
          
      //26.4uS Total
    }    
  
    //Clear The i counter
    i=0;
    //VSYNC Low 
    VSYNC_LOW;  //Low
    //200 Lines Of Blue
    while(i<200){
      //Blue Color High
      BLUE_ON;  //Low      
      //2.2uS Back Porch
      delayMicroseconds(2);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
          
      //20uS Color Data
      delayMicroseconds(20); // 1uS        
          
      //Blue Color Low
      BLUE_OFF;  //Low
          
      //1uS Front Porch
      delayMicroseconds(1); // 1uS 
      i++;
          
      //3.2uS Horizontal Sync
      HSYNC_HIGH;  //HSYNC High
      delayMicroseconds(3);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      HSYNC_LOW;  //HSYNC Low
          
      //26.4uS Total
    }    
    //Clear The i counter
    i=0;
    //VSYNC High
    VSYNC_HIGH;
    //4 Lines Of VSYNC   
    while(i<4){         
      //2.2uS Back Porch    
      delayMicroseconds(2);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
          
      //20 uS Of Color Data
      delayMicroseconds(20);// 20uS
          
      //1uS Front Porch
      delayMicroseconds(1); // 1uS
      i++;
          
      //HSYNC for 3.2uS
      HSYNC_HIGH;  //High
      delayMicroseconds(3);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      HSYNC_LOW;  //Low  
          
      //26.4uS Total
    }
      
    //Clear The i counter
    i=0;
    //VSYNC Low
    VSYNC_LOW;
    //23 Lines Of Vertical Back Porch + 1 Line Of Front Porch
    //This is very, very flexible...I'm setting it to 22 Lines!
    while(i < 22){
      //2.2uS Back Porch
      delayMicroseconds(2);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    
      //20uS Color Data
      delayMicroseconds(20);// 20uS
            
      //1uS Front Porch
      delayMicroseconds(1); // 1uS
      i++;
          
      //HSYNC for 3.2uS
      HSYNC_HIGH;  //High
      delayMicroseconds(3);
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      HSYNC_LOW;  //Low  
          
      //26.4uS Total
    }    
  }
}
