# Development guide

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


## Interesting places

- `main.c`
- `report_protocol.c`
- `smartcard.c`

## Run tests

Assuming `WORKSPACE` is `~/work`. Can be changed by specifying in the call - see below. 
```text
$ make -f build/gcc/dfu.mk run_quick_test
$ make -f build/gcc/dfu.mk run_quick_test PYTEST_ARG="test_pro.py -v --randomly-dont-reorganize" WORKSPACE=~/workspace
```