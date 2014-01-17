#!/bin/sh

# after "init" can do this..
#                     -c "stm32f2x mass_erase 0" \
#                    -c "flash erase_address 0x08000000 0x20000" \

openocd -f interface/stlink-v2.cfg \
                    -f target/stm32f2x_stlink.cfg \
                    -c "init" -c "reset init" \
                    -c "flash list" \
                    -c "stm32f2x mass_erase 0" \
                    -c "flash write_image vgatest.hex" \
                    -c "reset run" \
                    -c "shutdown"