Nitrokey Pro firmware [![Build Status](https://travis-ci.org/Nitrokey/nitrokey-pro-firmware.svg?branch=master)](https://travis-ci.org/Nitrokey/nitrokey-pro-firmware)  [![Code Health](https://landscape.io/github/Nitrokey/nitrokey-pro-firmware/master/landscape.svg?style=flat)](https://landscape.io/github/Nitrokey/nitrokey-pro-firmware/master) [![Coverity Scan Build](https://scan.coverity.com/projects/4745/badge.svg)](https://scan.coverity.com/projects/4745)
=====================

The following information is about the firmware of the Nitrokey Pro. For information about the hardware
please have a look at the [Nitrokey Pro hardware
repo](https://github.com/Nitrokey/nitrokey-pro-hardware).

* [Building](#building)
* [Flashing](#flashing)
  * [Versaloon](#versaloon-and-st-link-v2)
    * [Requirements](#requirements)
    * [Flashing](#flashing)
  * [STM32flash and DFU Mode](#stm32flash-and-dfu-mode)
    * [Requirements](#dfu-requirements)
    * [Flashing](#flashing-via-dfu)

# Overview
Nitrokey Pro, Start and HSM use the same hardware but different firmwares and different smart cards. The microprocessor being used is a STM32F103R8T6. The firmware is written in C, the desktop software Nitrokey App is written in C/C++.

To develop the firmware of the Nitrokey Pro/Start/HSM you would need:
* An original Nitrokey Pro/Start/HSM or better a development board such as the [Nucleo-F103RB]. Alternatively, get any other development board equipped with a STM32F103TB and 128KB flash. On request you can get a Nitrokey for development purposes from us.
* An OpenPGP Card 2.1 available at [Kernel Concepts] or on request from us. (Of course, this is not necessary for Nitrokey Start which doesn't contain a smart card.)
If you use it with original Nitrokey hardware, you would need to cut it to Micro-SIM size. This can be done by using a special SIM card cutter or even with a scissor.
If you use a development board, you may solder the OpenPGP Card to the board directly by using some wires or you get yourself a smart card jack which you solder to the dev board instead.
* To compile the firmware we recommend [ARM's official GNU tools].
[Kernel Concepts]: http://shop.kernelconcepts.de/
[Nucleo-F103RB]: https://www.st.com/en/evaluation-tools/nucleo-f103rb.html
[ARM's official GNU tools]: https://launchpad.net/gcc-arm-embedded/


# Building

make \[VID=0x20a0\] \[PID=0x4108\] firmware

Parameters:
* VID: Define Vendor ID
* PID: Define Product ID

# Flashing

|Note|
|-----|
|Any user data present on the device will be erased when flashing it. A backup is essential to prevent data loss.|

The microprocessor can be flashed in one of the following ways, depending on your hardware version:
* SWD is a STM-specific protocol and similar to JTAG allowing programming and debugging. Working adapters are Versaloon or any of the ST-Link V2 (clones). Under Linux you could give a patched OpenOCD a try but in the past it has been very troublesome. This approach requires to solder wires to the contact pads or to use an adapter with pogo pins and some kind of mounting (recommended).
* DFU is a simple protocol via serial port which allows programming but no debugging. On the Nitrokey hardware we expose the appropriate pins over the USB connector but it's not USB! Details are described in the next chapter.

|Note|
|-----|
|From hardware version 2 (04/04/2018) onwards, using the MCU's DFU bootloader is no longer possible|

## Versaloon and ST-Link V2

### Requirements

* Download the .hex file you want to flash e.g. look at the [releases section](https://github.com/Nitrokey/nitrokey-pro-firmware/releases) or build it yourself (see above).
* You may use a [ST-Link V2 programmer](https://www.ebay.com/sch/i.html?_odkw=st-link&_osacat=0&_from=R40&_trksid=p2045573.m570.l1313.TR0.TRC0.H0.Xst-link+v2&_nkw=st-link+v2&_sacat=0) or a Versaloon adapter.

The following picture shows the pin pads of the Nitrokey. The red rectangular is only available in
newer versions and easier to use as the pads are much bigger. The blue rectangular is present in older
and newer devices.

![SWD pins of newer Nitrokey Pro device](images/adapter_v2.jpg?raw=true)

The SWD pins for the red rectangular is as follows:

![NK Pro v2.0 Programming Connector Layout](images/conn_layout.png)

The SWD pins for the blue rectangular is as follows:

```
_________________________
 D   x    o    o         |________
VCC GND SWCLK SWIO       |        |
                         |        |
                         |        |
                         |________|
_________________________|
```

For SWD programming, connect the SWDIO, SWDCLK and GND pads to the respective pins of you ST-Link programmer. The device should be powered externally through USB or a 5V power supply during programming.

### Flashing

1. export OPENOCD_BIN=\<path-to-openocd-bin-folder\> && ./flash_versaloon.sh
   or edit the script directly to contain OPENOCD_BIN=\<path-to-openocd-bin-folder\>
2. make flash-vesaloon

(TODO: For now it has a bug. Run it once, then kill it with Ctrl-C, then re-run it and it should flash the image)

A proper OpenOCD (patched for SWD) seems to be this one:
https://github.com/snowcap-electronics/OpenOCD-SWD

or this one which is configured for automake 1.14:
https://github.com/ggkitsas/OpenOCD-SWD

## STM32flash and DFU Mode

Please note, that this approach only works for older Nitrokey Pro device, not Nitrokey Pro 2 (all devices purchased before 04/04/2018).

### DFU Requirements

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
![USB-to-TTL adapter and USB socket](images/adapter.jpg?raw=true)

To flash the firmware you need to bridge the two contact holes and only then connect (and power) the PCB to your adapter. The bridge triggers the hardware to boot into DFU mode. You can use a jumper with 2.0 mm pitch or just prepare/solder a wire. The following picture shows a bridge for the Nitrokey.

![Nitrokey bridged with a jumper](images/connected_jumper.jpg?raw=true)

### Flashing via DFU

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
