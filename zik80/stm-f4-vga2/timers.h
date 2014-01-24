
#ifndef h_timers_h
#define h_timers_h

static const uint8_t APBAHBPrescalerTable[16]={0,0,0,0,1,2,3,4,1,2,3,4,6,7,8,9};

static inline uint32_t SYSCLKFrequency()
{
	switch(RCC->CFGR&RCC_CFGR_SWS)
	{
		case 0x00:  // HSI used as system clock source
			return HSI_VALUE;

		default:
		case 0x04:  // HSE used as system clock source
			return HSE_VALUE;

		case 0x08: // PLL used as system clock source
		{
      		uint32_t srcclock;
			if(RCC->PLLCFGR&RCC_PLLCFGR_PLLSRC) srcclock=HSE_VALUE; // HSE used as PLL clock source
			else srcclock=HSI_VALUE; // HSI used as PLL clock source

			uint32_t pllm=RCC->PLLCFGR&RCC_PLLCFGR_PLLM;
			uint32_t pllvco=(srcclock/pllm)*((RCC->PLLCFGR&RCC_PLLCFGR_PLLN)>>6);
			uint32_t pllp=(((RCC->PLLCFGR&RCC_PLLCFGR_PLLP)>>16)+1)*2;
			return pllvco/pllp;
		}
	}
}

static inline uint32_t HCLKFrequency()
{
	uint32_t shift=APBAHBPrescalerTable[(RCC->CFGR&RCC_CFGR_HPRE)>>4];
	return SYSCLKFrequency()>>shift;
}

#endif
