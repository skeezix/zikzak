
// as if from SD
// clive1

#include <stdio.h>
#include <string.h>
 
typedef int (*pFunction)(void);
 
void RamCode(void)
{
  static const uint16_t Code[] = { 0xF04F, 0x007B, 0x4770 }; // payload
  uint8_t Buffer[16]; // as big as required
  pFunction RunCode;
 
  memcpy(Buffer, Code, sizeof(Code)); // Copy code
 
  RunCode = (pFunction)&Buffer[1]; // +1 for Thumb code
 
  printf("%d\n",RunCode());
}
