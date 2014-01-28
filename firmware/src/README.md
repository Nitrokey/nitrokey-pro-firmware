= Crypto Stick firmware =

This is the firmware for the Crypto Stick 1.4 which includes the OATH feature.

== Compiling on Ubuntu Linux==
1) Download gcc-arm-none-eabi version 4.6 from https://launchpad.net/gcc-arm-embedded
2) extract archive to ~/bin
3) export PATH=$PATH:~/bin/gcc-arm-none-eabi-4_6-2012q4/bin/
4) change to cryptostick.oath/firmware/build/gcc
5) make
