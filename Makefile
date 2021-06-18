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
	mkdir -p release
	-rm -r release/*.*
	cp $(shell readlink -f $(BUILD_DIR)/last.hex) $(shell readlink -f $(BUILD_DIR)/last.buildinfo) release/
	cd build/gcc && $(MAKE) -f dfu.mk flash-full-single
	cd release && find . -name *.hex -type f -printf "%f" | xargs -0 -n1 -I{} sh -c 'sha512sum -b {} > {}.sha512'
	ls release

.PHONY: gdbserver
gdbserver:
	openocd -f interface/stlink-v2.cfg  -f target/stm32f1x.cfg

.PHONY: ocdtelnet
ocdtelnet:
	telnet localhost 4444

HW_REV?=4
.PHONY: devloop
devloop: | clean
	$(MAKE) firmware -j12 BUILD_DEBUG=1 HW_REV=$(HW_REV)
	-# killall telnet
	- killall openocd
	cd build/gcc && $(MAKE) -f dfu.mk flash-full-single
	$(MAKE) gdbserver > /dev/null &
	sleep 1
	$(MAKE) ocdtelnet

.PHONY: devloop-release
devloop-release: | clean
	$(MAKE) firmware -j12 HW_REV=$(HW_REV)
	- killall openocd
	cd build/gcc && $(MAKE) -f dfu.mk flash-full-single
