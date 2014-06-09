#!/bin/sh

arm-none-eabi-gdb -ex 'tar extended-remote localhost:3333' vgatest.elf
