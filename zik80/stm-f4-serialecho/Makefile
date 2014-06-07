
# refering to libopencm3-examples Makefiles/ld-script
LCM3EX_BASE=/home/skeezix/archive/devo/toolkits/stm32/libopencm3-examples/examples/stm32/f4/

BINARY = main
APPOBJS = lib_serial.o
STMOBJS = stm32f4_misc.o stm32f4xx_gpio.o stm32f4xx_tim.o stm32f4xx_syscfg.o stm32f4xx_usart.o stm32f4xx_rcc.o

LDSCRIPT = ${LCM3EX_BASE}/stm32f4-discovery/stm32f4-discovery.ld

# copied libopencm3 Makefile.include into a local copy, hacked it up a touch
#include ${LCM3EX_BASE}/Makefile.include
include Makefile.libopencm3
