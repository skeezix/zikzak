
#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include "lib_i2c.h"

#define OK 0
#define printf(...)

//-------------------------------------------
// Definition of local constants
//-------------------------------------------
#define I2C_TIMEOUT  (0x1000)

//-------------------------------------------
// Declaration of local functions
//-------------------------------------------
static int I2C_check_dev(uint8_t addr);
static int I2C_start(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t rdwr);
static int I2C_restart(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t rdwr);
static int I2C_stop(I2C_TypeDef* I2Cx);
static int I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
static int I2C_read_ack(I2C_TypeDef* I2Cx);
static int I2C_read_nack(I2C_TypeDef* I2Cx);
static int I2C_timeout(char *msg);

/*******************************************************************************
* Function Name  : I2C_BusInit
* Description    : Configure I2C-Bus
* Input          : None
* Output         : None
* Return         : None
* Attention	     : None
*******************************************************************************/
void I2C_BusInit(void)
{ 												
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
  //NVIC_InitTypeDef NVIC_InitStructure;
  //=========================================================================================
  // Enable GPIO clock
  //-----------------------------------------------------------------------------------------
  RCC_AHB1PeriphClockCmd(Open_I2C_SCL_GPIO_CLK, ENABLE);

  //=========================================================================================
  // Enable I2C clock
  //-----------------------------------------------------------------------------------------
  RCC_APB1PeriphClockCmd(Open_I2C_CLK, ENABLE);

  //=========================================================================================
  //GPIO Alternate functions configuration
  //-----------------------------------------------------------------------------------------
  GPIO_PinAFConfig(Open_I2C_SCL_GPIO_PORT, Open_I2C_SCL_SOURCE, Open_I2C_SCL_AF);
  GPIO_PinAFConfig(Open_I2C_SDA_GPIO_PORT, Open_I2C_SDA_SOURCE, Open_I2C_SDA_AF);

  //=========================================================================================
  // Configure I2C GPIO
  //-----------------------------------------------------------------------------------------
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = Open_I2C_SCL_PIN | Open_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //=========================================================================================
  // Configure I2C Controller
  //-----------------------------------------------------------------------------------------
  I2C_InitStructure.I2C_ClockSpeed = I2C_SCL_SPEED;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDR;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  //=========================================================================================
  // Enable I2C
  //-----------------------------------------------------------------------------------------
  I2C_Cmd(Open_I2C, ENABLE);
  I2C_Init(Open_I2C, &I2C_InitStructure);

  //=========================================================================================
  // Configure the SPI interrupt priority
  //-----------------------------------------------------------------------------------------
  /*
  NVIC_InitStructure.NVIC_IRQChannel = Open_I2C_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  */
}

//===============================================================================
// I2C_ReadTransfer - initiate a read transfer on the I2C bus
//
// Initiates a read transfer on the I2C bus.
// If a register address is specified (<addressLength> != 0) a write without a stop
// condition will be initiated to the device to write the address, before the
// read is initiated.
// If no address if required (sequential read) <addressLength> is set to 0.
//-------------------------------------------------------------------------------
// u08 dev_addr  I2C device address
// u08 *buffer   pointer to the buffer to store the read data.
//               The buffer must be at least 'cnt' bytes long
// int cnt       number of bytes to read
// u32 ptr       register address, if required by the I2C-device
// u08 ptrlen    length of the register address to be written.
//               Valid values are 0..4
//-------------------------------------------------------------------------------
int I2C_ReadTransfer(uint8_t dev_addr, uint8_t *buffer, int cnt, uint32_t ptr, uint8_t ptrlen)
{
  int i, rc = OK;
  //-----------------------------------------------------------------------------
  // parameter check
  //-----------------------------------------------------------------------------
  if ((buffer == 0) || (ptrlen > 4) || ((cnt | ptrlen) == 0))
  {
    return I2C_check_dev(dev_addr); // may be used to check if device is responding
  }
  //-----------------------------------------------------------------------------
  // write the register address pointer to the device
  //-----------------------------------------------------------------------------
  if (ptrlen > 0)
  {
    rc = I2C_start(Open_I2C, dev_addr, I2C_Direction_Transmitter);
    if (rc == OK)
    {
      for (i=1; i<=ptrlen; i++)
      {
        rc |= I2C_write(Open_I2C,((ptr >>(8*(ptrlen-i))) & 0xff));
      }
    }
  }
  //-----------------------------------------------------------------------------
  // read data from device
  //-----------------------------------------------------------------------------
  if ((cnt > 0) && (rc == OK))
  {
    if (ptrlen > 0)
    {
      rc |= I2C_restart(Open_I2C, dev_addr, I2C_Direction_Receiver);
    }
    else
    {
      rc |= I2C_start(Open_I2C, dev_addr, I2C_Direction_Receiver);
    }
    if (rc == OK)
    {
      while (--cnt>0)                         // while more than one byte to read
      {
        *(buffer++) = I2C_read_ack(Open_I2C); // read next databyte from I2C device
      }
      *(buffer) = I2C_read_nack(Open_I2C);    // read last databyte from I2C device
    }
  }
  //-----------------------------------------------------------------------------
  I2C_stop(Open_I2C);                         // stop the transmission
  return rc;
}


//===============================================================================
// I2C_WriteTransfer r- initiate a write transfer on the I2C bus
//
// Initiates a write transfer on the I2C bus.
// If a register address is supplied it is inserted between the I2C device address
// and the data.
//-------------------------------------------------------------------------------
// u08 dev_addr  I2C device address
// u08 *buffer   pointer to the buffer to store the read data.
//               The buffer must be at least 'cnt' bytes long
// int cnt       number of bytes to read
// u32 ptr       register address, if required by the I2C-device
// u08 ptrlen    length of the register address to be written.
//               Valid values are 0..4
//-------------------------------------------------------------------------------
int  I2C_WriteTransfer ( uint8_t dev_addr, uint8_t *buffer, int cnt, uint32_t ptr, uint8_t ptrlen) {
  int i, rc=OK;
  //-----------------------------------------------------------------------------
  // parameter check
  //-----------------------------------------------------------------------------
  if ((buffer == 0) || (ptrlen > 4) || ((cnt | ptrlen) == 0))
  {
    return I2C_check_dev(dev_addr); // may be used to check if device is responding
  }
  //-----------------------------------------------------------------------------
  rc = I2C_start(Open_I2C, dev_addr, I2C_Direction_Transmitter);
  if (rc == OK)
  {
    //---------------------------------------------------------------------------
    // write the register address pointer to the device
    //---------------------------------------------------------------------------
    if (ptrlen > 0)
    {
      for (i=1; i<=ptrlen; i++)
      {
        rc |= I2C_write(Open_I2C,((ptr >>(8*(ptrlen-i))) & 0xff));
      }
    }
    //---------------------------------------------------------------------------
    // write data to the device
    //---------------------------------------------------------------------------
    for (i=0; i<cnt; i++)
    {
      rc |= I2C_write(Open_I2C,*(buffer++));
    }
  }
  //-----------------------------------------------------------------------------
  I2C_stop(Open_I2C);                // stop the transmission
  return rc;
}


//===============================================================================
// Check if a device is responding with acknowledge on the given I2C-Bus address
//-------------------------------------------------------------------------------
static int I2C_check_dev(uint8_t addr)
{
  int timeout;
  //-----------------------------------------------------------------------------
  timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(Open_I2C, I2C_FLAG_BUSY))
  {
    if((timeout--)==0)   // wait until I2C-Bus is not busy anymore
    {
      return ERROR;
    }
  }
  //-----------------------------------------------------------------------------
  I2C_GenerateSTART(Open_I2C, ENABLE);
  timeout = I2C_TIMEOUT;
  while(!I2C_CheckEvent(Open_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((timeout--)==0)  // wait while sending I2C-Bus START condition
    {
      I2C_GenerateSTOP(Open_I2C, ENABLE);
      return ERROR;
    }
  }
  //-----------------------------------------------------------------------------
  I2C_Send7bitAddress(Open_I2C, addr, I2C_Direction_Transmitter);
  timeout = I2C_TIMEOUT;
  while(!I2C_CheckEvent(Open_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((timeout--)==0)   // wait while sending slave address for write
    {
      I2C_GenerateSTOP(Open_I2C, ENABLE);
      return ERROR;
    }
  }
  //-----------------------------------------------------------------------------
  I2C_GenerateSTOP(Open_I2C, ENABLE);
  return OK;
}

//------------------------------------------------------------------
// This function issues a start condition and
// transmits the slave address + R/W bit
//
// Parameters:
//  I2Cx  --> the I2C peripheral e.g. I2C1
//  addr  --> the 7 bit slave address
//  rdwr  --> the tranmission direction can be:
//              I2C_Direction_Tranmitter for Master transmitter mode
//              I2C_Direction_Receiver for Master receiver
//------------------------------------------------------------------
static int I2C_start(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t rdwr)
{
  uint32_t timeout = (100 * I2C_TIMEOUT);
  // wait until I2C1 is not busy anymore
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
  {
    if((timeout--)==0) return I2C_timeout("I2C_start(): bus busy");
  }
  // Send I2C1 RESTART condition
  return I2C_restart(I2Cx, addr, rdwr);
}

//------------------------------------------------------------------
// This function issues a restart condition and
// transmits the slave address + R/W bit
//
// Parameters:
//  I2Cx  --> the I2C peripheral e.g. I2C1
//  addr  --> the 7 bit slave address
//  rdwr  --> the tranmission direction can be:
//              I2C_Direction_Tranmitter for Master transmitter mode
//              I2C_Direction_Receiver for Master receiver
//------------------------------------------------------------------
static int I2C_restart(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t rdwr)
{
  uint32_t timeout = I2C_TIMEOUT;
  // Send I2C1 START condition
  I2C_GenerateSTART(I2Cx, ENABLE);
  // wait for I2C1 EV5 --> Slave has acknowledged start condition
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((timeout--)==0) return I2C_timeout("I2C_start(): start failed");
  }
  // Send slave Address for read or write
  I2C_Send7bitAddress(I2Cx, addr, rdwr);
  //------------------------------------------------------------------------
  // wait for I2C1 EV6, check if Slave has acknowledged Master transmitter
  // or Master receiver mode, depending on the transmission direction
  //------------------------------------------------------------------------
  timeout = I2C_TIMEOUT;
  if (rdwr==I2C_Direction_Transmitter)
  {
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
      if((timeout--)==0) return I2C_timeout("I2C_start(): no acknowledge");
    }
  }
  else if(rdwr==I2C_Direction_Receiver)
  {
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
      if((timeout--)==0) return I2C_timeout("I2C_start(): no acknowledge");
    }
  }
  return 0;
}

//------------------------------------------------------------------
// This function transmits one byte to the slave device
// Parameters:
//    I2Cx --> the I2C peripheral e.g. I2C1
//    data --> the data byte to be transmitted
//------------------------------------------------------------------
static int I2C_write(I2C_TypeDef* I2Cx, uint8_t data)
{
  uint32_t timeout = I2C_TIMEOUT;
  I2C_SendData(I2Cx, data);
  // wait for I2C1 EV8_2 --> byte has been transmitted
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((timeout--)==0) return I2C_timeout("I2C_write(): write byte failed");
  }
  return 0;
}

//------------------------------------------------------------------
// This function reads one byte from the slave device
// and acknowledges the byte (requests another byte)
//------------------------------------------------------------------
static int I2C_read_ack(I2C_TypeDef* I2Cx)
{
  uint32_t timeout = I2C_TIMEOUT;
  // enable acknowledge of recieved data
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  // wait until one byte has been received
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
    if((timeout--)==0) return I2C_timeout("I2C_read_ack(): read byte failed");
  }
  // read data from I2C data register and return data byte
  return I2C_ReceiveData(I2Cx);
}

//------------------------------------------------------------------
// This function reads one byte from the slave device
// and doesn't acknowledge the recieved data
//------------------------------------------------------------------
static int I2C_read_nack(I2C_TypeDef* I2Cx)
{
  uint32_t timeout = I2C_TIMEOUT;
  // disabe acknowledge of received data
  I2C_AcknowledgeConfig(I2Cx, DISABLE);
  // wait until one byte has been received
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
    if((timeout--)==0) return I2C_timeout("I2C_read_nack(): read byte failed");
  }
  // read data from I2C data register and return data byte
  return I2C_ReceiveData(I2Cx);
}

//------------------------------------------------------------------
// This funtion issues a stop condition and therefore
// releases the bus
//------------------------------------------------------------------
static int I2C_stop(I2C_TypeDef* I2Cx)
{
  // Send I2C1 STOP Condition
  I2C_GenerateSTOP(I2Cx, ENABLE);
  return 0;
}

static int I2C_timeout(char *msg)
{
  if ( msg ) {
    printf("TIMEOUT: %s\n",msg);
  }
  return -1;
}

void I2C1_EV_IRQHandler(void)
{
  u32 event;
  event = I2C_GetLastEvent(I2C2);
  if ( event ) {
    printf("I2C1_EV_IRQHandler(): Event=0x%08X\n",event);
  }
  //---------------------------------------------------
  // todo, if I2C1 ISR mode shall be used
  //---------------------------------------------------
}

void I2C2_EV_IRQHandler(void)
{
  u32 event;
  event = I2C_GetLastEvent(I2C2);
  if ( event ) {
    printf("I2C2_EV_IRQHandler(): Event=0x%08X\n",event);
  }
  //---------------------------------------------------
  // todo, if I2C2 ISR mode shall be used
  //---------------------------------------------------
}

