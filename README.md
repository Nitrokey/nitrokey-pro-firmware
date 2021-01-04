Nitrokey HSM firmware
=====================
This branch hosts a variation of Pro firmware adjusted for HSM device and with disabled HID interface. Below is the README copy for Pro firmware project.

To build this firmware just issue `make release` command in the main directory (PID will be set to HSM respectively, no need to tune it by hand). Requires `gcc-arm-none-eabi` package under Ubuntu 16.04.


Nitrokey Pro firmware [![Build Status](https://travis-ci.org/Nitrokey/nitrokey-pro-firmware.svg?branch=master)](https://travis-ci.org/Nitrokey/nitrokey-pro-firmware)  [![Code Health](https://landscape.io/github/Nitrokey/nitrokey-pro-firmware/master/landscape.svg?style=flat)](https://landscape.io/github/Nitrokey/nitrokey-pro-firmware/master) [![Coverity Scan Build](https://scan.coverity.com/projects/4745/badge.svg)](https://scan.coverity.com/projects/4745)
=====================

Building
========
make \[VID=0x20a0\] \[PID=0x4108\] firmware

Parameters:
* VID: Define Vendor ID
* PID: Define Product ID

Flashing
=======================
#### Versaloon
1. export OPENOCD_BIN=\<path-to-openocd-bin-folder\> && ./flash_versaloon.sh
   or edit the script directly to contain OPENOCD_BIN=\<path-to-openocd-bin-folder\>
2. make flash-vesaloon

(TODO: For now it has a bug. Run it once, then kill it with Ctrl-C, then re-run it and it should flash the image)

A proper OpenOCD (patched for SWD) seems to be this one:
https://github.com/snowcap-electronics/OpenOCD-SWD

or this one which is configured for automake 1.14:
https://github.com/ggkitsas/OpenOCD-SWD

#### STM
1. When using the MCU's bootloader use STM Boot Loader Demonstrator or [stm32flash](http://sourceforge.net/p/stm32flash/wiki/Home/) under Linux.
2. Read-protect the flash

`sudo stm32flash -w crypto.hex -v /dev/ttyUSB0`


#### GDB
```text
# gdb command for debugging docker build
set substitute-path ../../src ./src/
```

