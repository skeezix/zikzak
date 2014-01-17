#!/bin/sh
openocd -f interface/stlink-v2.cfg \
                    -f target/stm32f2x_stlink.cfg \
                    -c "init" -c "reset init" \
                    -c "stm32f2x mass_erase 0" \
                    -c "flash write_image vgatest.hex" \
                    -c "reset" \
                    $(NULL)

# now, to connect from gdb..
# arm-none-eabi-gdb miniblink.elf
# > tar extended-remote localhost:3333
# can also load code and run it:
# > load
