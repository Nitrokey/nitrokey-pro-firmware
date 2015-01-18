Building
========
make firmware

Flashing with Versaloon
=======================
1. export OPENOCD_BIN=<path-to-openocd-bin-folder> && ./flash_versaloon.sh
   or edit the script directly to contain OPENOCD_BIN=<path-to-openocd-bin-folder>
2. make flash-vesaloon

(TODO: For now it has a bug. Run it once, then kill it with Ctrl-C, then re-run it and it should flash the image)

A proper OpenOCD (patched for SWD) seems to be this one:  
https://github.com/snowcap-electronics/OpenOCD-SWD
  
or this one which is configured for automake 1.14:  
https://github.com/ggkitsas/OpenOCD-SWD
