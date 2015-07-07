#!/bin/bash

sudo $OPENOCD_BIN/openocd   -f interface/stlink-v2.cfg \
                            -f target/stm32f1x.cfg \
                            -c "init" \
                            -c "reset" \
                            -c "halt" \
                            -c "flash write_image erase ../build/gcc/crypto.elf" \
                            -c "verify_image ../build/gcc/crypto.elf" 			 \
                            -c "reset run"
#                            -c "shutdown"

                            #-f board/olimex_stm32_h103.cfg \
