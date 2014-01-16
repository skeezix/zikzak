#!/bin/sh

# after "init" can do this..
#                     -c "stm32f2x mass_erase 0" \

openocd -f interface/stlink-v2.cfg \
                    -f target/stm32f2x_stlink.cfg \
                    -c "init" -c "reset init" \
                    -c "stm32f2x mass_erase 0" \
                    -c "flash write_image vgatest.hex" \
                    -c "reset run" \
                    -c "shutdown"