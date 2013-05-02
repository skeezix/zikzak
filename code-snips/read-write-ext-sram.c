#include "ReadWrite.h"
#include "io_macros.h"

//For general use to help people.  Did you hear that Kartman?
//These functions tested and working. 

/***************************************************************************
   Function Name           :  Write
   Function Description    : 
       Input               :  Data And Address
       Output              :  none
       Comments            :  Writes to an external device
***************************************************************************/
void Write(unsigned char data, unsigned char address)
{
  DDRC = 0xff;            //Set Data bus to output
  PORTA = address;         //Write address out to PORTA
  ClearBit(PORTB, BIT(1));   //Set RW Low
  PORTC = data;            //Write data out to PORTC
  ClearBit(PORTB, BIT(0));   //Set CS Low
  ClearBit(PORTB, BIT(0));   //I added this line to add 1 cycle of time
  SetBit(PORTB, BIT(0));      //Set CS High
  SetBit(PORTB, BIT(1));      //Set RW back High to create edge. 
}


/***************************************************************************
   Function Name           :  Read
   Function Description    : 
       Input               :  address
       Output              :  The data returned from the external device
       Comments            :  Reads the data from an external device
***************************************************************************/
unsigned char Read(unsigned char address)
{
  unsigned char data;
   
  DDRC = 0x00;            //Set Data bus to input
  PORTA = address;         //Set the address
  SetBit(PORTB, BIT(1));       //Set RW High
  ClearBit(PORTB, BIT(0));   //Set CS Low 
  ClearBit(PORTB, BIT(0));   //Set CS Low
  data = PINC;                //Read data bus
  SetBit(PORTB, BIT(0));     //return CS High
   
  return data;
} 
