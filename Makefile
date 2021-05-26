ROOT_DIR=$(CURDIR)
BUILD_DIR=$(ROOT_DIR)/build/gcc
SCRIPT_DIR=$(ROOT_DIR)/scripts
OPENOCD_BIN?=

DEPS=gcc-arm-none-eabi

.PHONY: firmware flash-versaloon clean release

firmware:
	cd $(BUILD_DIR) && \
	$(MAKE) && \
	cd -
#	mv $(BUILD_DIR)/crypto.elf .

#Reminder:	export OPENOCD_BIN=$(OPENOCD_BIN) 
flash-versaloon:
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

deps:
	sudo apt-get install ${DEPS}

release: firmware
	mkdir -p release && \
	cd release && \
	cp $(BUILD_DIR)/nitrokey-pro-firmware.elf $(BUILD_DIR)/nitrokey-pro-firmware.hex . && \
	find . -name *.elf -type f -printf "%f" | xargs -0 -n1 -I{} sh -c 'sha512sum -b {} > {}.sha512; md5sum -b {} > {}.md5' && \
    find . -name *.hex -type f -printf "%f" | xargs -0 -n1 -I{} sh -c 'sha512sum -b {} > {}.sha512; md5sum -b {} > {}.md5'
