#!/bin/bash

#sudo $OPENOCD_BIN/openocd gdb_memory_map disable 			    \
#								-f interface/vsllink-swd.cfg 	\
#								-f target/stm32f1x.cfg 			\
#								-c init -c reset -c halt 		\
#								-c "stm32f1x unlock 0"			\
#								-c shutdown						\
#								-c exit							
#sleep 2

sudo $OPENOCD_BIN/openocd gdb_memory_map disable 			    \
								-f interface/vsllink-swd.cfg 	\
								-f target/stm32f1x.cfg			\
								-c init -c reset -c halt		\
								-c "flash write_image erase ../build/gcc/crypto.elf" \
								-c "verify_image crypto.elf" 			\
								-c "reset run"							\
								-c exit

