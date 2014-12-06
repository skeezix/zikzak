#!/bin/sh

PATH=/usr/lib/lightdm/lightdm:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/home/skeezix/archive/devo/toolkits/avr-gcc-atmel/avr8-gnu-toolchain-linux_x86_64/bin/:/home/skeezix/archive/devo/toolkits/stm32/gcc-arm-none-eabi-4_8-2013q4/bin
export PATH

arm-none-eabi-gdb -ex 'tar extended-remote localhost:3333' vgatest.elf
