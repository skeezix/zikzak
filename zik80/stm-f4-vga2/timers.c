
#include "stm32f4xx.h"
#include "core_cm4.h"

#include "timers.h"

void timers_setup ( void ) {

  SysTick_Config ( HCLKFrequency()/100 );

}
