#!/bin/sh
#Flash the board using vsprog
vsprog -sstm32f1 -ms -oe -owf -ovf  -I ../build/gcc/crypto.hex

