# Development Guide

## Development

The handy development loop is prepared and can be executed by calling:

```bash
make devloop
```

This will:
- build the firmware binaries in the development mode (`-Og`), which:
  - has section-wide GC enabled;
  - uses debug version of the LD script, discarding some additional sections from the vendor libraries;
- flash the firmware to the connected sample using SWD
- start openocd GDB server, and connect a telnet client to it

```bash
make devloop-release
```
This will:
- build the firmware binaries in the release mode (`-O2`)
- flash the firmware to the connected sample using SWD

Both will prepare binaries using the default `arm-none-eabi-gcc` installed compiler. Compiler can be changed by supplying the `CC=` argument.


## Interesting Places

- `main.c`
- `report_protocol.c`
- `smartcard.c`

## Run Tests

Assuming `WORKSPACE` is `~/work`. Path be changed by specifying in the call - see below. 
```text
$ make -f build/gcc/dfu.mk run_quick_test
$ make -f build/gcc/dfu.mk run_quick_test PYTEST_ARG="test_pro.py -v --randomly-dont-reorganize" WORKSPACE=~/workspace
```

## Firmware Update

The fastest way to update the firmware is to use pynitrokey:

```text
$ nitropy pro enable-update
$ nitropy pro update nitrokey-pro-firmware-v0.14-RC3-to_update.bin
```

In case the libnitrokey could not be found automatically, the path to it can be provided with env. variable:
```text
$ env LIBNK_PATH=/libnk/path/libnitrokey.so nitropy pro enable-update
```

To find libnitrokey on your system use:
```text
$ locate libnitrokey.so
```

### Alternative Update Method
1. `dfu-util` can be used for the firmware update:
    ```text
    $ dfu-util -D update_binary.bin
    ```
   1. Binary for Windows is available at:
       - http://dfu-util.sourceforge.net/releases/dfu-util-0.10-binaries.tar.xz
       - http://dfu-util.sourceforge.net/releases/
   2. On macOS it's available with Homebrew:
      - https://formulae.brew.sh/formula/dfu-util
         ```text
         brew install dfu-util
         ``` 
2. Bootloader can be activated with Nitrokey App v1.5-RC7.


### UDEV rules (Linux only)

For the actual firmware update latest UDEV rules need to be installed - tip about that is shown with the error message, when the access is denied to the device.

This could be achieved with installing the latest libnitrokey package, or using the following snippet:
```text
$ curl https://raw.githubusercontent.com/Nitrokey/libnitrokey/master/data/41-nitrokey.rules | sudo tee /usr/lib/udev/rules.d/41-nitrokey.rules
```

### Cancelling Update

Update process can be canceled only before the actual update has started (e.g. after bootloader activation, but before running `update` command), by reinserting the device to the USB port. It should boot the last working firmware.