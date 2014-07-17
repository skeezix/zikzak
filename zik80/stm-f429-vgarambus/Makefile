
#HSE_XTAL=8000000
HSE_XTAL=16000000

# refering to libopencm3-examples Makefiles/ld-script
LCM3EX_BASE=/home/skeezix/archive/devo/toolkits/stm32/libopencm3-examples/examples/stm32/f4/

BINARY = main
APPOBJS = lib_serial.o lib_vga_cm3.o lib_vga_pixelclock.o system_cm3.o framebuffer.o framebuffer_demo.o lib_dma_memcpy.o logger.o commandqueue.o command.o lib_bus_ram.o skeelib.o lib_i2c.o
STMOBJS = stm32f4_misc.o stm32f4xx_gpio.o stm32f4xx_tim.o stm32f4xx_syscfg.o stm32f4xx_usart.o stm32f4xx_rcc.o stm32f4xx_dma.o stm32f4xx_i2c.o

LDSCRIPT = ${LCM3EX_BASE}/stm32f4-discovery/stm32f4-discovery.ld

# copied libopencm3 Makefile.include into a local copy, hacked it up a touch
#include ${LCM3EX_BASE}/Makefile.include
include Makefile.libopencm3
