Nitrokey HSM firmware
=====================
This branch hosts a variation of Pro firmware adjusted for HSM device and with disabled HID interface. Below is the README copy for Pro firmware project.

To build this firmware just issue `make release` command in the main directory (PID will be set to HSM respectively, no need to tune it by hand). Requires `gcc-arm-none-eabi` package under Ubuntu 16.04.


Nitrokey Pro firmware 
=====================

The following information is about the firmware of the Nitrokey Pro. For information about the hardware
please have a look at the [Nitrokey Pro hardware
repo](https://github.com/Nitrokey/nitrokey-pro-hardware).

* [Overview](#Overview)
* [Building](#building)
* [Flashing](#flashing)
  * [SWD](#SWD)
    * [Requirements](#requirements)
    * [Flashing](#flashing-and-development-access)
  * [DFU](#DFU)
    * [Requirements](#dfu-requirements)
    * [Flashing](#flashing-via-dfu)

# Overview
Nitrokey Pro, Start and HSM use the same hardware but different firmwares and different smart cards. The microprocessor being used is a STM32F103R8T6. The firmware is written in C, the desktop software Nitrokey App is written in C/C++.

To develop the firmware of the Nitrokey Pro/Start/HSM you would need:
* An original Nitrokey Pro/Start/HSM or better a development board such as the [Nucleo-F103RB] or the [Olimex STM32-H103]. Alternatively, get any other development board equipped with a STM32F103TB and 128KB flash. On request you can get a Nitrokey for development purposes from us.
* An OpenPGP Card 3.4 available at [FLOSS Shop] or on request from us. (Of course, this is not necessary for Nitrokey Start which doesn't contain a smart card.)
If you use it with original Nitrokey hardware, you would need to cut it to Micro-SIM size. This can be done by using a special SIM card cutter or even with scissors.
If you use a development board, you may solder the OpenPGP Card to the board directly by using some wires or you get yourself a smart card jack which you solder to the dev board instead.
* To compile the firmware we recommend [ARM's official GNU tools].

[FLOSS Shop]: https://www.floss-shop.de/
[Nucleo-F103RB]: https://www.st.com/en/evaluation-tools/nucleo-f103rb.html
[Olimex STM32-H103]: https://www.olimex.com/Products/ARM/ST/STM32-H103/
[ARM's official GNU tools]: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm


# Building

`make [VID=0x20a0] [PID=0x4108] firmware`

Parameters:
* VID: Define Vendor ID
* PID: Define Product ID

# Flashing

|Note|
|-----|
|Any user data present on the device will be erased when flashing it. A backup is essential to prevent data loss.|

The microcontroller can be flashed in one of the following ways, depending on your hardware version:
* **all hardware versions:** [SWD](#SWD) is a STM-specific protocol and similar to JTAG allowing programming and debugging. Working adapters are Versaloon or any of the ST-Link V2 (clones). Under Linux  the recent OpenOCD works quite well. This approach requires soldering wires to the contact pads or to use an adapter with pogo pins and some kind of mounting (recommended).
* **purchased before 04/04/2018:** [DFU](#DFU) is a simple protocol via serial port which allows programming but no debugging. On older Nitrokey versions, the appropriate pins are exposed over the USB connector (though it is *not* USB, the pin is only shared between these two).

## SWD

### Requirements

* Download the .hex file you want to flash e.g. look at the [releases section](https://github.com/Nitrokey/nitrokey-pro-firmware/releases) or build it yourself (see above).
* Any SWD compatible programmer for ST microcontrollers. They come as part of ST's line of [Discovery] and [Nucleo] boards or can be bought seperately from [ST] as well as as [clones] for around $5 on eBay, Amazon or AliExpress (search for "ST-Link v2")

[Discovery]: https://www.st.com/en/evaluation-tools/stm32-discovery-kits.html
[Nucleo]: https://www.st.com/en/evaluation-tools/stm32-nucleo-boards.html
[ST]: https://www.st.com/en/development-tools/st-link-v2.html
[clones]: http://www.ebay.com/sch/i.html?_from=R40&_trksid=p2050601.m570.l1313.TR0.TRC0.H0.Xstlink+v2.TRS0&_nkw=stlink+v2&_sacat=0

The following picture shows the pin pads of the Nitrokey. The red rectangle is only available in
newer versions and easier to use as the pads are much bigger. The blue rectangle is present in older
and newer devices.

![SWD pins of newer Nitrokey Pro device](images/adapter_v2.jpg?raw=true)

The SWD pins are as follows:

![NK Pro v2.0 Programming Connector Layout](images/conn_layout.png)


For SWD programming, connect the SWDIO, SWDCLK and GND pads to the respective pins of your ST-Link programmer. The device should be powered externally through USB or a 5V power supply during programming.

### Flashing and Development Access

See the [Development Guide] for the current use.

#### OpenOCD

Modern OpenOCD works quite well, if not better than the official tools (especially for the debugging). 

##### GDB Server
```bash
  openocd -f interface/stlink-v2.cfg  -f target/stm32f1x.cfg
```


##### Reading MCU Flash
Make sure the MCU is not memory protected, otherwise this operation will fail.

```text
$ cat <<END >stm32read.cfg 
source [find interface/stlink.cfg]
source [find target/stm32f1x.cfg]
init
flash read_bank 0 firmware.bin 0 0x20000
exit
END
$ openocd -f stm32read.cfg
```

#### STM32 Official Tool

Official tool is available at [stm32cubeprog].

[stm32cubeprog]: https://www.st.com/en/development-tools/stm32cubeprog.html

##### Flashing STM32

```bash
  STM32_Programmer_CLI -c port=SWD -halt  --readunprotect
  STM32_Programmer_CLI -c port=swd -e all -w firmware.hex 0x8000000 -v -rst
```

##### GDB Server

```text
  st-util
```

[Development Guide]: ./DEVELOPMENT.md

## DFU

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
