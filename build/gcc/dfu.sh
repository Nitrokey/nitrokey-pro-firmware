#!/bin/bash
STM32_Programmer_CLI -c port=swd -e all -w  ../../../dapboot/build/dapboot-nkpro.bin  0x8000000 -v -rst
sleep 1
arm-none-eabi-objcopy -O binary nitrokey-pro-firmware.elf nitrokey-pro-firmware.bin 
sudo dfu-util -D nitrokey-pro-firmware.bin
