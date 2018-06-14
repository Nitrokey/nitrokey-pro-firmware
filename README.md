Nitrokey Pro firmware [![Build Status](https://travis-ci.org/Nitrokey/nitrokey-pro-firmware.svg?branch=master)](https://travis-ci.org/Nitrokey/nitrokey-pro-firmware)  [![Code Health](https://landscape.io/github/Nitrokey/nitrokey-pro-firmware/master/landscape.svg?style=flat)](https://landscape.io/github/Nitrokey/nitrokey-pro-firmware/master) [![Coverity Scan Build](https://scan.coverity.com/projects/4745/badge.svg)](https://scan.coverity.com/projects/4745)
=====================

* [Building](#building)
* [Flashing](#flashing)
  * [STM32flash and DFU Mode](#stm32flash-and-dfu-mode)
    * [Requirements](#requirements)
    * [Actual Flashing](#actual-flashing)
  * [Versaloon](#versaloon)

# Building

make \[VID=0x20a0\] \[PID=0x4108\] firmware

Parameters:
* VID: Define Vendor ID
* PID: Define Product ID

# Flashing

The microprocessor can be flashed in two different ways:
* DFU is a simple protocol via serial port which allows programming but no debugging. On the Nitrokey hardware we expose the appropriate pins over the USB connector but it's not USB! Details are described in the next chapter.
* SWD is a STM-specific protocol and similar to JTAG allowing programming and debugging. Working adapters are Versaloon or any of the ST-Link V2 (clones). Under Linux you could give a patched OpenOCD a try but in the past it has been very troublesome. This approach requires to solder wires to the contact pads or to use an adapter with pogo pins and some kind of mounting (recommended).

## STM32flash and DFU Mode
### Requirements

* Download the .hex file you want to flash e.g. look at the [releases section](https://github.com/Nitrokey/nitrokey-pro-firmware/releases) or build it yourself (see above).
* You may use [STM32 Flash Loader Demonstrator](http://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/flasher-stm32.html) (Windows only) or the open source command line tool [stm32flash](http://stm32flash.sourceforge.net). *Note: the terminal commands below are based on the command line tool.*
* If your computer doesn't has a RS232 port (most modern laptops don't have it) you would need a USB-to-RS232/TTL adapter. [Sparkfun BOB-00718](https://www.sparkfun.com/products/718) should work (untested) and you can find even cheaper adapters online. Previously we built [our own adapter](https://www.assembla.com/spaces/cryptostick/wiki/Crypto_Stick_Programmer) which hardware layout you can [download](https://www.assembla.com/spaces/cryptostick/subversion/source/HEAD/trunk/hardware/programmer%20for%20v1.2).
* You would need a simple USB adapter to bridge Nitrokey's USB plug to the USB-to-RS232 adapter.

Your adapter should consist of a USB socket which four pins are connected to your serial/TTL connector. The pinout is as follows.

```
Nitrokey USB Plug <-> Serial/TTL adapter

Pin 1, VCC <-> VCC
Pin 2, D-  <-> TX
Pin 3, D+  <-> RX
Pin 4, GND <-> GND

This diagram represents the pinout of the USB socket which you are going to solder:

  ################### 
  #                 # 
  # ############### # 
  #                 # 
  #                 # 
  ################### 
     #   #   #   #   
     #   #   #   #    

     1   2   3   4
```

The following picture shows the adapter/USB-to-TTL connection.
![USB-to-TTL adapter and USB socket](adapter.jpg?raw=true)

To flash the firmware you need to bridge the two contact holes and only then connect (and power) the PCB to your adapter. The bridge triggers the hardware to boot into DFU mode. You can use a jumper with 2.0 mm pitch or just prepare/solder a wire. The following picture shows a bridge for the Nitrokey.

![Nitrokey bridged with a jumper](connected_jumper.jpg?raw=true)

### Actual Flashing 

While the jumper is plugged in, connect the Nitrokey to the USB-serial adapter on your computer. The jumper is only required during the first moment of connection and can be removed afterwards.

You can check if the Nitrokey got successfully into DFU mode by typing in the following into a terminal:

```
$ sudo stm32flash /dev/ttyUSB0

stm32flash 0.5

http://stm32flash.sourceforge.net/

Error probing interface "serial_posix"
Cannot handle device "/dev/ttyUSB0"
Failed to open port: /dev/ttyUSB0
```

Now we have to disable the read protection first by typing

```
sudo stm32flash -k /dev/ttyUSB0 # read unprotecting
```

You may need to reconnect the device, before you can proceed. Do not forget to bridge the holes again. Now we do the actual flashing:

```
sudo stm32flash -w nitrokey-pro-firmware.hex /dev/ttyUSB0
```

Enabling the read/write protection again:

```
sudo stm32flash -j /dev/ttyUSB0 # read protection
```

## Versaloon
1. export OPENOCD_BIN=\<path-to-openocd-bin-folder\> && ./flash_versaloon.sh
   or edit the script directly to contain OPENOCD_BIN=\<path-to-openocd-bin-folder\>
2. make flash-vesaloon

(TODO: For now it has a bug. Run it once, then kill it with Ctrl-C, then re-run it and it should flash the image)

A proper OpenOCD (patched for SWD) seems to be this one:
https://github.com/snowcap-electronics/OpenOCD-SWD

or this one which is configured for automake 1.14:
https://github.com/ggkitsas/OpenOCD-SWD
