#include "Accelerometer.h"
#include "GPIO.h"
#include "RCC.h"

#include "stm32f4xx.h"

static uint8_t ReadByte(uint8_t address);
static void ReadBytes(uint8_t *buffer,uint8_t address,int numbytes);
static void WriteByte(uint8_t byte,uint8_t address);
static void WriteBytes(uint8_t *bytes,uint8_t address,int numbytes);

static inline uint8_t LowerCS();
static inline uint8_t RaiseCS();
static uint8_t TransferByte(uint8_t byte);

void InitializeAccelerometer()
{
	// Enable peripherals.
	EnableAHB1PeripheralClock(RCC_AHB1ENR_GPIOAEN|RCC_AHB1ENR_GPIOEEN);
	EnableAPB2PeripheralClock(RCC_APB2ENR_SPI1EN);

	// Configure SPI pins as alternate function.
	SelectAlternateFunctionForGPIOPin(GPIOA,5,5);
	SelectAlternateFunctionForGPIOPin(GPIOA,6,5);
	SelectAlternateFunctionForGPIOPin(GPIOA,7,5);
	SetGPIOAlternateFunctionMode(GPIOA,(1<<5)|(1<<6)|(1<<7));
	SetGPIOPushPullOutput(GPIOA,(1<<5)|(1<<6)|(1<<7));
	SetGPIOSpeed50MHz(GPIOA,(1<<5)|(1<<6)|(1<<7));
	SetGPIOPullDownResistor(GPIOA,(1<<5)|(1<<6)|(1<<7));

	// Configure and enable SPI.
	SetAPB2PeripheralReset(RCC_APB2RSTR_SPI1RST);
	ClearAPB2PeripheralReset(RCC_APB2RSTR_SPI1RST);

	SPI1->I2SCFGR&=~SPI_I2SCFGR_I2SMOD; // Disable I2S mode.
	SPI1->CR1=SPI_CR1_SSM|(1*SPI_CR1_BR_0)|SPI_CR1_SSI|SPI_CR1_MSTR|SPI_CR1_SPE;

	// Configure CS pin as input to be compatible with VGA driver.
	// Set the output register to 0 anyway, and reconfigure later in LowerCS().
	SetGPIOInputMode(GPIOE,(1<<3));
	SetGPIOPushPullOutput(GPIOE,(1<<3));
	SetGPIOSpeed50MHz(GPIOE,(1<<3));
	SetGPIOPullUpResistor(GPIOE,(1<<3));
	GPIOE->BSRRH=1<<3;

	// Configure interrupt pins.
	SetGPIOInputMode(GPIOE,(1<<0)|(1<<1));
	SetGPIONoPullResistor(GPIOE,(1<<0)|(1<<1));
}

bool PingAccelerometer()
{
	uint8_t byte=ReadByte(LIS302DL_WHO_AM_I_ADDR);
	return byte==0x3b;
}

void SetAccelerometerMainConfig(uint8_t config)
{
	WriteByte(config,LIS302DL_CTRL_REG1_ADDR);
}

void SetAccelerometerFilterConfig(uint8_t config)
{
	WriteByte(config,LIS302DL_CTRL_REG2_ADDR);
}

void SetAccelerometerInterruptConfig(uint8_t config)
{
	WriteByte(config,LIS302DL_CLICK_CFG_REG_ADDR);
}

void ResetAccelerometer()
{
	uint8_t val=ReadByte(LIS302DL_CTRL_REG2_ADDR);
	WriteByte(val|LIS302DL_BOOT_REBOOTMEMORY,LIS302DL_CTRL_REG2_ADDR);
}

void ResetAccelerometerFilter()
{
	ReadByte(LIS302DL_HP_FILTER_RESET_REG_ADDR);
}

void ReadRawAccelerometerData(int8_t *values)
{
	uint8_t buffer[5];
	ReadBytes(buffer,LIS302DL_OUT_X_ADDR,5);
	values[0]=(int8_t)buffer[0];
	values[1]=(int8_t)buffer[2];
	values[2]=(int8_t)buffer[4];
}




#define ReadCommand 0x80 
#define MultiByteCommand 0x40

static uint8_t ReadByte(uint8_t address)
{  
	LowerCS();

	TransferByte(address|ReadCommand);
	uint8_t byte=TransferByte(0);

	RaiseCS();

	return byte;
}

static void ReadBytes(uint8_t *buffer,uint8_t address,int numbytes)
{  
	if(numbytes>1) address|=ReadCommand|MultiByteCommand;
	else address|=ReadCommand;

	LowerCS();

	TransferByte(address);
	for(int i=0;i<numbytes;i++) buffer[i]=TransferByte(0);

	RaiseCS();
}

static void WriteByte(uint8_t byte,uint8_t address)
{
	LowerCS();

	TransferByte(address);
	TransferByte(byte);

	RaiseCS();
}

static void WriteBytes(uint8_t *bytes,uint8_t address,int numbytes)
{
	if(numbytes>1) address|=MultiByteCommand;

	LowerCS();

	TransferByte(address);
	for(int i=0;i<numbytes;i++) TransferByte(bytes[i]);

	RaiseCS();
}

static inline uint8_t LowerCS()
{
	SetGPIOOutputMode(GPIOE,(1<<3));
}

static inline uint8_t RaiseCS()
{
	SetGPIOInputMode(GPIOE,(1<<3));
}

#define Timeout 0x1000

static uint8_t TransferByte(uint8_t byte)
{
	uint32_t timer=Timeout;
	while(!(SPI1->SR&SPI_SR_TXE)) if(timer--==0) return 0;

	SPI1->DR=byte;

	timer=Timeout;
	while(!(SPI1->SR&SPI_SR_RXNE)) if(timer--==0) return 0;

	return SPI1->DR;
}
