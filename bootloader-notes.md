Random notes regarding used bootloader:
- Check [libnitrokey] (for execution of bootloader activation command), and [dapboot] (for actual bootloader) projects.
- Data retention tests done - PWS tested only.
- Full libnitrokey test suite for Pro passed both after the update, and on merged firmware flashing.
- GnuPG tested and seems fine.
- Merged firmware (bootloader+fw) for one-pass flashing works.
- Firmware is write-verified by the bootloader, and the process is cancelled on write failure. This might result in broken firmware state.
- Interrupting DFU update procedure may result in broken firmware state on device (having parts of two firmwares). Bootloader will not start by default in such case with current configuration, but rather start the potentially broken firmware.
- Firmware is replaced on-the-fly, monotonically.
- Bootloader does not have access to the user space memory (over 110kB).
- When bootloader is activated, and the device is power-cycled without any action, it safely comes back to the main firmware application, without any change to the memory flash.


[libnitrokey]: https://github.com/Nitrokey/libnitrokey/blob/master/unittest/test_pro_bootloader.py
[dapboot]: https://github.com/Nitrokey/dapboot
