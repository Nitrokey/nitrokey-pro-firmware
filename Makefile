ROOT_DIR=$(CURDIR)
BUILD_DIR=$(ROOT_DIR)/build/gcc
SCRIPT_DIR=$(ROOT_DIR)/scripts
OPENOCD_BIN?=~/Projects/openocd/build/bin

.PHONY: firmware flash-versaloon clean

firmware:
	cd $(BUILD_DIR) && \
	make && \
	cd - && \
	mv $(BUILD_DIR)/crypto.elf .


flash-versaloon:
	export OPENOCD_BIN=
	cd scripts && \
	./flash-versaloon.sh

#flash-versaloon:
#	sudo $(OPENOCD_BIN)/openocd gdb_memory_map disable 			\
								-f interface/vsllink-swd.cfg 	\
								-f target/stm32f1x.cfg 			\
								-c init -c reset -c halt 		\
								-c "stm32f1x unlock 0"			\
								-c shutdown						\
								-c exit							
#	sudo $(OPENOCD_BIN)/openocd gdb_memory_map disable 			\
								-f interface/vsllink-swd.cfg 	\
								-f target/stm32f1x.cfg			\
								-c init -c reset -c halt		\
								-c "flash write_image erase crypto.elf" \
								-c "verify_image crypto.elf" 			\
								-c "reset run"							\
								-c exit
	#sudo $(OPENOCD_BIN)/openocd gdb_memory_map disable -f interface/vsllink-swd.cfg -f target/stm32f1x.cfg -f $(SCRIPT_DIR)/write.tcl
	
clean:
	cd $(BUILD_DIR) && \
	make clean
