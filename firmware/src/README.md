= Crypto Stick firmware =

This is the firmware for the Crypto Stick 1.4 which includes the OATH feature.

== Compiling on Ubuntu Linux ==
1) Install gcc-arm-none-eabi from https://launchpad.net/gcc-arm-embedded
2) change to cryptostick.oath/firmware/build/gcc
3) make

== Flashing ==
1) Flashing: When using Versaloon programmer use vsprog. When using the MCU's bootloader use STM Boot Loader Demonstrator or stm32flash [1] under Linux.
2) Read-protect the flash

[1] https://code.google.com/p/stm32flash/
    sudo ~/bin/stm32flash -w crypto.hex -v /dev/ttyUSB0

== Using a Different Toolchain ==
In the makefile you have to change the tool names according to your toolchain
I changed them to "CC = arm-none-eabi-gcc" etc.
In hw_config.c and platform_config.h you have to set up the USB
disconnect pin, because I had to change it for my devboard.


