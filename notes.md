# Development Notes

## Cheatsheet

```shell
# interesting files
build/gcc/nitrokey-*-firmware.*
build/gcc/stm32.ld
# flash via openocd in container
#   - the current dir is mounted at /app
make -f podman.mk openocd-server FLASH=/app/release/nitrokey-pro-firmware.hex
# run GDB server for debugging
make -f podman.mk
# build complete package for the release
make release
```

## Troubleshooting

Make sure that:

- the application is correctly moved to the beginning of the flash when flashed directly and without the bootloader;
- the base firmware used (forked off) supports the hardware you are working on;
- if bootloader is used, check its hardware configuration.

If the firmware fails while setting the basic clocks for the platform (before even initializing the smart card),
make sure the hardware detection is working properly. Make it static if needed.

Clion remote debug works out of the box, when connected to the OpenOCD GDB server.
Make sure to use the right elf file for the debugging.

## General Notes

1. Since the firmware v0.11 the bootloader is added to the Nitrokey Pro platform.
   This means, that the produced application binary will be offset by the
   bootloader size, and written directly will fail to boot on the hardware.

Offset size is 8kB. Excerpt from the linking file at `build/gcc/stm32.ld`:

```
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH (rx) : ORIGIN = 0x8000000+8K, LENGTH = 110K-8K
  USER (rx) : ORIGIN = 0x8000000 + 110K, LENGTH = 128K-110K
```

With the current configuration building the bootloader is mandatory. In the final binary image bootloader will be
merged properly, delegating the execution to the binary placed at the mentioned offset.
In case the bootloader removal is desired, the following should be changed:

- the linking script should point FLASH ORIGIN to the flash start;
- the merge step done with srec_cat tool (from the srecord package) should be skipped;

2. The Nitrokey HSM release is provided with the bootloader, however it is never switched to.

3. Hardware support
   Following hardware revisions are supported by these firmware revisions:

| Hardware revision | Supporting firmware           | Changes                                                                                                                                                                                                                                                                        |
|-------------------|-------------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| HW1-3             | all                           | -                                                                                                                                                                                                                                                                              |
| HW4               | v0.13 <br>  (v0.12 single HW) | Using different MCU packaging of the same model and vendor - change in GPIO pins; <br> Provided way for reading out the HW revision by comparing the specific GPIO pins HIGH/LOW state                                                                                         |
| HW5               | v0.15                         | Different MCU - switch to GD32, same packaging and GPIO pins setup as in HW1-3;<br> This MCU uses different clock setup, hence its peripherals won't work directly with the previous firmware;<br> It's possible to detect hardware revision by reading the MCU's vendor field |

4. The tested compilers are:

- ARM GCC 8.2.1
- ARM GCC 10.2.1

Using other compilers may bring in some binary incompatibilities. Do not change the compiler without thorough testing
the performance and reliability of the device.
Upgrading should not be avoided however, and rather pursued as an update in a better optimization, security and static
source code analysis.

Warning: GCCs 9.2.1 - 10.2 contain buggy stack smash protector code, and should not be used.
Details at:

- https://github.com/Nitrokey/nitrokey-pro-firmware/issues/86
- https://blog.inhq.net/posts/faulty-stack-canary-arm-systems/

5. The recent firmware releases are provided with a `.buildinfo` file, which contains information about the build
   environment configuration, specifically the compilers version and used flags.

6. The bootloader application used is located at:

- https://github.com/Nitrokey/dapboot/

7. All the required tools should be reachable over the Docker image, available as well over Podman. See `podman.mk` for
   the details. Make sure that the proper Udev rules are installed to have access to the debug adapter for flashing.

8. Nitrokey Pro firmware can be tested with the HSM smart card inserted. This is useful for debugging.

9. The HSM fork in the `hsm` and `hsm-hw4` branches contains the following changes:

- remove the HID interface
- remove the extra PWS/OTP features, which were available over HID
- change the way of getting the serial number of the smart card, including the usage of a TLV parsing

## Bootloader Notes

Random notes regarding the used bootloader:

- Check [libnitrokey] (for the execution of bootloader activation command), and [dapboot] (for the actual bootloader)
  projects.
- Data retention tests done - PWS tested only.
- Full libnitrokey test suite for Pro passed both after the update, and on merged firmware flashing.
- GnuPG tested and seems fine.
- Merged firmware (bootloader+fw) for one-pass flashing works.
- Firmware is write-verified by the bootloader, and the process is cancelled on write failure.
- Interrupting DFU update procedure may result in broken firmware state on device (having parts of two firmwares).
  Bootloader will not start by default in such case with the current configuration, but rather start the potentially
  broken
  firmware.
- Firmware is replaced on-the-fly, monotonically.
- Bootloader does not have access to the user space memory (over 110kB).
- When bootloader is activated, and the device is power-cycled without any action, it safely comes back to the main
  firmware application, without any change to the memory flash.

[libnitrokey]: https://github.com/Nitrokey/libnitrokey/blob/master/unittest/test_pro_bootloader.py

[dapboot]: https://github.com/Nitrokey/dapboot

## To Do

- Add Dapboot submodule
- Automate Dapboot's build
