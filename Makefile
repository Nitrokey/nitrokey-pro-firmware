ROOT_DIR=$(CURDIR)
BUILD_DIR=$(ROOT_DIR)/build/gcc
SCRIPT_DIR=$(ROOT_DIR)/scripts
OPENOCD_BIN?=

DEPS=gcc-arm-none-eabi

.PHONY: firmware flash-versaloon clean release

HW_REV?=4

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
	-rm nitrokey-*-firmware-hw*.tar.gz

deps:
	sudo apt-get install ${DEPS}

.PHONY: release release-all
release: | clean
	mkdir -p release
	-rm -r release/*.*
	$(MAKE) firmware -j12 HW_REV=$(HW_REV)
	cd build/gcc && $(MAKE) -f dfu.mk firmware.hex
	cd build/gcc && $(MAKE) -f dfu.mk all.hex
	cp `readlink -f $(BUILD_DIR)/last.hex` `readlink -f $(BUILD_DIR)/last_update.hex` `readlink -f $(BUILD_DIR)/last.buildinfo` release/
	cd release && find . -name "*.hex" -type f -printf "%f\0" | xargs -0 -n1 -I{} sh -c 'sha512sum -b {} > {}.sha512'
	ls -lh release
	tar -czvf nitrokey-pro-firmware-hw$(HW_REV).tar.gz -C release .

release-all:
	mkdir -p release-all
	-rm -r release-all/*.*
	$(MAKE) release HW_REV=3
	mv *tar.gz release-all
	$(MAKE) release HW_REV=4
	mv *tar.gz release-all
	ls -lh release-all/

.PHONY: gdbserver
gdbserver:
	openocd -f interface/stlink-v2.cfg  -f target/stm32f1x.cfg

.PHONY: ocdtelnet
ocdtelnet:
	telnet localhost 4444

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
