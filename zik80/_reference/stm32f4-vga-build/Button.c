#include "Button.h"
#include "GPIO.h"
#include "RCC.h"

//#include <stm32f4xx_exti.h>
//#include <stm32f4xx_syscfg.h>
//#include <misc.h>

void InitializeUserButton()
{
	EnableAHB1PeripheralClock(RCC_AHB1ENR_GPIOAEN);

	SetGPIOInputMode(GPIOA,(1<<0));
	SetGPIONoPullResistor(GPIOA,(1<<0));
}

void EnableUserButtonInterrupt()
{
	EnableAPB2PeripheralClock(RCC_APB2ENR_SYSCFGEN);

/*	EXTI_InitTypeDef EXTI_InitStructure;
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

	EXTI_Init(&(EXTI_InitTypeDef){
		.EXTI_Line=EXTI_Line0,
		.EXTI_Mode=EXTI_Mode_Interrupt,
		.EXTI_Trigger=EXTI_Trigger_Rising,
		.EXTI_LineCmd=ENABLE,
	});

	NVIC_Init(&(NVIC_InitTypeDef){
		.NVIC_IRQChannel=EXTI0_IRQn,
		.NVIC_IRQChannelPreemptionPriority=0x0f, // Lowest priority
		.NVIC_IRQChannelSubPriority=0x0f,
		.NVIC_IRQChannelCmd=ENABLE,
	});*/
}

