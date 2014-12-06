
#ifndef _I2C_H
#define _I2C_H

#define I2C1_OPEN
//#define I2C2_OPEN

#define I2C_SCL_SPEED       (200000)    // SCL in Hz
#define I2C_OWN_ADDR        (0x00)      // use 0x00 for master

/**
 * @brief Definition for I2C1
 */  
#if defined	I2C1_OPEN
	#define Open_I2C                        I2C1
	#define Open_I2C_CLK                    RCC_APB1Periph_I2C1
	
	#define Open_I2C_SCL_PIN                GPIO_Pin_6
	#define Open_I2C_SCL_GPIO_PORT          GPIOB
	#define Open_I2C_SCL_GPIO_CLK           RCC_AHB1Periph_GPIOB
	#define Open_I2C_SCL_SOURCE             GPIO_PinSource6
	#define Open_I2C_SCL_AF                 GPIO_AF_I2C1
	
	#define Open_I2C_SDA_PIN                GPIO_Pin_9
	#define Open_I2C_SDA_GPIO_PORT          GPIOB
	#define Open_I2C_SDA_GPIO_CLK           RCC_AHB1Periph_GPIOB
	#define Open_I2C_SDA_SOURCE             GPIO_PinSource9
	#define Open_I2C_SDA_AF                 GPIO_AF_I2C1
	
	#define Open_I2C_IRQn                   I2C1_EV_IRQn
	#define I2Cx_IRQHANDLER                 I2C1_EV_IRQHandler

#elif defined I2C2_OPEN
  #define Open_I2C                        I2C2
  #define Open_I2C_CLK                    RCC_APB1Periph_I2C2

  #define Open_I2C_SCL_PIN                GPIO_Pin_10
  #define Open_I2C_SCL_GPIO_PORT          GPIOB
  #define Open_I2C_SCL_GPIO_CLK           RCC_AHB1Periph_GPIOB
  #define Open_I2C_SCL_SOURCE             GPIO_PinSource10
  #define Open_I2C_SCL_AF                 GPIO_AF_I2C2

  #define Open_I2C_SDA_PIN                GPIO_Pin_11
  #define Open_I2C_SDA_GPIO_PORT          GPIOB
  #define Open_I2C_SDA_GPIO_CLK           RCC_AHB1Periph_GPIOB
  #define Open_I2C_SDA_SOURCE             GPIO_PinSource11
  #define Open_I2C_SDA_AF                 GPIO_AF_I2C2

  #define Open_I2C_IRQn                   I2C2_EV_IRQn
  #define I2Cx_IRQHANDLER                 I2C2_EV_IRQHandler

#else
	#error "Please select the I2C-Device to be used (in i2c.h)"
#endif 		 

void I2C_BusInit(void);
int  I2C_ReadTransfer(uint8_t dev_addr, uint8_t *buffer, int cnt, uint32_t ptr, uint8_t ptrlen);
int  I2C_WriteTransfer(uint8_t dev_addr, uint8_t *buffer, int cnt, uint32_t ptr, uint8_t ptrlen);

void I2C1_EV_IRQHandler(void);
void I2C2_EV_IRQHandler(void);

#endif /*_I2C_H*/
