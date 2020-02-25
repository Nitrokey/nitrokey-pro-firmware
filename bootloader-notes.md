Firmware side work is done in wip-bootloader_pro.
Check TODOs in the wip-bootloader branch.
Check libnitrokey (for execution), and dapboot (for actual bootloader) projects.

Data retention tests done - PWS tested only.
Full libnitrokey test suite for Pro passed after the update, and on merged firmware flashing.
GnuPG tested and seems fine.
Merged firmware (bootloader+fw) for one-pass flashing works.
Firmware is write-verified by the bootloader, and the process is cancelled on write failure. 
Interrupting DFU update procedure may result in broken firmware state on device. Bootloader will not start by default in such case with current configuration, but rather start the broken firmware.
Firmware is replaced on-the-fly, monotonically.
Bootloader does not have access to the user space memory (over 110kB).
